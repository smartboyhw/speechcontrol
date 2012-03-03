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

#include "dictionary.hpp"
#include "corpus.hpp"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDomDocument>

using namespace SpeechControl;

Dictionary::Dictionary ( QObject* p_parent ) : QObject ( p_parent ) {

}

Dictionary::Dictionary ( const Dictionary& p_other ) : QObject ( p_other.parent() ), m_words ( p_other.m_words ),
    m_device ( p_other.m_device ) {

}

Dictionary::Dictionary ( const QUuid &p_uuid ) {
    load ( new QFile ( getPathFromUuid ( p_uuid ) ) );
}

void Dictionary::load ( const QUuid& p_uuid ) {
    load ( getPathFromUuid ( p_uuid ) );
}

void Dictionary::load ( QIODevice* p_device ) {
    Q_ASSERT ( m_device != 0 || p_device != 0 );

    if ( p_device )
        m_device = p_device;

    if ( !m_device->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qWarning() << "Failed to open dictionary" << m_device->errorString();
        return;
    }

    QTextStream l_strm ( m_device );

    while ( !l_strm.atEnd() ) {
        const QString l_line = l_strm.readLine();
        const QStringList l_tokens = l_line.split ( "\t",QString::SkipEmptyParts );
        addEntry ( new DictionaryEntry ( this,l_tokens[0],l_tokens[1] ) );
    }

    m_device->close();

    qDebug() << m_words.size() << "words found in this dictionary.";
}

QString Dictionary::getPathFromUuid ( const QUuid& p_uuid ) {
    return QDir::homePath() + "/.speechcontrol/dictionaries/" + p_uuid.toString() + ".dic";
}

Dictionary* Dictionary::obtain ( const QUuid &p_uuid ) {
    if ( !QFile::exists ( getPathFromUuid ( p_uuid ) ) )
        return 0;

    Dictionary* l_dict = new Dictionary ( p_uuid );
    return l_dict;
}

Dictionary* Dictionary::obtain ( const QString& p_path ) {
    QFile* l_file = new QFile(p_path);
    Dictionary* l_dict = new Dictionary;
    if (!l_file->exists())
        return 0;

    l_dict->load(l_file);
    return l_dict;
}

DictionaryEntryList Dictionary::entries() const {
    return m_words.values();
}

void Dictionary::addEntry ( DictionaryEntry *p_entry ) {
    m_words.insert ( p_entry->word(),p_entry );
}

DictionaryEntry * Dictionary::removeEntry ( const QString& p_word ) {
    return m_words.take ( p_word );
}

Dictionary& Dictionary::operator << ( DictionaryEntry *p_entry ) {
    addEntry ( p_entry );
    return *this;
}

Dictionary& Dictionary::operator << ( DictionaryEntryList& p_list ) {
    Q_FOREACH ( DictionaryEntry* l_entry, p_list )
    addEntry ( l_entry );

    return *this;
}

/// @todo Implement the saving ability.
void Dictionary::save() {
    m_device->open ( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text );
    QTextStream l_strm ( m_device );

    Q_FOREACH ( const DictionaryEntry* l_entry, entries() ) {
        l_strm << l_entry->word() << "\t" << l_entry->phoneme();
    }

    m_device->close();
}

Dictionary::~Dictionary() {
}

DictionaryEntry::DictionaryEntry ( Dictionary* p_dictionary, const QString& p_word, const QString& p_phoneme ) :
    QObject ( p_dictionary ), m_dict ( p_dictionary ), m_word ( p_word ), m_phnm ( p_phoneme ) {
}

DictionaryEntry::DictionaryEntry ( const DictionaryEntry& p_other ) : QObject(),
    m_dict ( p_other.m_dict ), m_word ( p_other.m_word ), m_phnm ( p_other.m_phnm ) {

}

QString DictionaryEntry::word() const {
    return m_word;
}

QString DictionaryEntry::phoneme() const {
    return m_phnm;
}

DictionaryEntry::~DictionaryEntry() {

}

#include "dictionary.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
