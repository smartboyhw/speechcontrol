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

#include <QMap>
#include <QObject>
#include <QString>

class QFile;

namespace SpeechControl
{
class Dictionary;
class DictionaryEntry;
class DictionaryPrivate;
class DictionaryEntryPrivate;
typedef QMap<QString, Dictionary* > DictionaryMap;
typedef QMap<QString, DictionaryEntry* > DictionaryEntryMap;

class DictionaryEntryPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (DictionaryEntryPrivate)

public:
    explicit DictionaryEntryPrivate (Dictionary* p_dictionary,
                                     const QString& p_word,
                                     const QString& p_phoneme);
    virtual ~DictionaryEntryPrivate();

    Dictionary* m_dict;     ///< The Dictionary that owns this object.
    QString m_word;         ///< The word of this entry.
    QString m_phnm;         ///< The phoneme of this entry.
};

class DictionaryPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (DictionaryPrivate)

public:
    explicit DictionaryPrivate (QObject* parent = 0);
    virtual ~DictionaryPrivate();
    static QString getPathFromId (const QString& p_id);
    static DictionaryMap s_lst;
    DictionaryEntryMap m_words;
    QFile* m_device;

};

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
