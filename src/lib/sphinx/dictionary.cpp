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
 * @date 05/18/12 15:40:34 PM
 */

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QVariant>
#include <QDomDocument>
#include "dictionaryprivate.hpp"
#include "dictionary.hpp"

using namespace SpeechControl;

DictionaryPrivate::DictionaryPrivate () : m_words(), m_device (0)
{

}

DictionaryPrivate::~DictionaryPrivate()
{

}

QString DictionaryPrivate::getPathFromId (const QString& p_id)
{
    return QDir::homePath() + "/.config/speechcontrol/dictionaries/" + p_id + ".dict";
}

DictionaryEntryPrivate::DictionaryEntryPrivate (Dictionary* p_dictionary, const QString& p_word, const QString& p_phoneme) :
    m_dict (p_dictionary), m_word (p_word), m_phnm (p_phoneme)
{

}

DictionaryEntryPrivate::~DictionaryEntryPrivate()
{

}

Dictionary::Dictionary (QObject* p_parent) :
    QObject (p_parent), d_ptr (new DictionaryPrivate)
{
}

Dictionary::Dictionary (const Dictionary& p_other) :
    QObject (p_other.parent()), d_ptr (const_cast<DictionaryPrivate*> (p_other.d_ptr.data()))
{
}

Dictionary::Dictionary (const QString& p_id) :
    QObject (0), d_ptr (new DictionaryPrivate)
{
    load (new QFile (DictionaryPrivate::getPathFromId (p_id)));
}

void Dictionary::load (const QString& p_id)
{
    load (DictionaryPrivate::getPathFromId (p_id));
}

Dictionary* Dictionary::create (QStringList p_wordlist, QString p_id)
{
    QFile* fileDictionary = new QFile (DictionaryPrivate::getPathFromId (p_id));
    fileDictionary->open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    QTextStream strm (fileDictionary);
    qDebug() << "[Dictionary::create()] Removed " << p_wordlist.removeDuplicates() << "extra words.";

    Q_FOREACH (const QString & word, p_wordlist) {
        QString phonemes;
        QString wordUpper = word.toUpper();
        wordUpper = wordUpper.trimmed().simplified();

        Q_FOREACH(const QChar& letter, wordUpper) {
            phonemes += letter;
            phonemes += " ";
        }

        phonemes = phonemes.trimmed();
        qDebug() << word << wordUpper << phonemes;
        strm << wordUpper << "\t" << phonemes << endl;
    }

    fileDictionary->close();

    return Dictionary::obtain (p_id);
}

Dictionary* Dictionary::merge(const DictionaryList& p_list, const QString& p_id)
{
    QFile* fileDictionary = new QFile (DictionaryPrivate::getPathFromId (p_id));
    fileDictionary->open (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    QTextStream strm (fileDictionary);

    Q_FOREACH(const Dictionary* p_dict, p_list) {
        QFile dict(p_dict->path());
        strm << dict.readAll() << endl;
    }

    fileDictionary->close();
    return Dictionary::obtain(p_id);
}

void Dictionary::load (QFile* p_device)
{
    Q_ASSERT (d_func()->m_device != 0 || p_device != 0);

    if (p_device)
        d_func()->m_device = p_device;

    if (!d_func()->m_device->open (QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[Dictionary::load()] Failed to open dictionary" << d_func()->m_device->errorString();
        return;
    }

    QTextStream strm (d_func()->m_device);

    while (!strm.atEnd()) {
        const QString line = strm.readLine();

        if (!line.isEmpty() && !line.isNull() && line.length() >= 3) {
            QStringList tokens = line.split ("\t", QString::SkipEmptyParts);
            tokens.removeAll ("\t");
            const QString word = tokens.at (0);
            const QString phoneme = tokens.at (1);
            addEntry (new DictionaryEntry (this, word, phoneme));
        }
    }

    d_func()->m_device->close();

    qDebug() << "[Dictionary::load()]" << d_func()->m_words.size() << "words found in this dictionary.";
}

Dictionary* Dictionary::obtain (const QString& p_id)
{
    if (!QFile::exists (DictionaryPrivate::getPathFromId (p_id)))
        return 0;

    Dictionary* l_dict = new Dictionary (p_id);
    return l_dict;
}

Dictionary* Dictionary::obtainFromPath (const QString& p_path)
{
    QFile* l_file = new QFile (p_path);
    Dictionary* l_dict = new Dictionary;

    if (!l_file->exists())
        return 0;

    l_dict->load (l_file);
    return l_dict;
}

DictionaryEntryList Dictionary::entries() const
{
    return d_func()->m_words.values();
}

void Dictionary::addEntry (DictionaryEntry* p_entry)
{
    d_func()->m_words.insert (p_entry->word(), p_entry);
}

DictionaryEntry* Dictionary::removeEntry (const QString& p_word)
{
    return d_func()->m_words.take (p_word);
}

Dictionary& Dictionary::operator << (DictionaryEntry* p_entry)
{
    addEntry (p_entry);
    return *this;
}

Dictionary& Dictionary::operator << (DictionaryEntryList& p_list)
{
    Q_FOREACH (DictionaryEntry * entry, p_list) {
        addEntry (entry);
    }

    return *this;
}

/// @todo Implement the saving ability.
void Dictionary::save()
{
    d_func()->m_device->open (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream strm (d_func()->m_device);

    Q_FOREACH (const DictionaryEntry * entry, entries()) {
        strm << entry->word() << "\t" << entry->phoneme();
    }

    d_func()->m_device->close();
}

QString Dictionary::path() const
{
    return d_func()->m_device->fileName();
}

Dictionary::~Dictionary()
{
}

DictionaryEntry::DictionaryEntry (Dictionary* p_dictionary, const QString& p_word, const QString& p_phoneme) :
    QObject (p_dictionary), d_ptr (new DictionaryEntryPrivate (p_dictionary, p_word, p_phoneme))
{
}

DictionaryEntry::DictionaryEntry (const DictionaryEntry& p_other) : QObject (p_other.parent()),
    d_ptr (const_cast<DictionaryEntryPrivate*> (p_other.d_ptr.data()))
{

}

QString DictionaryEntry::word() const
{
    return d_func()->m_word;
}

QString DictionaryEntry::phoneme() const
{
    return d_func()->m_phnm;
}

DictionaryEntry::~DictionaryEntry()
{

}

#include "dictionary.moc"
// kate: indent-mode cstyle; replace-tabs on;
