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

#include <QDataStream>

#include "lib/audiosource/sink.hpp"
#include "lib/audiosource/source.hpp"
#include "lib/audiosource/abstract.hxx"

namespace SpeechControl
{

class StreamAudioSourcePrivate;

class StreamAudioSourcePrivate : public AbstractAudioSourcePrivate
{
public:
    StreamAudioSourcePrivate() : AbstractAudioSourcePrivate(), m_strm (0) { }
    StreamAudioSourcePrivate (const AbstractAudioSourcePrivate& p_other) : AbstractAudioSourcePrivate (p_other), m_strm (0) { }
    virtual ~StreamAudioSourcePrivate() {
        m_strm->unsetDevice();
    }

    QDataStream* m_strm;
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;