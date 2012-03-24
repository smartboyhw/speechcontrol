/***
 *  This file is part of SpeechControl.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <stdexcept>
#include <QDebug>
#include <QFile>

#include "app/core.hpp"
#include "sessions/phrase.hpp"
#include "sessions/corpus.hpp"
#include "sessions/content.hpp"
#include "sessions/session.hpp"

using namespace SpeechControl;

QMap<QString, QDomElement*> Session::s_elems;
QDomDocument* Session::s_dom = 0;
ContentMap Content::s_lst;

Session::Session (const QString& p_id) : m_corpus (0), m_content (0), m_elem (0)
{
    load (p_id);
}

Session::~Session()
{
}

Corpus* Session::corpus() const
{
    return m_corpus;
}

/// @todo Invoke the progress of this session when loaded.
void Session::setCorpus (Corpus* corpus)
{
    if (corpus == NULL)
        throw std::invalid_argument("Null corpus error.");

    m_corpus = corpus;
    assessProgress();

//         qDebug() << "[Session::setCorpus()] Null corpus not added.";
}

Content* Session::content() const
{
    return m_content;
}

void Session::setContent (Content* p_content)
{
    if (p_content == NULL)
        throw std::invalid_argument("Null content error.");

    m_content = p_content;
    assessProgress();
}

double Session::assessProgress() const
{
    double progress = 0.0;

    Q_FOREACH (const Phrase * phrase, corpus()->phrases()) {
        progress += (phrase->isCompleted()) ? 1.0 : 0.0;
    }

    double progressDelta = progress / (double) (corpus()->phrases().count());
    emit progressChanged (progressDelta);
    return progressDelta;
}

void Session::init()
{
    qDebug() << "[Session::init()] Loading sessions...";
    QFile* configFile = new QFile (Core::configurationPath().absolutePath() + "/sessions.xml");
    s_elems.clear();

    if (s_dom) {
        delete s_dom;
    }

    s_dom = new QDomDocument ("Sessions");

    if (configFile->exists()) {
        configFile->open (QIODevice::ReadOnly);
        s_dom->setContent (configFile);

        const QDomElement documentElem = s_dom->documentElement();

        if (documentElem.isNull()) {
            qDebug() << "[Session:init()] Core Session XML is null; resetting...";
            configFile->remove();
            init();
            qDebug() << "[Session:init()] Core Session XML has been reset.";
        }
        else {

            const QDomNodeList domList = documentElem.elementsByTagName ("Session");

            for (int i = 0; i < domList.count(); i++) {
                QDomElement node = domList.at (i).toElement();
                const QString id (node.attribute ("id"));
                s_elems.insert (id, new QDomElement (domList.at (i).toElement()));
            }

            qDebug() << "[Session::init()] " << domList.count() << "sessions loaded.";
        }
    }
    else {
        qDebug() << "[Session::init()] Creating session listing...";
        QDomElement elem = s_dom->createElement ("Sessions");

        if (s_dom->appendChild (elem).isNull()) {
            qFatal ("[Session::init()] The internal Session XML is corrupt.");
        }

        save();
        qDebug() << "[Session::init()] Created session listing.";
    }

    qDebug() << s_dom->toString (4);
    configFile->close();
}

void Session::load (const QString& p_id)
{
    m_elem = s_elems.value (p_id);

    if (m_elem && !m_elem->isNull()) {
        setCorpus (Corpus::obtain (m_elem->attribute ("corpus")));
        setContent (Content::obtain (m_elem->attribute ("content")));
    }
    else {
        s_elems.remove (p_id);
        m_content = 0;
        m_corpus = 0;
        m_elem = 0;
    }

    qDebug() << "[Session::load()] Is Session" << p_id << "valid?" << isValid();
}

bool Session::isValid() const
{
    const bool l_valid = (
                             m_elem && !m_elem->isNull()
                             && m_corpus && m_corpus->isValid()
                         );
    return l_valid;
}

SessionList Session::allSessions()
{
    SessionList lst;
    Q_FOREACH (const QString id, s_elems.keys()) {
        Session* session = Session::obtain (id);

        if (session && session->isValid())
            lst << session;
    }

    return lst;
}

SessionList Session::completedSessions()
{
    SessionList lst = allSessions();
    SessionList completedLst;

    Q_FOREACH (Session * session, lst) {
        if (session->isCompleted())
            completedLst << session;
    }

    return completedLst;
}

SessionList Session::incompleteSessions()
{
    SessionList lst = allSessions();
    SessionList incompletedLst;

    Q_FOREACH (Session * session, lst) {
        if (!session->isCompleted())
            incompletedLst << session;
    }

    return incompletedLst;

}

void Session::save()
{
    QFile* configFile = new QFile (Core::configurationPath().absolutePath() + "/sessions.xml");

    if (!configFile->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "[Session::save()] Unable to open session data for saving." << configFile->errorString();
        return;
    }

    configFile->write (s_dom->toByteArray (4));
    configFile->close();
}

QString Session::id() const
{
    return m_elem->attribute ("id");
}

Session* Session::obtain (const QString& p_id)
{
    qDebug() << "[Session::obtain()] Obtaining session" << p_id;
    return new Session (p_id);
}

/// @todo Create a new Corpus with this Session.
Session* Session::create (const Content* p_content)
{
    const QStringList lst = p_content->pages().join ("\n").simplified().trimmed().replace (".", ".\n").split ("\n", QString::SkipEmptyParts);
    qDebug() << "[Session::create()] Session has" << lst.length() << "sentences.";
    const QString id = QUuid::createUuid().toString().split("-")[0].replace("{","");
    Corpus* corpus = Corpus::create (lst);

    if (!corpus) {
        qDebug() << "[Session::create()] Failed to create corpus for session.";
        return 0;
    }

    // create element data
    QDomElement sessElem = s_dom->createElement ("Session");
    QDomElement dateElem = s_dom->createElement ("Date");

    dateElem.setAttribute ("created", QDateTime::currentDateTimeUtc().toString (Qt::SystemLocaleDate));
    dateElem.setAttribute ("completed", "-1");
    sessElem.setAttribute ("id", id);
    sessElem.setAttribute ("content", p_content->id());
    sessElem.setAttribute ("corpus", corpus->id());

    sessElem = s_dom->documentElement().appendChild (sessElem).toElement();
    dateElem = sessElem.appendChild (dateElem).toElement();

    if (dateElem.isNull() || sessElem.isNull()) {
        qDebug() << "[Session::create()] Error creating Session, invalid DOM used.";
        return 0;
    }

    save();
    init();
    return Session::obtain (id);
}

Session* Session::Backup::session()
{
    return 0;
}

Phrase* Session::firstIncompletePhrase() const
{
    const PhraseList lst = incompletedPhrases();

    if (!lst.isEmpty())
        return lst.first();

    return 0;
}

Phrase* Session::lastIncompletePhrase() const
{
    const PhraseList lst = incompletedPhrases();

    if (!lst.isEmpty())
        return lst.last();

    return 0;
}

PhraseList Session::incompletedPhrases() const
{
    PhraseList list;

    Q_FOREACH (Phrase * phrase, m_corpus->phrases()) {
        if (!phrase->isCompleted()) {
            list << phrase;
            qDebug() << "[Phrase::incompletedPhrases()] Incomplete: " << phrase->index() << phrase->text();
        }
        else
            continue;
    }

    qDebug() << "[Phrase::incompletedPhrases()] Number of incomplete sentences:" << list.length();

    if (list.length() > 0)
        qDebug() << "[Phrase::incompletedPhrases()] First up at: " << list.first()->index();
    else {
        m_elem->namedItem("Date").toElement().setAttribute("completed",QDateTime::currentDateTimeUtc().toString());
        qDebug() << "[Phrase::incompletedPhrases()] No more phrases detected, setting Session to 'completed' state.";
    }

    return list;
}

bool Session::isCompleted() const
{
    return incompletedPhrases().isEmpty() == true;
}

Session::BackupList* Session::backups() const
{
    return 0;
}

void Session::erase() const
{
    QString id (m_elem->attribute ("id"));
    s_elems.remove (id);
    m_corpus->erase();
    s_dom->documentElement().removeChild (*m_elem);

    save();
    init();

    qDebug() << "[Session::erase()] Session" << id << "removed.";
}

Session::Backup* Session::createBackup() const
{
    return Backup::generate (*this);
}

void Session::setName (const QString& p_name)
{
    if (!p_name.isEmpty() && !p_name.isEmpty())
        m_elem->setAttribute ("Name", p_name);
}

QString Session::name() const
{
    if (m_elem->hasAttribute ("Name"))
        return m_elem->attribute ("Name");
    else {
        if (content() && content()->isValid())
            return content()->title();
    }

    return QString::null;
}


Session* Session::clone() const
{
    const QString id = QUuid::createUuid().toString().split("-")[0].replace("{","");
    Corpus* corpus = m_corpus->clone();
    QDomElement elem = m_elem->cloneNode (true).toElement();
    elem.attribute ("id", id);
    elem.attribute ("corpus", corpus->id());
    elem.namedItem ("Date").toElement().setAttribute ("created", QDateTime::currentDateTimeUtc().toString (Qt::SystemLocaleDate));
    s_dom->documentElement().appendChild (elem);
    s_elems.insert (id, new QDomElement (elem));
    return Session::obtain (id);
}

Session::Backup::Backup() : m_dom (0)
{
}

Session::Backup::~Backup()
{

}

QDateTime Session::Backup::created()
{
    QString l_time = m_dom->documentElement().attribute ("DateCreated");
    return QDateTime::fromString (l_time);
}

const QString Session::Backup::getPath (const QString& p_id)
{
    return QDir::homePath() + "/.speechcontrol/backups/" + p_id + ".bckp";
}

/// @todo Implement a means of backing up a session's data to a compressed document.
Session::Backup* Session::Backup::generate (const Session& p_sssn)
{
    QDomDocument dom ("Backup");
    QDateTime tm = QDateTime::currentDateTimeUtc();
    QString id = p_sssn.id() + "_" + QString::number (tm.toMSecsSinceEpoch());
    QDomElement domElem = dom.appendChild (dom.createElement ("Backup")).toElement();
    QFile* file = new QFile (getPath (id));
    file->open (QIODevice::WriteOnly | QIODevice::Truncate);

    // Obtain session data.
    const QString sssnStr = p_sssn.m_elem->text();
    const QByteArray sssnData = qCompress (sssnStr.toUtf8());

    // Compress corpus data.
    const Corpus* corpus = p_sssn.corpus();
    QFile* corpusFile = new QFile (Corpus::getPath (corpus->id()));
    QByteArray corpusData;
    corpusData = qCompress (corpusFile->readAll());

    QDomElement sssnElem = dom.createElement ("Session");
    domElem.appendChild (sssnElem);
    sssnElem.appendChild (dom.createTextNode (sssnData.toBase64()));

    QDomElement crpsElem = dom.createElement ("Corpus");
    domElem.appendChild (crpsElem);
    crpsElem.appendChild (dom.createTextNode (corpusData.toBase64()));

    qDebug() << "[Session::Backup::generate()] "
             << domElem.text()
             << corpusData
             << sssnData;
    return 0;
}

#include "session.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
