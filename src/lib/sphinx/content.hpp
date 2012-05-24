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
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 22:37:15 PM
 */

#ifndef SPCHCNTRL_CONTENT_HPP
#define SPCHCNTRL_CONTENT_HPP

#include <QMap>
#include <QUrl>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QStringList>
#include <app/global.hpp>

class QFile;
class QDomDocument;

SPCHCNTRL_BEGIN_NAMESPACE

class Content;
class AbstractContentSource;
class TextContentSource;
struct ContentPrivate;
struct AbstractContentSourcePrivate;

/**
 * @brief Represents a @c QList of @c Content objects.
 **/
typedef QList<Content*> ContentList;

/**
 * @brief Represents a @c QMap of @c Content objects, mapped by their @c QString keys.
 **/
typedef QMap<QString, Content*> ContentMap;

/**
 * @brief Represents a collection of text to be used for training by @c Sessions.
 *
 * Contents are the pure-text representation of @c Corpus objects; @c Content objects
 * goes out to provide more specific data about the text being trained. Information about
 * authors, books and more can be injected into Content objects. AbstractContentSource
 * provides a interface for rendering Content objects with ease.
 *
 * @see AbstractContentSource
 **/
class Content : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Content)
    Q_DISABLE_COPY (Content)
    QScopedPointer<ContentPrivate> d_ptr;

public:
    /**
     * @brief Default constructor.
     * @param p_id The uuid of the @c Content.
     **/
    explicit Content (const QString& p_id);

    /**
     * @brief Destructor.
     **/
    virtual ~Content();

    /**
     * @brief Loads a @c Content object by a specified ID.
     * @param p_id The uuid of the Content to load.
     * @note After loading, you should check to see if isValid() returns true. It's possible for the loading operation to fail.
     **/
    void load (const QString& p_id);

    /**
     * @brief Loads a Content object from a file.
     *
     * @param p_file The QFile to load the Content from.
     **/
    void load (QFile* p_file);

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
     * @brief Obtains a unique count of words in this @Content and returns that number.
     * @return A unsigned integer representing the unique count of words in the text.
     **/
    uint uniqueWords() const;

    /**
     * @brief Counts the characters within this @c Content and returns that number.
     * @return A unsigned integer representing the total number of characters in the text.
     **/
    uint length() const;

    /**
     * @brief Counts the alphanumerical characters within this @c Content and returns that number.
     * @return A unsigned integer representing the total number of alphanumerical characters in the text.
     **/
    uint characters() const;

    /**
     * @brief Obtains the ID of this @c Content.
     * @return The ID of this @c Content.
     **/
    QString id() const;

    /**
     * @brief Obtains the title of this @c Content, or if provided, the nickname of the Session.
     * @return A string holding the title of this @c Content, or the nickname if provided.
     **/
    QString title() const;

    /**
     * @brief Obtains the author of this @c Content.
     * @return A string holding the name of the author of this @c Content.
     **/
    QString author() const;

    /**
     * @brief Obtains a list representing all of the pages contained by this @c Content.
     * @return A @c QStringList of all of the pages.
     **/
    QStringList pages() const;

    /**
     * @brief Get a number of sentences.
     *
     * @return A number of sentences in this Content.
     **/
    int uttNumber() const;

    /**
     * @brief Get an utterance from the Content.
     *
     * @param id List index to the utterance.
     * @return Utterance at the given index.
     * @throws OutOfBoundException
     */
    QString getUtterance(int id) const;

    /**
     * @brief Get a sequence of utterances.
     *
     * @param begin First index of the sequence.
     * @param end Last index of the sequence.
     * @return List of the utterances.
     */
    QStringList getUtteranceSeq(int begin, int end) const;

    /**
     * @brief Obtains a specified page at index p_indx.
     * @param p_indx The index at which the page is to be found.
     * @return A string with the page's text, or QString::null if not found.
     **/
    QString pageAt (const int& p_index) const;

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
    static Content* create (const QString& p_author, const QString& p_title , const QString& p_text);

    /**
     * @brief Obtains a specific @Content by its identifying @c QString.
     * @param p_id The ID to pick out the @c Content by.
     **/
    static Content* obtain (const QString& p_id);

    /**
     * @brief Obtains a specific @Content from a URL.
     *
     * @param p_url The URL to the Content.
     **/
    static Content* obtainFromFile (const QUrl& p_url);

    /**
     * @brief Obtains a list of Contents object recognized by SpeechControl.
     **/
    static ContentList allContents();
};

