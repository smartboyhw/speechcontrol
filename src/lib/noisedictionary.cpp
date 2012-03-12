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

#include <QPair>
#include <QDebug>
#include <QTextStream>

#include "noisedictionary.hpp"

using namespace SpeechControl;

NoiseDictionary::NoiseDictionary (QObject* p_parent) : QObject (p_parent)
{

}

NoiseDictionary::NoiseDictionary (const NoiseDictionary& p_other) : QObject (p_other.parent()), m_entries (p_other.m_entries),
    m_device (p_other.m_device)
{

}

void NoiseDictionary::addEntry (const QString& p_entry, const QString& p_value)
{
    m_entries.insert (p_entry, p_value);
}

QStringMap NoiseDictionary::entries()
{
    return m_entries;
}

NoiseDictionary* NoiseDictionary::fromFile (QFile* p_file)
{
    NoiseDictionary* l_dict = new NoiseDictionary;

    if (l_dict->load (p_file))
        return l_dict;
    else
        return false;
}

bool NoiseDictionary::hasEntry (const QString& p_entry)
{
    return m_entries.contains (p_entry);
}

bool NoiseDictionary::load (QIODevice* p_device)
{
    p_device->open (QIODevice::ReadOnly | QIODevice::Text);

    if (!p_device->isOpen()) {
        qDebug() << "Can't open noise dictionary" << p_device->errorString();
        return false;
    }

    if (!p_device->isReadable()) {
        qDebug() << "Can't read noise dictionary file" << p_device->errorString();
        return false;
    }

    m_device = p_device;

    QTextStream l_strm (m_device);

    while (!l_strm.atEnd()) {
        QString l_line = l_strm.readLine();
    }

    m_device->close();
    return true;
}

void NoiseDictionary::mergeEntries (const QStringMap& p_entries)
{
    m_entries = m_entries.unite (p_entries);
}

void NoiseDictionary::save()
{
    m_device->open (QIODevice::WriteOnly | QIODevice::Truncate);

    if (!m_device->isOpen()) {
        qDebug() << "Can't open noise dictionary" << m_device->errorString();
        return;
    }

    if (!m_device->isWritable()) {
        qDebug() << "Can't read noise dictionary file" << m_device->errorString();
        return;
    }

    int l_size = 0;

    for (QStringMap::iterator l_itr = m_entries.begin();
            l_itr != m_entries.end(); l_itr++) {
        if (l_size < l_itr.key().size())
            l_size = l_itr.key().size() + 5;
    }

    QTextStream l_strm (m_device);
    const QString l_padSpace = QString (l_size, ' ');

    for (QStringMap::iterator l_itr = m_entries.begin();
            l_itr != m_entries.end(); l_itr++) {
        l_strm << l_itr.key() << l_padSpace << l_itr.value() << "\n";
    }

    m_device->close();
}

#include "noisedictionary.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
