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

#include "lib/audiosource/abstract.hxx"

using namespace SpeechControl;

AbstractAudioSourcePrivate::AbstractAudioSourcePrivate() : m_appSink (0),
    m_appSrc (0), m_binPtr (), m_pipeline (), m_sinkPtr (),
    m_srcPtr (), m_volumePtr (), m_levelPtr ()
{
    m_binPtr.clear();
    m_pipeline.clear();
    m_sinkPtr.clear();
    m_srcPtr.clear();
    m_volumePtr.clear();
    m_levelPtr.clear();
}

AbstractAudioSourcePrivate::~AbstractAudioSourcePrivate()
{
    // Clean up your junk!
    if (!m_binPtr.isNull())
        m_binPtr->setState (QGst::StateNull);

    if (!m_pipeline.isNull())
        m_pipeline->setState (QGst::StateNull);

    if (!m_sinkPtr.isNull())
        m_sinkPtr->setState (QGst::StateNull);

    if (!m_srcPtr.isNull())
        m_srcPtr->setState (QGst::StateNull);

    if (!m_volumePtr.isNull())
        m_volumePtr->setState (QGst::StateNull);

    if (!m_levelPtr.isNull())
        m_levelPtr->setState (QGst::StateNull);

    // Clean memory.
    m_binPtr.clear();
    m_pipeline.clear();
    m_sinkPtr.clear();
    m_srcPtr.clear();
    m_volumePtr.clear();
    m_levelPtr.clear();
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
