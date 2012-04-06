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

#include <QFile>
#include <QDebug>

#include "core.hpp"
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
    if (corpus == 0)
        throw std::invalid_argument ("Null corpus error.");

    m_corpus = corpus;
    assessProgress();
}

Content* Session::content() const
{
    return m_content;
}

void Session::setContent (Content* p_content)
{
    if (p_content == 0)
        throw std::invalid_argument ("Null content error.");

    m_content = p_content;
    assessProgress();
}

double Session::assessProgress() const
{
    double progress = 0.0;

    if (isValid()) {
        Q_FOREACH (const Phrase * phrase, corpus()->phrases()) {
            progress += (phrase->isCompleted()) ? 1.0 : 0.0;
        }

        double progressDelta = progress / (double) (corpus()->phrases().count());
        emit progressChanged (progressDelta);
        return progressDelta;
    }
    else {
        return -1.0;
    }
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
                             m_elem && m_corpus
                             && !m_elem->isNull() && m_corpus->isValid()
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
    if (!p_id.isEmpty() && !p_id.isNull()) {
        qDebug() << "[Session::obtain()] Obtaining session" << p_id;
        return new Session (p_id);
    }

    return 0;
}

Session* Session::create (const Content* p_content)
{
    const QStringList lst = p_content->pages().join ("\n").simplified().trimmed().replace (".", ".\n").split ("\n", QString::SkipEmptyParts);
    qDebug() << "[Session::create()] Session has" << lst.length() << "sentences.";
    const QString id = QString::number (qrand());
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
    sessElem.setAttribute ("name", p_content->title());
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

Phrase* Session::firstIncompletePhrase() const
{
    const PhraseList lst = uncompletedPhrases();

    if (!lst.isEmpty())
        return lst.first();

    return 0;
}

Phrase* Session::lastIncompletePhrase() const
{
    const PhraseList lst = uncompletedPhrases();

    if (!lst.isEmpty())
        return lst.last();

    return 0;
}

PhraseList Session::uncompletedPhrases() const
{
    PhraseList list;

    if (isValid()) {
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
            if (! (dateCompleted().isValid() && !dateCompleted().isNull())) {
                m_elem->namedItem ("Date").toElement().setAttribute ("completed", QDateTime::currentDateTimeUtc().toString());
                qDebug() << "[Phrase::incompletedPhrases()] No more phrases detected, setting Session to 'completed' state.";
            }
            else {
                qDebug() << "[Phrase::incompletedPhrases()] Completed on " << dateCompleted().toLocalTime();
            }
        }
    }

    return list;
}

QDateTime Session::dateCompleted() const
{
    return QDateTime::fromString (m_elem->namedItem ("Date").toElement().attribute ("completed"));
}

QDateTime Session::dateCreated() const
{
    return QDateTime::fromString (m_elem->namedItem ("Date").toElement().attribute ("created"));
}

bool Session::isCompleted() const
{
    return uncompletedPhrases().isEmpty() == true;
}

void Session::erase() const
{
    if (isValid()) {
        QString id (m_elem->attribute ("id"));
        s_elems.remove (id);
        m_corpus->erase();
        s_dom->documentElement().removeChild (*m_elem);

        save();
        init();

        qDebug() << "[Session::erase()] Session" << id << "removed.";
    }
}

void Session::setName (const QString& p_name)
{
    if (isValid()) {
        if (!p_name.isEmpty() && !p_name.isEmpty()) {
            m_elem->setAttribute ("name", p_name);
            save();
        }
    }
}

QString Session::name() const
{
    if (isValid()) {
        if (m_elem->hasAttribute ("name"))
            return m_elem->attribute ("name");
        else {
            if (content() && content()->isValid())
                return content()->title();
        }
    }

    return QString::null;
}


Session* Session::clone() const
{
    if (isValid()) {
        const QString id = QString::number (qrand());
        Corpus* corpus = m_corpus->clone();
        QDomElement elem = m_elem->cloneNode (true).toElement();
        elem.attribute ("id", id);
        elem.attribute ("corpus", corpus->id());
        elem.namedItem ("Date").toElement().setAttribute ("created", QDateTime::currentDateTimeUtc().toString (Qt::SystemLocaleDate));
        s_dom->documentElement().appendChild (elem);
        s_elems.insert (id, new QDomElement (elem));
        return Session::obtain (id);
    }
    else {
        return 0;
    }
}

#include "sessions/session.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
