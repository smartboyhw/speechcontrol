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
#ifndef SESSION_HPP
#define SESSION_HPP

#include <QMap>
#include <QUuid>
#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QtXml/QDomDocument>

namespace SpeechControl {
class Corpus;
class Content;
class Sentence;
class Session;

/**
 * @brief Represents a @c QList of @c Session objects.
 **/
typedef QList<Session*> SessionList;

/**
 * @brief Represents a @c QList of @c Content objects.
 **/
typedef QList<Content*> ContentList;

/**
 * @brief Represents a @c QList of @c Sentence objects.
 **/
typedef QList<Sentence*> SentenceList;

/**
 * @brief Represents a @c QMap of @c Content objects, mapped by their @c QUuid keys.
 **/
typedef QMap<QUuid, Content*> ContentMap;

/**
 * @brief Represents a @c QMap of @c Session objects, mapped by their @c QUuid keys.
 **/
typedef QMap<QUuid, Session*> SessionMap;

/**
 * @brief Represents a collection of text to be used for training by @c Sessions.
 *
 * Contents are the pure-text representation of @c Corpus objects; @c Content objects
 * goes out to provide more specific data about the text being trained.
 *
 **/
class Content : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Content )

public:
    /**
     * @brief Default constructor.
     * @param p_uuid The uuid of the @c Content.
     **/
    explicit Content ( const QUuid& p_uuid );

    virtual ~Content();

    /**
     * @brief Loads a @c Content object by a specified UUID.
     * @param p_uuid The uuid of the Content to load.
     * @note After loading, you should check to see if isValid() returns true. It's possible for the loading operation to fail.
     **/
    void load ( const QUuid& p_uuid );

    /**
     * @brief Erases the Content, wiping all of its information.
     **/
    void erase();

    /**
     * @brief Counts the pages within this @c Content and returns that number.
     * @return A unsigned integer representing the number of pages in this @c Content.
     **/
    uint pageCount() const;

    /**
     * @brief Counts the words within this @c Content and returns that number.
     * @return A unsigned integer representing the number of words in the text.
     **/
    uint words() const;

    /**
     * @brief Counts the characters within this @c Content and returns that number.
     * @return A unsigned integer representing the total number of characters in the text.
     **/
    uint length() const;

    /**
     * @brief Counts the alphanumber characters within this @c Content and returns that number.
     * @return A unsigned integer representing the total number of alphanumber characters in the text.
     **/
    uint characters() const;

    /**
     * @brief Obtains the UUID of this @c Content.
     * @return The UUID of this @c Content.
     **/
    const QUuid uuid() const;

    /**
     * @brief Obtains the title of this @c Content.
     * @return A string holding the title of this @c Content.
     **/
    const QString title() const;

    /**
     * @brief Obtains the author of this @c Content.
     * @return A string holding the name of the author of this @c Content.
     **/
    const QString author() const;

    /**
     * @brief Obtains a list representing all of the pages contained by this @c Content.
     * @return A @c QStringList of all of the pages.
     **/
    const QStringList pages() const;

    /**
     * @brief Obtains a specified page at index p_indx.
     * @param p_indx The index at which the page is to be found.
     * @return A string with the page's text, or QString::null if not found.
     **/
    const QString pageAt ( const int& p_indx ) const;

    bool isValid() const;

    /**
     * @brief Creates a new @c Content object to be used by @c Session objects.
     * @param p_author The author/writer of this text.
     * @param p_title The title of the text.
     * @param p_text The text that's represented by this @c Content.
     * @return A pointer to the new @c Content object created.
     **/
    static Content* create ( const QString& p_author, const QString& p_title , const QString& p_text );

    /**
     * @brief Obtains a specific @Content by its identifying @c QUuid.
     * @param p_uuid The UUID to pick out the @c Content by.
     * @return
     **/
    static Content* obtain ( const QUuid& p_uuid );

    /**
     * @brief
     * @return
     **/
    static ContentList allContents();
private:
    static QString getPath ( const QUuid& );
    void parseText ( const QString& p_text );
    static ContentMap s_lst;
    QStringList m_pages;
    QDomDocument* m_dom;
    QUuid m_uuid;
};

/**
 * @brief ...
 **/
class Session : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Session )
    Q_PROPERTY ( Corpus* Corpus READ corpus WRITE setCorpus )
    Q_PROPERTY ( QUuid Uuid READ uuid )
    Q_PROPERTY ( Content* Content READ content WRITE setContent )

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
    //static const QString getPath(const QUuid&);
    static QDomDocument* s_dom;
    static QMap<QUuid, QDomElement*> s_elems;
    Corpus* m_corpus;
    Content* m_content;
    QDomElement* m_elem;
};
}

#endif // SESSION_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
