/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "corpus.hpp"
#include "sentence.hpp"
#include "dictionary.hpp"
#include "app/core.hpp"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QDomDocument>

using SpeechControl::Core;
using SpeechControl::Corpus;
using SpeechControl::Sentence;
using SpeechControl::Dictionary;
using SpeechControl::CorpusList;
using SpeechControl::SentenceList;

Corpus::Corpus (const QUuid& p_uuid) : QObject(), m_dom (new QDomDocument)
{
    load (p_uuid);
}

/// @todo This list has to be ordered from '0' to 'n-1'.
/// @note Since this sorts the values by index, the value returned should be called once and stored into a variable, to prevent overhead.
/// @todo Cache the ordered listing and update it whenever it's modified.
SentenceList Corpus::sentences() const
{
    return m_sntncLst;
}

/// @todo Add the Sentence to the XML document and then to the list.
Sentence* Corpus::addSentence (Sentence* p_sentence)
{
    m_sntncLst << p_sentence;
    return p_sentence;
}

Sentence* Corpus::addSentence (const QString& p_text, const QFile* p_audio)
{
    Sentence* l_sentence = Sentence::create (this, p_text);

    if (p_audio) {
        l_sentence->getElement()->attribute (p_audio->fileName());
    }

    return l_sentence;
}

Corpus& Corpus::operator << (Sentence* p_sentence)
{
    this->addSentence (p_sentence);
    return *this;
}

/// @todo Just invoke the above method.
Corpus& Corpus::operator << (SentenceList& p_sentenceList)
{
    Q_FOREACH (Sentence * l_phrs, p_sentenceList) {
        this->addSentence (l_phrs);
    }

    return *this;
}

/// @todo Build a dictionary from the said text.
/// @todo Find a way to keep the text in an ordinal fashion.
Corpus* Corpus::create (const QStringList& p_text)
{
    QUuid uuid = QUuid::createUuid();
    QDir dir (getPath (uuid));

    if (!dir.mkpath (dir.path())) {
        qDebug() << "[Corpus::create()] Can't make corpus directory" << uuid << "at" << dir.path();
        return 0;
    }

    QDomDocument dom ("Corpus");
    QDomElement rootElem = dom.createElement ("Corpuses");
    QDomElement dateElem = dom.createElement ("Timing");
    QDomElement sentencesElem = dom.createElement ("Sentences");

    rootElem.setAttribute ("Uuid", uuid.toString());
    rootElem.setAttribute ("Version", 0.01);
    dateElem.setAttribute ("Created", QDateTime::currentDateTimeUtc().toString());

    rootElem.appendChild (dateElem);
    rootElem.appendChild (sentencesElem);
    dom.appendChild (rootElem);

    QFile* file = new QFile (getPath (uuid) + "/corpus.xml");

    if (!file->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "[Corpus::create()] Failed to create corpus XML: " << file->errorString();
        return 0;
    }

    QTextStream strm (file);
    dom.save (strm, 4);

    qDebug() << "[Corpus::create()] XML dump: " << dom.toString() << file->size();

    Corpus* corpus = Corpus::obtain (uuid);

    if (!corpus) {
        qDebug() << "[Corpus::create()] Failed to render Corpus." << file->errorString();
        return 0;
    }

    Q_FOREACH (QString str, p_text) {
        str = str.simplified().trimmed();

        if (str.isEmpty() || str.isNull())
            continue;

        Sentence* l_sent = corpus->addSentence (str.toUtf8() , 0);
        corpus->m_dom->documentElement().namedItem ("Sentences").appendChild (*l_sent->getElement());
        qDebug() << "[Corpus::create()] Added sentence" << corpus->sentences().count() << str;
    }

    corpus->save();
    qDebug() << "[Corpus::create()] Created corpus at " << file->fileName();
    return corpus;
}

bool Corpus::exists (const QUuid& p_uuid)
{
    return QFile::exists (getPath (p_uuid));
}

QString Corpus::getPath (const QUuid& p_uuid)
{
    const QString l_baseComp = Core::configurationPath().path() + "/corpus/";
    return l_baseComp + p_uuid.toString();
}

QString Corpus::audioPath() const
{
    return getPath (this->uuid()) + "/audio";
}

const QUuid Corpus::uuid() const
{
    return m_dom->documentElement().attribute ("Uuid");
}

Corpus* Corpus::obtain (const QUuid& p_uuid)
{
    Corpus* crps = 0;
    const QString path = getPath (p_uuid) + "/corpus.xml";
    qDebug() << "[Corpus::obtain()] Obtaining corpus" << p_uuid << path;

    if (!QFile::exists (path)) {
        qDebug() << "[Corpus::obtain()] Corpus not found at" << path;
        return 0;
    }

    crps = new Corpus (p_uuid);

    if (!crps->isValid()) {
        qDebug() << "[Corpus::obtain()] Invalid corpus" << p_uuid;
        return 0;
    }

    return crps;
}

