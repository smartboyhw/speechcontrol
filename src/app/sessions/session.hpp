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

// speechcontrol includes
#include <app/sessions/corpus.hpp>
#include <app/sessions/content.hpp>

namespace SpeechControl
{
class Session;

/**
 * @brief Represents a @c QList of @c Session objects.
 **/
typedef QList<Session*> SessionList;

/**
 * @brief Represents a @c QMap of @c Session objects, mapped by their @c QUuid keys.
 **/
typedef QMap<QUuid, Session*> SessionMap;

/**
 * @brief Represents data to be used for adaption.
 **/
class Session : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Session)

public:
    /**
     * @brief Backup class
     **/
    class Backup
    {
        friend class Session;

    public:
        virtual ~Backup();
        Session* session();
        QDateTime created();

    private:
        explicit Backup();

        static const QString getPath (const QString&);
        static Backup* generate (const Session&);
        QDomDocument* m_dom;
    };

    /**
     * @brief List of pointers to Backup objects.
     **/
    typedef QList<Backup*> BackupList;

    /**
     * @brief New Session with UUID
     *
     * @param  QUuid UUID of the Session
     **/
    explicit Session (const QUuid&);

    /**
     * @brief Destructor
     **/
    virtual ~Session();

    /**
     * @brief UUID of the Session
     *
     * @return UUID of the Session
     **/
    QUuid uuid() const;

    /**
     * @brief ...
     *
     * @return Name of the Session
     **/
    QString name() const;

    /**
     * @brief Check if the Session is completed.
     *
     * @return Boolean of completion
     **/
    bool isCompleted() const;

    /**
     * @brief Check if the Session is valid.
     *
     * @return Boolean of validity
     **/
    bool isValid() const;

    void erase() const;

    /**
     * @brief Sets the name of this @c Session.
     * @param  p_name The new name of the @c Session.
     **/
    void setName (const QString& p_name);

    Session* clone() const;
    Backup* createBackup() const;
    BackupList* backups() const;
    Corpus* corpus() const;
    Content* content() const;
    Phrase* firstIncompletePhrase() const;
    Phrase* lastIncompletePhrase() const;
    PhraseList incompletedPhrases() const;
    static void init();
    static Session* obtain (const QUuid& p_uuid);
    static Session* create (const Content*);
    static void save();
    static SessionList allSessions();

signals:
    /**
     * @brief Signal emitted when the Session progress changes.
     *
     * @param  progress Amount of progress changed
     **/
    void progressChanged (const double& p_progress) const;

public slots:
    /**
     * @brief Set Corpus for this Session.
     *
     * @param  p_corpus Corpus for this Session
     **/
    void setCorpus (Corpus* p_corpus);
    void setContent (Content* p_content);
    void load (const QUuid& p_uuid);
    double assessProgress() const;

private:
    static QDomDocument* s_dom;
    static QMap<QUuid, QDomElement*> s_elems;
    Corpus* m_corpus;
    Content* m_content;
    QDomElement* m_elem;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
