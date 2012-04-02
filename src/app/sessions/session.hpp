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

#ifndef SESSION_HPP
#define SESSION_HPP

#include <QMap>
#include <QList>
#include <QObject>
#include <QDateTime>
#include <QDomDocument>

namespace SpeechControl
{
class Phrase;
class Corpus;
class Content;
class Session;

typedef QList<Phrase*> PhraseList;

/**
 * @brief Represents a @c QList of @c Session objects.
 **/
typedef QList<Session*> SessionList;

/**
 * @brief Represents a @c QMap of @c Session objects, mapped by their @c QString keys.
 **/
typedef QMap<QString, Session*> SessionMap;

/**
 * @brief Represents data to be used for adaption.
 *
 * Session objects provide a simple means of representing speech corpora, content
 * for transcription and other bits of data needed for the adaption or training
 * process in one place.
 **/
class Session : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Session)

public:
    /**
     * @brief Constructor.
     *
     * Loads a session from disk from it's specific Session.
     *
     * @param  QString The ID of the Session.
     **/
    explicit Session (const QString& p_id);

    /**
     * @brief Destructor
     **/
    virtual ~Session();

    /**
     * @brief Obtains the ID of the Session.
     **/
    QString id() const;

    /**
     * @brief Obtains the nickname assigned to the Session.
     **/
    QString name() const;

    /**
     * @brief Determines if the Session has been completed.
     **/
    bool isCompleted() const;

    /**
     * @brief Determines if the Session's valid.
     **/
    bool isValid() const;

    /**
     * @brief Erases the Session and its data from disk.
     **/
    void erase() const;

    /**
     * @brief Sets the name of this @c Session.
     * @param p_name The new name of the @c Session.
     **/
    void setName (const QString& p_name);

    /**
     * @brief Clones this entire Session's data.
     *
     * @return A pointer to the cloned Session.
     **/
    Session* clone() const;

    /**
     * @brief Obtains the Corpus used by this Session.
     **/
    Corpus* corpus() const;

    /**
     * @brief Obtains the Content used by this Session.
     **/
    Content* content() const;

    /**
     * @brief Obtains the first uncompleted phrase.
     *
     * @return A pointer to a phrase if uncompletedPhrases().empty() != false, else NULL.
     **/
    Phrase* firstIncompletePhrase() const;

    /**
     * @brief Obtains the last uncompleted phrase.
     *
     * @return A pointer to a phrase if uncompletedPhrases().empty() != false, else NULL.
     **/
    Phrase* lastIncompletePhrase() const;

    /**
     * @brief Obtains a list of uncompleted phrases.
     *
     * @return A PhraseList.
     **/
    PhraseList uncompletedPhrases() const;

    /**
     * @brief Obtains the date and time this Session was created.
     **/
    QDateTime dateCompleted() const;

    /**
     * @brief Obtains the date and time this Session was completed.
     **/
    QDateTime dateCreated() const;

    /**
     * @brief Initializes the Session repositories.
     *
     * Loads data about all of the Sessions and builds the internal cache of
     * the Session listing.
     *
     * @note It's important to invoke this method at least ONCE when using Sessions.
     **/
    static void init();

    /**
     * @brief Saves information about the Session's cache to disc.
     **/
    static void save();

    /**
     * @brief Obtains a Session by its specific ID.
     *
     * @param p_id The ID in question.
     * @return A pointer to a Session object if it exists, NULL otherwise.
     **/
    static Session* obtain (const QString& p_id);

    /**
     * @brief Creates a new Session.
     *
     * Creates a new Session by using the text provided in its Content, p_content.
     *
     * @param p_content The Content to use for this Session's text.
     * @return A pointer to a Session object if the process succeeded, NULL otherwise.
     **/
    static Session* create (const Content* p_content);

    /**
     * @brief Obtains a list of all of the Sessions.
     **/
    static SessionList allSessions();

    /**
     * @brief Obtains a SessionList of all of the completed Sessions.
     **/
    static SessionList completedSessions();

    /**
     * @brief Obtains a SessionList of all of the incomplete Sessions.
     **/
    static SessionList incompleteSessions();

signals:
    /**
     * @brief Signal emitted when the Session progress changes.
     *
     * @param  progress Amount of progress changed
     **/
    void progressChanged (const double& p_progress) const;

public slots:
    /**
     * @brief Sets the Corpus for this Session.
     *
     * @param  p_corpus Corpus for this Session
     **/
    void setCorpus (Corpus* p_corpus);

    /**
     * @brief Sets the Content for this Session.
     *
     * @param p_content The Content to represent this session.
     **/
    void setContent (Content* p_content);

    /**
     * @brief Loads a Session by its specified ID, p_id.
     *
     * @param p_id The ID to load the Session with.
     **/
    void load (const QString& p_id);

    /**
     * @brief Assesses the progress completed by this Session.
     *
     * @return The progress completed of this Session.
     **/
    double assessProgress() const;

private:
    static QDomDocument* s_dom;                   ///< The DOM cache.
    static QMap<QString, QDomElement*> s_elems;   ///< The listing of already initialized Session object.
    Corpus* m_corpus;                             ///< Pointer to the Corpus data.
    Content* m_content;                           ///< The Content element of this Session.
    QDomElement* m_elem;                          ///< The accompanying XML data of the Session.
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
