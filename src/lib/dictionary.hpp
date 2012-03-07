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

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <QMap>
#include <QDir>
#include <QList>
#include <QUuid>
#include <QObject>
#include <QDateTime>

#include <export.hpp>

class QUrl;
class QFile;
class QDomDocument;
class QDomElement;

namespace SpeechControl {
class Corpus;
class Phrase;
class Sentence;
class Dictionary;
class DictionaryEntry;

/**
 * @brief Represents a list of Dictionary objects.
 **/
typedef QList<Dictionary*> DictionaryList;

/**
 * @brief Represents a list of DictionaryEntry objects.
 **/
typedef QList<DictionaryEntry*> DictionaryEntryList;

/**
 * @brief Represents a mapping of QUuids to Dictionary objects.
 **/
typedef QMap<QUuid, Dictionary*> DictionaryMap;

/**
 * @brief Represents a mapping of QStrings to DictionaryEntry objects.
 **/
typedef QMap<QString, DictionaryEntry*> DictionaryEntryMap;

/**
 * @brief Represents an entry in a Dictionary object.
 *
 * Entries within a dictionary are typically used to provide the most
 * accurate pronunciation (phoneme) of words for Sphinx to use during
 * the act of building or adapting acoustic models.
 *
 * @see Dictionary
 **/
class SPCH_EXPORT DictionaryEntry : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( DictionaryEntry )
    Q_PROPERTY ( QString Word READ word )
    Q_PROPERTY ( QString Phoneme READ phoneme )
    friend class Dictionary;

public:
    /**
     * @brief Constructor.
     * Creates the representation of an entry for a Dictionary p_dictionary with the specified
     * word p_word and the phoneme p_phoneme.
     * @param p_dictionary Dictionary
     * @param p_word QString
     * @param p_phoneme QString
     **/
    DictionaryEntry ( Dictionary* p_dictionary , const QString& p_word, const QString& p_phoneme );
    virtual ~DictionaryEntry();

    /**
     * @brief Obtains the word represented.
     * @return QString
     **/
    QString word() const;

    /**
     * @brief Obtains the phoneme represented.
     * @return QString
     **/
    QString phoneme() const;

private:
    Dictionary* m_dict;     ///< The Dictionary that owns this object.
    QString m_word;         ///< The word of this entry.
    QString m_phnm;         ///< The phoneme of this entry.
};

/**
 * @brief Represents a collection of words recognizable by SpeechControl.
 *
 * Dictionaries are used tightly with SpeechControl and its training facilities
 * in order to provide the appropriate linkages with the word and their corresponding
 * phonemes.
 **/
class SPCH_EXPORT Dictionary : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Dictionary )
    friend class Corpus;

public:

    /**
     * @brief Null constructor.
     *
     * @param p_parent Defaults to 0.
     **/
    explicit Dictionary ( QObject* p_parent = 0 );

    /**
     * @brief Constructor.
     * @param p_uuid The UUID to obtain this dictionary with.
     **/
    Dictionary ( const QUuid& p_uuid );

    /**
     * @brief Destructor.
     **/
    virtual ~Dictionary();

    /**
     * @brief Obtains a Dictionary by its UUID.
     * @param p_uuid The UUID to obtain the dictionary with.
     * @return A pointer to a Dictionary object if found, NULL otherwise.
     **/
    static Dictionary* obtain ( const QUuid& p_uuid );

    /**
     * @brief Obtains a Dictionary from a path.
     *
     * @param p_path The path of which the dictionary resides.
     * @return A pointer to a Dictionary object if found, NULL otherwise.
     **/
    static Dictionary* obtain ( const QString& p_path );

    /**
     * @brief Obtains the list of entries representing this Dictionary.
     * @return A pointer to a DictionaryEntryList list, NULL otherwise if no entries exist.
     **/
    DictionaryEntryList entries() const;

    /**
     * @brief Adds an entry into this Dictionary.
     * @param  p_entry The DictionaryEntry to add.
     **/
    void addEntry ( DictionaryEntry* p_entry );

    /**
     * @brief Removes an entry by the word specifying it.
     *
     * @param p_word The word to add into the entry.
     * @return A DictionaryEntry from the formed action, or returns the existing entry.
     **/
    DictionaryEntry* removeEntry ( const QString& p_word );

    /**
     * @brief Adds a DictionaryEntry to the Dictionary.
     *
     * @param p_entry The DictionaryEntry to add.
     **/
    Dictionary& operator<< ( DictionaryEntry* p_entry );

    /**
     * @brief Adds a DictionaryEntryList into this Dictionary.
     *
     * @param p_list The DictionaryEntryList to add.
     **/
    Dictionary& operator<< ( DictionaryEntryList& p_list );

    /**
     * @brief Loads the Dictionary from the specified QIODevice p_device.
     **/
    void load ( QIODevice* p_device );

    /**
     * @brief Loads the Dictionary from a specified UUID.
     *
     * @param p_uuid The UUID of the Dictionary to load.
     **/
    void load ( const QUuid& p_uuid );

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
    static QString getPathFromUuid ( const QUuid& p_uuid );
    static DictionaryMap s_lst;
    DictionaryEntryMap m_words;
    QIODevice* m_device;
};
}
#endif // DICTIONARY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
