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
 * @date 05/18/12 16:04:14 PM
 */

#ifndef SPCHCNTRL_DICTIONARY_HPP
#define SPCHCNTRL_DICTIONARY_HPP

#include <QDir>
#include <QMap>
#include <QList>
#include <QString>
#include <QObject>
#include <QScopedPointer>
#include <QDateTime>

#include <lib/global.hpp>
#include <lib/export.hpp>

class QUrl;
class QFile;
class QDomDocument;
class QDomElement;

namespace SpeechControl {

class Phrase;
class Sentence;
class Dictionary;
class DictionaryPrivate;
class DictionaryEntry;
class DictionaryEntryPrivate;

/**
 * @brief Represents a list of Dictionary objects.
 **/
typedef QList<Dictionary*> DictionaryList;

/**
 * @brief Represents a list of DictionaryEntry objects.
 **/
typedef QList<DictionaryEntry*> DictionaryEntryList;

/**
 * @brief Represents a mapping of QStrings to Dictionary objects.
 **/
typedef QMap<QString, Dictionary*> DictionaryMap;

/**
 * @brief Represents a mapping of QStrings to DictionaryEntry objects.
 **/
typedef QMap<QString, DictionaryEntry*> DictionaryEntryMap;

/**
 * @brief Represents an entry in a Dictionary object.
 *
 * Entries within a dictionary are typically used to provide the most
 * accurate pronunciation (phoneme) of words for Sphinx to use during
 * the act of building or adapting acoustic models. It's important that
 * dictionary objects render the proper and a valid phoneme scheme for
 * words.
 *
 * @see Dictionary
 **/
class SPCH_EXPORT DictionaryEntry : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (DictionaryEntry)
    Q_DECLARE_PRIVATE (DictionaryEntry)
    Q_PROPERTY (QString Word READ word)
    Q_PROPERTY (QString Phoneme READ phoneme)
    friend class Dictionary;

    /**
     * @brief Constructor.
     *
     * Creates the representation of an entry for a Dictionary p_dictionary with the specified
     * word p_word and the phoneme p_phoneme.
     *
     * @param p_dictionary A pointer to a Dictionary object.
     * @param p_word A string of the word being represented.
     * @param p_phoneme A string of the phoneme used by this word.
     **/
    DictionaryEntry (Dictionary* p_dictionary , const QString& p_word, const QString& p_phoneme);

private:

    /**
     * @brief Destructor.
     **/
    virtual ~DictionaryEntry();

    /**
     * @brief Obtains the word represented.
     * @return A string of the word being represented by this dictionary entry.
     **/
    QString word() const;

    /**
     * @brief Obtains the phoneme represented.
     * @return A string of the phoneme represented by this dictionary entry.
     **/
    QString phoneme() const;

private:
    QScopedPointer<DictionaryEntryPrivate> d_ptr;
};

/**
 * @brief Represents a collection of words recognizable by SpeechControl.
 *
 * Dictionaries are used tightly with SpeechControl and its training facilities
 * in order to provide the appropriate linkages with the word and their corresponding
 * phonemes.
 **/
class SPCH_EXPORT Dictionary : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Dictionary)
    Q_DECLARE_PRIVATE (Dictionary)
    friend class Corpus;

public:

    /**
     * @brief Null constructor.
     *
     * @param p_parent Defaults to 0.
     **/
    explicit Dictionary (QObject* p_parent = 0);

    /**
     * @brief Constructor.
     * @param p_id The ID to obtain this dictionary with.
     **/
    Dictionary (const QString& p_id);

    /**
     * @brief Destructor.
     **/
    virtual ~Dictionary();

    /**
     * @brief Obtains a Dictionary by its ID.
     * @param p_id The ID to obtain the dictionary with.
     * @return A pointer to a Dictionary object if found, NULL otherwise.
     **/
    static Dictionary* obtain (const QString& p_id);

    /**
     * @brief Obtains a Dictionary from a path.
     *
     * @param p_path The path of which the dictionary resides.
     * @return A pointer to a Dictionary object if found, NULL otherwise.
     **/
    static Dictionary* obtainFromPath (const QString& p_path);

    /**
     * @brief Creates a new Dictionary from the specified text.
     *
     * @param p_text The text to use.
     * @param p_id The ID of the new Dictionary.
     * @return Dictionary* A pointer to the newly formed Dictionary.
     **/
    static Dictionary* create (QStringList p_text, QString p_id);

    /**
     * @brief Merges a collection of Dictionary objects into one dictionary.
     *
     * @param p_list A list of the Dictionary objects to merge.
     * @param p_id The ID of the merged Dictionary.
     **/
    static Dictionary* merge(const DictionaryList &p_list, const QString& p_id);

    /**
     * @brief Obtains the list of entries representing this Dictionary.
     * @return A pointer to a DictionaryEntryList list, NULL otherwise if no entries exist.
     **/
    DictionaryEntryList entries() const;

    /**
     * @brief Adds an entry into this Dictionary.
     *
     * Appends a new DictionaryEntry, p_entry, into this Dictionary object.
     *
     * @param p_entry The DictionaryEntry to add.
     **/
    void addEntry (DictionaryEntry* p_entry);

    /**
     * @brief Removes an entry by the word specifying it.
     *
     * @param p_word The word to add into the entry.
     * @return A DictionaryEntry from the formed action, or returns the existing entry.
     **/
    DictionaryEntry* removeEntry (const QString& p_word);

    /**
     * @brief Adds a DictionaryEntry to the Dictionary.
     *
     * @param p_entry The DictionaryEntry to add.
     **/
    Dictionary& operator<< (DictionaryEntry* p_entry);

    /**
     * @brief Adds a DictionaryEntryList into this Dictionary.
     *
     * @param p_list The DictionaryEntryList to add.
     **/
    Dictionary& operator<< (DictionaryEntryList& p_list);

    /**
     * @brief Loads the Dictionary from the specified QIODevice p_device.
     **/
    void load (QFile* p_device);

    /**
     * @brief Loads the Dictionary from a specified ID.
     *
     * @param p_id The ID of the Dictionary to load.
     **/
    void load (const QString& p_id);

    /**
     * @brief Saves the Dictionary.
     * @note This method uses the QIODevice specified in the @c load() method.
     **/
    void save();

    /**
     * @brief Obtains the path to this Dictionary.
     **/
    QString path() const;

private:
    QScopedPointer<DictionaryPrivate> d_ptr;
};

}

#endif // DICTIONARY_HPP
// kate: indent-mode cstyle; replace-tabs on;
