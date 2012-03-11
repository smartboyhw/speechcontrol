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

#include "session.hpp"
#include <core.hpp>

#include <QDebug>
#include <QFile>

using namespace SpeechControl;

QMap<QUuid, QDomElement*> Session::s_elems;
QDomDocument* Session::s_dom = 0;
ContentMap Content::s_lst;

Session::Session (const QUuid& p_uuid) : m_corpus (0), m_content (0), m_elem (0)
{
    load (p_uuid);
}

Session::~Session()
{
}

Corpus* Session::corpus() const
{
    return m_corpus;
}

/// @todo Invoke the progress of this session when loaded.
void Session::setCorpus (Corpus* l_corpus)
{
    if (l_corpus) {
        m_corpus = l_corpus;
        assessProgress();
    } else {
        qDebug() << "Null corpus not added.";
    }
}

Content* Session::content() const
{
    return m_content;
}

void Session::setContent (Content* p_content)
{
    m_content = p_content;
    assessProgress();
}

double Session::assessProgress() const
{
    double l_progress = 0.0;

    Q_FOREACH (const Sentence * l_snt, corpus()->sentences()) {
        l_progress += l_snt->completedProgress();
    }

    double progressDelta = l_progress / (double) (corpus()->sentences().count());
    emit progressChanged (progressDelta);
    return progressDelta;
}

void Session::init()
{
    qDebug() << "Loading sessions...";
    QFile* configFile = new QFile ( Core::configurationPath().absolutePath() +"/sessions.xml" );
    s_elems.clear();

    if (s_dom) {
        s_dom->clear();
        delete s_dom;
    }

    s_dom = new QDomDocument ("Sessions");

    if (configFile->exists()) {
        configFile->open (QIODevice::ReadOnly);
        s_dom->setContent (configFile);

        const QDomElement documentElem = s_dom->documentElement();
        const QDomNodeList l_domList = documentElem.elementsByTagName ("Session");

        for (int i = 0; i < l_domList.count(); i++) {
            QDomElement l_node = l_domList.at(i).toElement();
            const QUuid l_uuid (l_node.attribute ("uuid"));
            s_elems.insert (l_uuid, new QDomElement (l_domList.at (i).toElement()));
        }

        qDebug() << l_domList.count() << "sessions loaded.";
    } else {
        configFile->open (QIODevice::WriteOnly | QIODevice::Truncate);
        QDomElement l_elem = s_dom->createElement ("Sessions");
        s_dom->appendChild (l_elem);
        configFile->write (s_dom->toString (4).toLocal8Bit());
        qDebug() << "Created session listing.";
    }

    configFile->close();
}

void Session::load (const QUuid& p_uuid)
{
    m_elem = s_elems.value (p_uuid);

    if (m_elem && !m_elem->isNull()) {
        setCorpus (Corpus::obtain (m_elem->attribute ("corpus")));
        setContent (Content::obtain (m_elem->attribute ("content")));
    } else {
        s_elems.remove (p_uuid);
        m_content = 0;
        m_corpus = 0;
        m_elem = 0;
    }

    qDebug() << "Is Session" << p_uuid << "valid?" << isValid();
}

bool Session::isValid() const
{
    const bool l_valid = (
                             m_elem && !m_elem->isNull()
                             && m_corpus && m_corpus->isValid()
                             && m_content && m_content->isValid()
                         );
    //Q_ASSERT ( l_valid == true );
    return l_valid;
}

SessionList Session::allSessions()
{
    SessionList l_lst;
    Q_FOREACH (const QUuid l_uuid, s_elems.keys()) {
        Session* l_session = Session::obtain (l_uuid);

        if (l_session && l_session->isValid())
            l_lst << l_session;
    }

    return l_lst;
}

QUuid Session::uuid() const
{
    return m_elem->attribute ("uuid");
}

Session* Session::obtain (const QUuid& p_uuid)
{
    //qDebug() << "Obtaining session" << p_uuid;
    return new Session (p_uuid);
}

/// @todo Create a new Corpus with this Session.
Session* Session::create (const Content* p_content)
{
    const QStringList l_lst = p_content->pages().join ("\n").simplified().trimmed().replace (".", ".\n").split ("\n", QString::SkipEmptyParts);
    qDebug () << "Session has " << l_lst.length() << "potential sentences.";
    const QUuid l_uuid = QUuid::createUuid();
    Corpus* l_corpus = Corpus::create (l_lst);

    if (!l_corpus) {
        qDebug() << "Failed to create corpus for session.";
        return 0;
    }

    // create element data
    QDomElement l_sessElem = s_dom->createElement ("Session");
    QDomElement l_dateElem = s_dom->createElement ("Date");

    l_dateElem.setAttribute ("created", QDateTime::currentDateTimeUtc().toString (Qt::SystemLocaleDate));
    l_dateElem.setAttribute ("completed", "-1");
    l_sessElem.setAttribute ("uuid", l_uuid.toString());
    l_sessElem.setAttribute ("content", p_content->uuid().toString());
    l_sessElem.setAttribute ("corpus", l_corpus->uuid());

    s_dom->documentElement().appendChild (l_sessElem);
    s_dom->documentElement().appendChild (l_dateElem);

    QFile* l_file = new QFile ( Core::configurationPath().path() + "/sessions.xml" );
    l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_str (l_file);
    s_dom->save (l_str, 4);
    l_file->close();

    init();

    return Session::obtain (l_uuid);
}

