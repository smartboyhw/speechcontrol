/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <QMap>
#include <QDir>
#include <QList>
#include <QUuid>
#include <QObject>
#include <QDateTime>

class QUrl;

class QFile;

class QDomDocument;

class QDomElement;

namespace SpeechControl
{

class Corpus;

class Phrase;

class Sentence;

class Dictionary;

typedef QList<Dictionary*> DictionaryList;
typedef QMap<QUuid, Dictionary*> DictionaryMap;

/**
 * @brief Single entry in a dictionary.
 */

class DictionaryEntry : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (DictionaryEntry)

    friend class Dictionary;

public:
    DictionaryEntry (Dictionary*, const QString&, const QString&);
    virtual ~DictionaryEntry();
    QString word() const;
    QString phoneme() const;

private:
    Dictionary* m_dict;
    QString m_word;
    QString m_phnm;
};

typedef QList<DictionaryEntry*> DictionaryEntryList;
typedef QMap<QString, DictionaryEntry*> DictionaryEntryMap;

/**
 * @brief Phonetic dictionary.
 */

class Dictionary : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Dictionary)

    friend class Corpus;

public:
    Dictionary (const QUuid&);
    virtual ~Dictionary();
    
    static Dictionary* obtain (const QUuid&);
    DictionaryEntryList* entries() const;
    
    void addEntry (DictionaryEntry*);
    DictionaryEntry* removeEntry (const QString&);
    Dictionary& operator<< (DictionaryEntry*);
    Dictionary& operator<< (DictionaryEntryList& entries);

public slots:
    void load (const QUuid&);
    void save();

private:
    static QString getPath (const QUuid&);
    static DictionaryMap dicts;
    DictionaryEntryMap m_words;
};
}
#endif // DICTIONARY_HPP
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
