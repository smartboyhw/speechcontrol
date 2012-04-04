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

#ifndef SPEECHCONTROL_NOISEDICTIONARY_HPP
#define SPEECHCONTROL_NOISEDICTIONARY_HPP

#include <QMap>
#include <QFile>
#include <QObject>
#include <QString>

#include <lib/config.hpp>

namespace SpeechControl
{

class NoiseDictionaryPrivate;
class NoiseDictionary;

typedef QMap<QString, QString> QStringMap;

/**
 * @brief Represents a dictionary of noises.
 *
 * Noise dictionaries allow acoustic models to define sounds that would
 * be typically unused or unrepresented in speech (aside from the occasional whitespace).
 * Sounds like laughter and silence would be considered noise and listed here.
 **/
class NoiseDictionary : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NoiseDictionary)

public:
    /**
     * @brief Null constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit NoiseDictionary (QObject* p_parent = 0);

    /**
     * @brief Copy constructor.
     * @param p_other The other NoiseDictionary to be copied.
     **/
    NoiseDictionary (const NoiseDictionary& p_other);

    /**
     * @brief Obtains a NoiseDictionary from a QFile.
     * @param p_file The QFile to use to load the NoiseDictionary.
     * @return A pointer to a NoiseDictionary object, or NULL if there was a failure.
     **/
    static NoiseDictionary* fromFile (QFile* p_file);

    /**
     * @brief Obtains a mapping of the entries in this noise dictionary.
     **/
    QStringMap entries();

    /**
     * @brief Determines whether or not a entry exists.
     * @param p_entry The entry in question.
     **/
    bool hasEntry (const QString& p_entry);

    /**
     * @brief Loads a QIODevice to use as the source of this NoiseDictionary.
     * @param p_device The QIODevice to be used.
     * @note This NoiseDictionary takes the pointer of the QIODevice so it can use it to save data.
     * @see save()
     **/
    bool load (QIODevice* p_device);

    /**
     * @brief Merges a set of entries with the entries in this NoiseDictionary.
     * @param p_entries The entries to merge.
     **/
    void mergeEntries (const QStringMap& p_entries);

    /**
     * @brief Adds an entry to the NoiseDictionary.
     *
     * @param p_entry The entry to add.
     * @param p_value The value of the entry to add.
     *
     * @note If the entry already exists, it's updated with the new value.
     **/
    void addEntry (const QString& p_entry, const QString& p_value);

    /**
     * @brief Saves the NoiseDictionary to the QIODevice used to load data.
     * @see load();
     **/
    void save();

    /**
     * @brief Determines if the NoiseDictionary is valid.
     **/
    bool isValid() const;

private:
    QScopedPointer<NoiseDictionaryPrivate> d_ptr;
};

}

#endif // SPEECHCONTROL_NOISEDICTIONARY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