Session* Session::Backup::session()
{
    return 0;
}

Sentence* Session::firstIncompleteSentence() const
{
    const SentenceList l_lst = m_corpus->sentences();

    for (int i = 0; i < l_lst.count(); i++) {
        Sentence* l_sent = l_lst.at (i);

        if (!l_sent->allPhrasesCompleted())
            return l_sent;
        else {
            qDebug() << l_sent->text() << "already completed @" << l_sent->audioPath().absolutePath();
            continue;
        }
    }

    return 0;
}

Sentence* Session::lastIncompleteSentence() const
{
    const SentenceList l_lst = m_corpus->sentences();
    SentenceList::ConstIterator l_endItr = l_lst.begin();

    for (SentenceList::ConstIterator l_itr = l_lst.end(); l_itr != l_endItr; l_itr--) {
        const Sentence* l_sent = (*l_itr);

        if (!l_sent->allPhrasesCompleted())
            return *l_itr;
        else {
            continue;
        }
    }

    return 0;
}

SentenceList Session::incompletedSentences() const
{
    SentenceList l_baseLst = m_corpus->sentences();
    SentenceList l_lst;

    for (SentenceList::Iterator l_itr = l_baseLst.begin(); l_itr != l_baseLst.end(); l_itr++) {
        Sentence* l_sent = (*l_itr);

        if (!l_sent->allPhrasesCompleted())
            l_lst << l_sent;

        continue;
    }

    //qDebug() << l_lst;

    return l_lst;
}

bool Session::isCompleted() const
{
    return (incompletedSentences().empty());
}

Session::BackupList* Session::backups() const
{
    return 0;
}

void Session::erase() const
{
    QUuid l_uuid (m_elem->attribute ("uuid"));
    s_elems.remove (l_uuid);
    m_corpus->erase();
    s_dom->documentElement().removeChild (*m_elem);

    QFile* l_file = new QFile (QDir::homePath() + "/.speechcontrol/sessions.xml");
    l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_strm (l_file);

    s_dom->save (l_strm, 4);

    qDebug() << "Session" << l_uuid << "removed.";
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

    return content()->title();
}


Session* Session::clone() const
{
    QUuid l_uuid = QUuid::createUuid();
    Corpus* l_corpus = m_corpus->clone();
    QDomElement l_elem = m_elem->cloneNode (true).toElement();
    l_elem.attribute ("uuid", l_uuid.toString());
    l_elem.attribute ("corpus", l_corpus->uuid());
    l_elem.namedItem ("Date").toElement().setAttribute ("created", QDateTime::currentDateTimeUtc().toString (Qt::SystemLocaleDate));
    s_dom->documentElement().appendChild (l_elem);
    s_elems.insert (l_uuid, &l_elem);
    return Session::obtain (l_uuid);
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
    QDomDocument l_dom ("Backup");
    QDateTime l_tm = QDateTime::currentDateTimeUtc();
    QString l_id = p_sssn.uuid().toString() + "_" + QString::number (l_tm.toMSecsSinceEpoch());
    QDomElement l_domElem = l_dom.appendChild (l_dom.createElement ("Backup")).toElement();
    QFile* l_file = new QFile (getPath (l_id));
    l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);

    // Obtain session data.
    const QString l_sssnStr = p_sssn.m_elem->text();
    const QByteArray l_sssnData = qCompress (l_sssnStr.toUtf8());

    // Compress corpus data.
    const Corpus* l_corpus = p_sssn.corpus();
    QFile* l_corpusFile = new QFile (Corpus::getPath (l_corpus->uuid()).toLocalFile());
    QByteArray l_corpusData;
    l_corpusData = qCompress (l_corpusFile->readAll());

    QDomElement l_sssnElem = l_dom.createElement ("Session");
    l_domElem.appendChild (l_sssnElem);
    l_sssnElem.appendChild (l_dom.createTextNode (l_sssnData.toBase64()));

    QDomElement l_crpsElem = l_dom.createElement ("Corpus");
    l_domElem.appendChild (l_crpsElem);
    l_crpsElem.appendChild (l_dom.createTextNode (l_corpusData.toBase64()));

    qDebug() << l_domElem.text()
             << l_corpusData
             << l_sssnData;
    return 0;
}

#include "session.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
