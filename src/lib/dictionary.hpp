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
    Dictionary* m_dict;     /// < The Dictionary that owns this object.
    QString m_word;         /// < The QString of the
    QString m_phnm;
};

/**
 * @brief ...
 **/
class SPCH_EXPORT Dictionary : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Dictionary )
    friend class Corpus;

public:
    /**
     * @brief ...
     *
     * @param  ...
     **/
    Dictionary ( const QUuid& );
    /**
     * @brief ...
     *
     **/
    virtual ~Dictionary();
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Dictionary*
     **/
    static Dictionary* obtain ( const QUuid& );
    /**
     * @brief ...
     *
     * @return :DictionaryEntryList*
     **/
    DictionaryEntryList* entries() const;
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void addEntry ( DictionaryEntry* );
    /**
     * @brief ...
     *
     * @param  ...
     * @return :DictionaryEntry*
     **/
    DictionaryEntry* removeEntry ( const QString& );
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Dictionary&
     **/
    Dictionary& operator<< ( DictionaryEntry* );
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Dictionary&
     **/
    Dictionary& operator<< ( DictionaryEntryList& );
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Dictionary*
     **/
    static Dictionary* fromDirectory ( const QDir& );

public slots:
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void load ( const QUuid& );
    /**
     * @brief ...
     *
     * @return void
     **/
    void save();

private:
    static const QString getPath ( const QUuid& );
    static DictionaryMap s_lst;
    DictionaryEntryMap m_words;
};
}
#endif // DICTIONARY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
