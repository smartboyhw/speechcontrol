/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  d_func()->modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for d_func()->more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QPair>
#include <QDebug>
#include <QTextStream>

#include "noisedictionaryprivate.hpp"
#include "noisedictionary.hpp"

using namespace SpeechControl;

NoiseDictionary::NoiseDictionary (QObject* p_parent) : QObject (p_parent), d_ptr (new NoiseDictionaryPrivate)
{
}

NoiseDictionary::NoiseDictionary (const NoiseDictionary& p_other) : QObject (p_other.parent()), d_ptr (const_cast<NoiseDictionaryPrivate*> (p_other.d_ptr.data()))
{
}

void NoiseDictionary::addEntry (const QString& p_entry, const QString& p_value)
{
    d_func()->m_entries.insert (p_entry, p_value);
}

QStringMap NoiseDictionary::entries()
{
    return d_func()->m_entries;
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
    return entries().contains (p_entry);
}

/// @todo Populate d_func()->m_entries with the data from the noise dictionary.
bool NoiseDictionary::load (QIODevice* p_device)
{
    p_device->open (QIODevice::ReadOnly | QIODevice::Text);

    if (!p_device->isOpen()) {
        qDebug() << "[NoiseDictionary::load()] Can't open noise dictionary for reading:" << p_device->errorString();
        d_func()->m_device = 0;
        d_func()->m_entries.clear();
        return false;
    }

    if (!p_device->isReadable()) {
        qDebug() << "[NoiseDictionary::load()] Can't read noise dictionary file for loading:" << p_device->errorString();
        d_func()->m_device = 0;
        d_func()->m_entries.clear();
        return false;
    }

    d_func()->m_device = p_device;

    QTextStream strm (d_func()->m_device);

    while (!strm.atEnd()) {
        QString line = strm.readLine();
        QString key;
        QString value;
        bool isInSpace = false;
        QChar lastChar;

        Q_FOREACH (const QChar charVal, line) {
            const bool isBeginningKey = key.isEmpty() && value.isEmpty() && !charVal.isSpace();
            const bool isBeginningValue = !key.isEmpty() && value.isEmpty() && !charVal.isSpace() && lastChar.isSpace();

            if (isBeginningKey)
                key += charVal;
            else if (isBeginningValue)
                value += charVal;
            else {
                if (charVal.isSpace()) {
                    isInSpace = true;
                }
                else {
                    if (isInSpace) {
                        value += charVal;
                    }
                    else {
                        key += charVal;
                    }
                }
            }

            lastChar = charVal;
        }

        d_func()->m_entries.insert (key, value);
    }

    d_func()->m_device->close();
    return true;
}

void NoiseDictionary::mergeEntries (const QStringMap& p_entries)
{
    d_func()->m_entries = entries().unite (p_entries);
}

void NoiseDictionary::save()
{
    d_func()->m_device->open (QIODevice::WriteOnly | QIODevice::Truncate);

    if (!d_func()->m_device->isOpen()) {
        qDebug() << "[NoiseDictionary::save()] Can't open noise dictionary for saving:" << d_func()->m_device->errorString();
        return;
    }

    if (!d_func()->m_device->isWritable()) {
        qDebug() << "[NoiseDictionary::save()] Can't write to noise dictionary file" << d_func()->m_device->errorString();
        return;
    }

    int size = 1;

    Q_FOREACH (const QString key, entries().values()) {
        if (size < key.size())
            size = key.size() + 5;
    }

    QTextStream strm (d_func()->m_device);
    const QString padSpace = QString (size, ' ');

    for (QStringMap::iterator itr = d_func()->m_entries.begin();
            itr != d_func()->m_entries.end(); itr++) {
        strm << itr.key() << padSpace << itr.value() << "\n";
        qDebug() << "[NoiseDictionary::load()] Saving entry" << itr.key() << "with the value" << itr.value();
    }

    d_func()->m_device->close();
}

bool NoiseDictionary::isValid() const
{
    return d_func()->m_device && !d_func()->m_entries.isEmpty();
}

#include "noisedictionary.moc"

// kate: indent-mode cstyle; replace-tabs on; 
