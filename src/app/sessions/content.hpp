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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <QUuid>
#include <QMap>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QDomElement>

namespace SpeechControl {
class Content;
/**
 * @brief Represents a @c QList of @c Content objects.
 **/
typedef QList<Content*> ContentList;

/**
 * @brief Represents a @c QMap of @c Content objects, mapped by their @c QUuid keys.
 **/
typedef QMap<QUuid, Content*> ContentMap;

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
     * @brief Obtains the title of this @c Content, or if provided, the nickname of the Session.
     * @return A string holding the title of this @c Content, or the nickname if provided.
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

    /**
     * @brief Determines if this @c Session is valid.
     * @return bool
     **/
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

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
