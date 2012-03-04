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

// libspchcntrl includes
#include <lib/corpus.hpp>

// speechcontrol includes
#include <app/sessions/content.hpp>

namespace SpeechControl {
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
 * @brief ...
 **/
class Session : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Session )

public:
    /**
     * @brief ...
     **/
    class Backup {
        friend class Session;
    public:
        virtual ~Backup();
        /**
         * @brief ...
         *
         * @return :Session*
         **/
        Session* session();
        /**
         * @brief ...
         *
         * @return QDateTime
         **/
        QDateTime created();

    private:
        static const QString getPath ( const QString& );
        static Backup* generate ( const Session& );
        explicit Backup();
        QDomDocument* m_dom;
    };

    /**
     * @brief ...
     **/
    typedef QList<Backup*> BackupList;

    /**
     * @brief ...
     *
     * @param  ...
     **/
    explicit Session ( const QUuid& );
    virtual ~Session();
    /**
     * @brief ...
     *
     * @return const QUuid
     **/
    const QUuid uuid() const;


    const QString name() const;
    /**
     * @brief ...
     *
     * @return const bool
     **/
    bool isCompleted() const;
    /**
     * @brief ...
     *
     * @return const bool
     **/
    bool isValid() const;
    /**
     * @brief ...
     *
     * @return void
     **/
    void erase() const;

    /**
     * @brief Sets the name of this @c Session.
     * @param  p_name The new name of the @c Session.
     * @return void
     **/
    void setName ( const QString& p_name );

    /**
     * @brief ...
     *
     * @return :Session*
     **/
    Session* clone() const;
    /**
     * @brief ...
     *
     * @return :Session::Backup*
     **/
    Backup* createBackup() const;
    /**
     * @brief ...
     *
     * @return :Session::BackupList*
     **/
    BackupList* backups() const;
    /**
     * @brief ...
     *
     * @return Corpus*
     **/
    Corpus* corpus() const;
    /**
     * @brief ...
     *
     * @return Content*
     **/
    Content* content() const;
    /**
     * @brief ...
     *
     * @return :Sentence*
     **/
    Sentence* firstIncompleteSentence() const;
    /**
     * @brief ...
     *
     * @return :Sentence*
     **/
    Sentence* lastIncompleteSentence() const;
    /**
     * @brief ...
     *
     * @return :SentenceList
     **/
    SentenceList incompletedSentences() const;

    /**
     * @brief ...
     *
     * @return void
     **/
    static void init();
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Session*
     **/
    static Session* obtain ( const QUuid& p_uuid );
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Session*
     **/
    static Session* create ( const Content* );
    /**
     * @brief ...
     *
     * @return :SessionList
     **/
    static SessionList allSessions();

signals:
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void progressChanged ( const double& );

public slots:
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void setCorpus ( Corpus* p_corpus );

    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void setContent ( Content* p_content );

    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void load ( const QUuid& p_uuid );

    /**
     * @brief ...
     *
     * @return void
     **/
    void assessProgress();

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