void Corpus::load (const QUuid& p_uuid)
{
    QFile* file = new QFile (getPath (p_uuid) + "/corpus.xml");

    if (!file->exists()) {
        qDebug() << "[Corpus::load()] Corpus XML doesn't exist at" << file->fileName();
        nullify();
        return;
    }

    if (!file->open (QIODevice::ReadOnly)) {
        qDebug() << "[Corpus::load()] Corpus XML couldn't be read:" << file->errorString();
        nullify();
        return;
    }

    if (!m_dom) {
        m_dom = new QDomDocument ("Corpus");
    }

    QString msg;
    int line, col;

    if (!m_dom->setContent (file, &msg, &line, &col)) {
        qDebug() << "[Corpus::load()] Failed to load corpus." << msg << line << col;
        nullify();
        return;
    }

    QDomNodeList elems = m_dom->documentElement().firstChildElement ("Sentences").childNodes();

    if (elems.isEmpty()) {
        qDebug() << "[Corpus::load()] This corpus has no sentences.";
    }
    else {
        for (int i = 0; i < elems.count(); ++i) {
            QDomElement l_elem = elems.at (i).toElement();

            if (l_elem.isNull()) {
                continue;
            }

            Sentence* l_sntc = new Sentence (this, (new QDomElement (elems.at (i).toElement())));
            qDebug() << "[Corpus::load()] Loaded sentence:" << l_sntc->text();
            addSentence (l_sntc);
        }
    }

    m_uuid = p_uuid;
}

void Corpus::nullify()
{
    m_dom = 0;
    m_dict = 0;
    m_sntncLst = SentenceList();
    m_uuid = QUuid (QString::null);
    qDebug() << "[Corpus::nullify()] Nullified.";
}

/// @todo What happens if this corpus has no sentences?
/// @todo The dictionary isn't checked at the moment, but should be in the future when the class is more defined.
bool Corpus::isValid() const
{
    const bool l_valid = m_dom && !m_uuid.isNull();
    SC_ASSERT (l_valid == true, "Invalid Corpus.");
    qDebug() << "[Corpus::isValid()] Is corpus valid?" << l_valid;
    return l_valid;
}

void Corpus::save()
{
    QFile* file = new QFile (getPath (this->uuid()) + "/corpus.xml");

    if (file->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream l_strm (file);
        m_dom->save (l_strm, 4);
        qDebug() << "[Corpus::save()] Saved Corpus to" << file->fileName() << "with size" << file->size();
    }
    else {
        qDebug() << "[Corpus::save()] Can't write to" << file->fileName() << ":" << file->errorString();
    }
}

CorpusList Corpus::allCorpuses()
{
    CorpusList l_lst;
    QDir l_dir (Core::configurationPath().path() + "/corpus");
    l_dir.setFilter (QDir::Dirs);
    QStringList l_results = l_dir.entryList (QStringList() << "*");
    Q_FOREACH (const QString & l_uuid, l_results) {
        l_lst << Corpus::obtain (QUuid (l_uuid));
    }

    qDebug() << "[Corpus::allCorpuses()] Found " << l_lst.length() << "corpuses.";
    return l_lst;
}

void Corpus::erase()
{
    QDir* l_dir = new QDir (getPath (m_uuid));
    l_dir->rmdir (l_dir->absolutePath());
    qDebug() << "[Corpus::erase()] Erased Corpus" << m_uuid << "from" << l_dir->absolutePath();
}

Corpus* Corpus::clone() const
{
    QUuid l_uuid = QUuid::createUuid();
    QDir l_thisDir (Core::configurationPath().path() + "/corpus/" + m_uuid.toString());
    QDir l_newDir (Core::configurationPath().path() + "/corpus/" + l_uuid.toString());
    l_newDir.mkpath (l_newDir.absolutePath());
    QStringList l_lst = l_newDir.entryList ( (QStringList() << "*"), QDir::NoDotAndDotDot | QDir::Files, QDir::NoSort);

    Q_FOREACH (QString l_pth, l_lst) {
        QFile* l_file = new QFile (l_pth);
        const QString l_newPth = l_pth.replace (m_uuid.toString(), l_uuid.toString());
        l_file->copy (l_newPth);
    }

    return Corpus::obtain (l_uuid);
}

Sentence* Corpus::sentenceAt (const int& p_index) const
{
    if (p_index < 0 || p_index > m_sntncLst.count())
        return 0;
    else
        return m_sntncLst.at (p_index);
}

Dictionary* Corpus::dictionary() const
{
    return m_dict;
}

const QDateTime Corpus::timeStarted() const
{
    return QDateTime::fromString (m_dom->elementsByTagName ("Date").at (0).toElement().attribute ("Started"));
}

const QDateTime Corpus::timeLastModified() const
{
    return QDateTime::fromString (m_dom->elementsByTagName ("Date").at (0).toElement().attribute ("LastModified"));
}

const QDateTime Corpus::timeCompleted() const
{
    return QDateTime::fromString (m_dom->elementsByTagName ("Date").at (0).toElement().attribute ("Completed"));
}

/// @todo What to clean-up?
Corpus::~Corpus()
{

}

#include "corpus.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
