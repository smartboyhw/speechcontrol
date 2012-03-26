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
    NoiseDictionary* dict = new NoiseDictionary;

    if (dict->load (p_file))
        return dict;

    return 0;
}

bool NoiseDictionary::hasEntry (const QString& p_entry)
{
    return m_entries.contains (p_entry);
}

/// @todo Populate m_entries with the data from the noise dictionary.
bool NoiseDictionary::load (QIODevice* p_device)
{
    p_device->open (QIODevice::ReadOnly | QIODevice::Text);

    if (!p_device->isOpen()) {
        qDebug() << "[NoiseDictionary::load()] Can't open noise dictionary for reading:" << p_device->errorString();
        m_device = 0;
        m_entries.clear();
        return false;
    }

    if (!p_device->isReadable()) {
        qDebug() << "[NoiseDictionary::load()] Can't read noise dictionary file for loading:" << p_device->errorString();
        m_device = 0;
        m_entries.clear();
        return false;
    }

    m_device = p_device;

    QTextStream strm (m_device);

    while (!strm.atEnd()) {
        QString line = strm.readLine();
        QString key;
        QString value;
        bool isInSpace = false;
        QChar lastChar;

        Q_FOREACH(const QChar charVal, line){
            const bool isBeginningKey = key.isEmpty() && value.isEmpty() && !charVal.isSpace();
            const bool isBeginningValue = !key.isEmpty() && value.isEmpty() && !charVal.isSpace() && lastChar.isSpace();

            if (isBeginningKey)
                key += charVal;
            else if (isBeginningValue)
                value += charVal;
            else {
                if (charVal.isSpace()){
                    isInSpace = true;
                }
                else {
                    if (isInSpace){
                        value += charVal;
                    } else {
                        key += charVal;
                    }
                }
            }

            lastChar = charVal;
        }

        m_entries.insert(key,value);
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
        qDebug() << "[NoiseDictionary::save()] Can't open noise dictionary for saving:" << m_device->errorString();
        return;
    }

    if (!m_device->isWritable()) {
        qDebug() << "[NoiseDictionary::save()] Can't write to noise dictionary file" << m_device->errorString();
        return;
    }

    int size = 1;

    for (QStringMap::iterator l_itr = m_entries.begin();
            l_itr != m_entries.end(); l_itr++) {
        if (size < l_itr.key().size())
            size = l_itr.key().size() + 5;
    }

    QTextStream strm (m_device);
    const QString padSpace = QString (size, ' ');

    for (QStringMap::iterator itr = m_entries.begin();
            itr != m_entries.end(); itr++) {
        strm << itr.key() << padSpace << itr.value() << "\n";
    qDebug() << "[NoiseDictionary::load()] Saving entry" << itr.key() << "with the value" << itr.value();
    }

    m_device->close();
}

bool NoiseDictionary::isValid() const
{
    return m_device && !m_entries.isEmpty();
}

#include "noisedictionary.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