/**
 * @brief An abstract outline for content sources to base themselves upon.
 *
 * Content for model training can arise from anywhere. Thus, a base for
 * manipulating and obtaining such sources need to be made. AbstractContentSource
 * allows developers to implement a new source of content for the user to use within
 * SpeechControl for training. This, however, does not cause it to appear in the
 * the content creation wizard. In order to do this, one must implement an interface
 * for that and register it with SpeechControl's wizard system.
 **/
class AbstractContentSource : public QObject
{
    Q_OBJECT
    QScopedPointer<AbstractContentSourcePrivate> d_ptr;

protected:
    Q_DISABLE_COPY(AbstractContentSource)
    Q_DECLARE_PRIVATE(AbstractContentSource)

    /**
     * @brief Destructor.
     **/
    virtual ~AbstractContentSource();

    /**
     * @brief Null constructor.
     *
     * @param p_parent Defaults to 0.
     **/
    explicit AbstractContentSource (QObject* p_parent = 0);

    /**
     * @brief The ID of this AbstractContentSource.
     *
     * @param p_id The ID to use.
     * @param p_parent Defaults to 0.
     **/
    AbstractContentSource (QString p_id, QObject* p_parent = 0);

public:
    /**
     * @brief Sets the author of the Content to be generated.
     *
     * @param p_author The name of the author.
     **/
    void setAuthor (const QString& p_author);

    /**
     * @brief Sets the title of the Content to be generated.
     *
     * @param p_title The title of the content.
     **/
    void setTitle (const QString& p_title);

    /**
     * @brief Sets the text of the Content.
     *
     * @param p_text The text of the Content.
     **/
    void setText (const QString& p_text);

    /**
     * @brief Obtains the ID of this AbstractContentSource.
     **/
    QString id() const;

    /**
     * @brief Obtains the author of this AbstractContentSource.
     **/
    QString author() const;

    /**
     * @brief Obtains the title of this AbstractContentSource.
     **/
    QString title() const;

    /**
     * @brief Obtains the text of this AbstractContentSource.
     **/
    QString text() const;

    /**
     * @brief Determines if the data stored by this AbstractContentSource is valid.
     *
     * The data of the AbstractContentSource is considered to be valid when the
     * title, author and text are all set to valid, non-empty values.
     *
     * @return TRUE if the data's valid, FALSE otherwise.
     **/
    bool isValid();

    /**
     * @brief Generates a Content from the information stored by this AbstractContentSource.
     **/
    Content* generate();
};

/**
 * @brief A content source that allows content generation from XML-formatted text sources.
 *
 * Sources that are formatted in the XML format for SpeechControl (typically end in the *.spch
 * extension) can be used to render content for SpeechControl for corpus training. This format
 * is typically the local and distributional format as it's usable for sharing.
 *
 * @see TextContentSource
 **/
class TextContentSource : public AbstractContentSource
{
    Q_OBJECT

public:
    explicit TextContentSource (QObject* p_parent = 0);
    virtual ~TextContentSource();
    /**
     * @brief Sets the QFile to be used to load the Content data.
     *
     * @param p_file A reference to the QFile in question.
     * @return TRUE if the data was loaded successfully, FALSE otherwise.
     **/
    bool setFile (QFile* p_file);

    /**
     * @brief Attempts to load data from the specified QUrl p_url to be used as the AbstractContentSource.
     *
     * @param p_url A constant reference to the QUrl of the potential data.
     * @return TRUE if the data was loaded successfully, FALSE otherwise.
     **/
    bool setUrl (const QUrl& p_url);
};

SPCHCNTRL_END_NAMESPACE

#endif
// kate: indent-mode cstyle; replace-tabs on; 
