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
#include <QString>

class QIODevice;

namespace SpeechControl
{

typedef QMap<QString, QString> QStringMap;
struct NoiseDictionaryPrivate;

struct NoiseDictionaryPrivate {
    NoiseDictionaryPrivate() : m_entries(), m_device (0) { }
    QStringMap m_entries;       ///< Holds the entries.
    QIODevice* m_device;        ///< Holds the device for saving purposes.
};
}
// kate: indent-mode cstyle; replace-tabs on; 
