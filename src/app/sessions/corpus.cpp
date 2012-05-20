/***
 *  This file is part of the SpeechControl project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/19/12 13:00:10 PM
 */

#include <stdexcept>

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDomDocument>

#include <lib/dictionary.hpp>

#include "app/core.hpp"
#include "app/global.hpp"
#include "app/sessions/corpus.hpp"
#include "app/sessions/phrase.hpp"

using namespace SpeechControl;

Corpus::Corpus (const QString& p_id) : QObject(), m_id (p_id),
    m_dom (new QDomDocument), m_dict (0), m_phraseList()
{
    load (p_id);
}

PhraseList Corpus::phrases() const
{
    return m_phraseList;
}

Phrase* Corpus::addPhrase (Phrase* p_phrase)
{
    m_phraseList << p_phrase;
    return p_phrase;
}

Phrase* Corpus::addPhrase (const QString& p_text, const QFile* p_audio)
{
    Phrase* phrase = Phrase::create (this, p_text);

    if (p_audio) {
        phrase->m_elem->attribute (p_audio->fileName());
    }

    return phrase;
}

Corpus& Corpus::operator << (Phrase* p_phrase)
{
    this->addPhrase (p_phrase);
    return *this;
}

/// @todo Just invoke the above method.
Corpus& Corpus::operator << (PhraseList& p_phraseList)
{
    Q_FOREACH (Phrase * phrs, p_phraseList) {
        this->addPhrase (phrs);
    }

    return *this;
}

/// @todo Build a dictionary from the said text.
/// @todo Find a way to keep the text in an ordinal fashion.
Corpus* Corpus::create (const QStringList& p_text)
{
    QString id = QString::number (qrand());
    QDir dir (getPath (id));

    if (!dir.mkpath (dir.path())) {
        qDebug() << "[Corpus::create()] Can't make corpus directory" << id << "at" << dir.path();
        return 0;
    }

    if (!dir.mkdir ("audio")) {
        qDebug() << "[Corpus::create()] Can't make corpus's at directory" << id << "at" << dir.path() + "/audio";
        return 0;
    }

    QDomDocument dom ("Corpus");
    QDomElement rootElem = dom.createElement ("Corpuses");
    QDomElement dateElem = dom.createElement ("Timing");
    QDomElement phrasesElem = dom.createElement ("Phrases");

    rootElem.setAttribute ("ID", id);
    rootElem.setAttribute ("Version", SPCHCNTRL_BUILD_VERSION);
    dateElem.setAttribute ("Created", QDateTime::currentDateTimeUtc().toString());

    rootElem.appendChild (dateElem);
    rootElem.appendChild (phrasesElem);
    dom.appendChild (rootElem);

    QFile* file = new QFile (getPath (id) + "/corpus.xml");

    if (!file->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "[Corpus::create()] Failed to create corpus XML: " << file->errorString();
        return 0;
    }

    QTextStream strm (file);
    dom.save (strm, 4);

    qDebug() << "[Corpus::create()] XML dump: " << dom.toString() << file->size();

    Corpus* corpus = Corpus::obtain (id);

    if (!corpus) {
        qDebug() << "[Corpus::create()] Failed to render Corpus." << file->errorString();
        return 0;
    }

    QStringList words = p_text.join (".").split (" ");
    QString phrase;
    uint wordCount = 0;

    Q_FOREACH (QString word, words) {
        const bool reachedWordLimit = (wordCount == CORPUS_WORD_BLOCK);
        const bool isAtEnd = (word == words.last());
        word = word.simplified().trimmed();

        if (isAtEnd || reachedWordLimit) {
            if (isAtEnd)
                phrase += " " + word;

            Phrase* phrs = corpus->addPhrase (phrase.trimmed().simplified(), 0);
            corpus->m_dom->documentElement().namedItem ("Phrases").appendChild (* (phrs->m_elem));
            qDebug() << "[Corpus::create()] Added phrase" << corpus->phrases().count() << phrase;
            wordCount = 0;
            phrase = word.simplified().trimmed();
        }
        else {
            if (word.isEmpty() || word.isNull()) {
                continue;
            }
            else {
                phrase += " " + word;
            }
        }

        wordCount++;
    }

    qDebug() << "[Corpus::create()] Rendered" << words.count() << "into " << corpus->phrases().count() << "phrases.";

    // Create dictionary.
    corpus->m_dict = Dictionary::create (words, id);

    corpus->save();
    qDebug() << "[Corpus::create()] Created corpus at " << file->fileName();
    return corpus;
}

bool Corpus::exists (const QString& p_id)
{
    return QFile::exists (getPath (p_id));
}

QString Corpus::getPath (const QString& p_id)
{
    const QString l_baseComp = Core::configurationPath().path() + "/corpus/";
    return l_baseComp + p_id;
}

QString Corpus::audioPath() const
{
    return getPath (this->id()) + "/audio";
}

const QString Corpus::id() const
{
    return m_dom->documentElement().attribute ("ID");
}

Corpus* Corpus::obtain (const QString& p_id)
{
    Corpus* crps = 0;
    const QString path = getPath (p_id) + "/corpus.xml";
    qDebug() << "[Corpus::obtain()] Obtaining corpus" << p_id << path;

    if (!QFile::exists (path)) {
//         qDebug() << "[Corpus::obtain()] Corpus not found at" << path;
        throw std::runtime_error ("Corpus not found at " + path.toStdString());
    }

    crps = new Corpus (p_id);

    if (!crps->isValid()) {
//         qDebug() << "[Corpus::obtain()] Invalid corpus" << p_id;
        throw std::runtime_error ("Invalid corpus.");
    }

    return crps;
}

