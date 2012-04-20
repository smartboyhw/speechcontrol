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

#include "lib/audiosource/source.hpp"
#include "lib/sphinx/abstract.hpp"
#include "lib/sphinx/audiosource.hpp"
#include "lib/sphinx/audiosourceprivate.hpp"

using namespace SpeechControl;

AudioSourceSphinxPrivate::AudioSourceSphinxPrivate (AudioSourceSphinx* p_qPtr) :
    AbstractSphinxPrivate (p_qPtr), m_appSrc (0)
{

}

void AudioSourceSphinxPrivate::linkSource ()
{
    Q_Q (AudioSourceSphinx);
    QString description = q->standardDescription();
    description = description.replace ("autoaudiosrc name=src", "appsrc name=src");
    q->buildPipeline (description);
    m_appSrc = new AudioSourceSphinxSource (q);
    qDebug() << "[AudioSourceSphinx::linkSource()] Linked up sources.";
}

AudioSourceSphinxPrivate::~AudioSourceSphinxPrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
