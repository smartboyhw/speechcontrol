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


void Dictionary::load ( const QUuid &l_uuid ) {
    QFile* l_file = new QFile ( getPath ( l_uuid ) );
    l_file->open ( QIODevice::ReadOnly );
    QTextStream l_strm ( l_file );

    while ( !l_strm.atEnd() ) {
        const QString l_line = l_strm.readLine();
        const QStringList l_tokens = l_line.split ( "\t",QString::SkipEmptyParts );
        m_words.insert ( l_tokens[0],new DictionaryEntry ( this,l_tokens[0],l_tokens[1] ) );
    }
}

const QString Dictionary::getPath ( const QUuid &l_uuid ) {
    return QDir::homePath() + "/.speechcontrol/dictionaries/" + l_uuid.toString() + ".dic";
}

DictionaryEntry::DictionaryEntry ( Dictionary* p_dictionary, const QString& p_word, const QString& p_phoneme ) :
    QObject ( p_dictionary ), m_dict ( p_dictionary ), m_word ( p_word ), m_phnm ( p_phoneme ) {
}

DictionaryEntry::DictionaryEntry ( const DictionaryEntry& p_other ) : QObject(),
    m_dict ( p_other.m_dict ), m_word ( p_other.m_word ), m_phnm ( p_other.m_phnm ) {

}

DictionaryEntry::~DictionaryEntry() {

}

QString DictionaryEntry::word() const {
    return m_word;
}

QString DictionaryEntry::phoneme() const {
    return m_phnm;
}

Dictionary* Dictionary::obtain ( const QUuid &l_uuid ) {
    return 0;
}

DictionaryEntryList * Dictionary::entries() const {
    return 0;
}

void Dictionary::addEntry ( DictionaryEntry *l_entry ) {
}

DictionaryEntry * Dictionary::removeEntry ( const QString& p_word ) {
    return m_words.take ( p_word );
}

Dictionary& Dictionary::operator << ( DictionaryEntry *p_entry ) {
    m_words.insert ( p_entry->word(),p_entry );
    return *this;
}

Dictionary& Dictionary::operator << ( DictionaryEntryList& p_lst ) {
    Q_FOREACH ( DictionaryEntry* l_entry, p_lst )
    m_words.insert ( l_entry->word(),l_entry );
    return *this;
}

/// @todo Implement the saving ability.
void Dictionary::save() {

}

Dictionary* Dictionary::fromDirectory ( const QDir& ) {
    return 0;
}

Dictionary::Dictionary ( const Dictionary& p_other ) : QObject(),
    m_words ( p_other.m_words ) {

}

Dictionary::Dictionary ( const QUuid &p_uuid ) {
    load ( p_uuid );
}

Dictionary::~Dictionary() {
}

#include "dictionary.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