void Corpus::load (const QString& p_id)
{
    QFile* file = new QFile (getPath (p_id) + "/corpus.xml");

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

    QDomNodeList elems = m_dom->documentElement().firstChildElement ("Phrases").childNodes();

    if (elems.isEmpty()) {
        qDebug() << "[Corpus::load()] This corpus has no phrases.";
    }
    else {
        for (int i = 0; i < elems.count(); ++i) {
            QDomElement l_elem = elems.at (i).toElement();

            if (l_elem.isNull()) {
                continue;
            }

            Phrase* l_sntc = new Phrase (this, (new QDomElement (elems.at (i).toElement())));
            //qDebug() << "[Corpus::load()] Loaded phrase:" << l_sntc->text();
            addPhrase (l_sntc);
        }
    }

    m_id = p_id;
    QDir path (getPath (id()));
    path.mkdir ("audio");

    m_dict = Dictionary::obtain (id());
}

void Corpus::nullify()
{
    m_dom = 0;
    m_dict = 0;
    m_phraseList = PhraseList();
    m_id = QString (QString::null);
    qDebug() << "[Corpus::nullify()] Nullified.";
}

/// @todo What happens if this corpus has no phrases?
/// @todo The dictionary isn't checked at the moment, but should be in the future when the class is more defined.
bool Corpus::isValid() const
{
    const bool valid = m_dom && !m_id.isNull();
    SC_ASSERT (valid == true, "Invalid Corpus.");
    qDebug() << "[Corpus::isValid()] Is corpus valid?" << valid;
    return valid;
}

void Corpus::save()
{
    QFile* file = new QFile (getPath (this->id()) + "/corpus.xml");

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
    CorpusList lst;
    QDir dir (Core::configurationPath().path() + "/corpus");
    dir.setFilter (QDir::Dirs);
    QStringList results = dir.entryList (QStringList() << "*");
    Q_FOREACH (const QString & id, results) {
        lst << Corpus::obtain (QString (id));
    }

    qDebug() << "[Corpus::allCorpuses()] Found " << lst.length() << "corpuses.";
    return lst;
}

void Corpus::erase()
{
    QDir* dir = new QDir (getPath (m_id));
    dir->rmdir (dir->absolutePath());
    qDebug() << "[Corpus::erase()] Erased Corpus" << m_id << "from" << dir->absolutePath();
}

Corpus* Corpus::clone() const
{
    QString id = QString::number (qrand());
    QDir thisDir (getPath (m_id));
    QDir newDir (getPath (id));
    newDir.mkpath (newDir.absolutePath());
    QStringList lst = newDir.entryList ( (QStringList() << "*"), QDir::NoDotAndDotDot | QDir::Files, QDir::NoSort);

    Q_FOREACH (QString pth, lst) {
        QFile* file = new QFile (pth);
        const QString newPth = pth.replace (m_id, id);
        file->copy (newPth);
    }

    return Corpus::obtain (id);
}

Phrase* Corpus::phraseAt (const int& p_index) const
{
    if (p_index < 0 || p_index > m_phraseList.count())
        return 0;
    else
        return m_phraseList.at (p_index);
}

Dictionary* Corpus::dictionary() const
{
    if (isValid()) {
        return m_dict;
    }

    return 0;
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

QFile* Corpus::fileIds() const
{
    if (isValid()) {
        QFile* fileIds = new QFile (getPath (m_id) + "/fileids");

        if (!fileIds->exists()) {
            fileIds->open (QIODevice::WriteOnly | QIODevice::Truncate);

            if (!fileIds->isOpen() || !fileIds->isWritable()) {
                qDebug() << "[Corpus::fileIds()] Can't open fileids for writing:" << fileIds->errorString();
                return 0;
            }

            QTextStream strm (fileIds);

            Q_FOREACH (const Phrase * phrase, phrases()) {
                QFileInfo currentFile (phrase->audioFile()->fileName());
                const QString fileid = currentFile.baseName();
                strm << fileid << endl;
            }

            fileIds->close();
        }

        return fileIds;
    }

    return 0;
}

QFile* Corpus::transcription (QString const& p_silencePrefix,
                              QString const& p_silenceSuffix) const
{
    if (isValid()) {
        QFile* transcription = new QFile (getPath (m_id) + "/transcription");

        if (!transcription->exists()) {
            transcription->open (QIODevice::WriteOnly | QIODevice::Truncate);

            if (!transcription->isOpen() || !transcription->isWritable()) {
                qDebug() << "[Corpus::transcription()] Can't open transcription for writing:" << transcription->errorString();
                return 0;
            }

            QTextStream strm (transcription);

            Q_FOREACH (const Phrase * phrase, phrases()) {
                QFileInfo currentFile (phrase->audioFile()->fileName());
                const QString fileid = currentFile.baseName();
                const QString phraseText = phrase->text().toUpper();
                strm << p_silencePrefix << " "
                     << phraseText << " "
                     << p_silenceSuffix << " "
                     << "(" << fileid << ")" << endl;
            }

            transcription->close();
        }

        return transcription;
    }

    return 0;
}

/// @todo What to clean-up?
Corpus::~Corpus()
{

}

#include "sessions/corpus.moc"
// kate: indent-mode cstyle; replace-tabs on; 
