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

#include "lib/sphinx/abstract.hxx"
#include "lib/sphinx/audiosource.hpp"

namespace SpeechControl
{

class AudioSourceSphinx;

class AbstractSphinxPrivate;
class AudioSourceSphinxPrivate;

class AudioSourceSphinxPrivate : public AbstractSphinxPrivate
{
public:
    explicit AudioSourceSphinxPrivate (AudioSourceSphinx* p_qPtr) : AbstractSphinxPrivate(p_qPtr), m_audioSrc (0), m_appSrc (0){ }
    virtual ~AudioSourceSphinxPrivate() {

    }

    void linkSource ();
    AbstractAudioSource* m_audioSrc;
    AudioSourceSphinxSource* m_appSrc;

};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
