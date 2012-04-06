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

#include <QGst/Bin>
#include <QGst/Element>
#include <QGst/Pipeline>

namespace SpeechControl
{

class AbstractSphinx;

class GenericSink;
class GenericSource;
class AbstractAudioSource;
class AbstractAudioSourcePrivate;

class AbstractAudioSourcePrivate
{
public:
    AbstractAudioSourcePrivate() : m_appSink (0), m_appSrc (0), m_binPtr (),
        m_pipeline (), m_sinkPtr (), m_srcPtr (), m_volumePtr (), m_levelPtr () {
        m_binPtr.clear();
        m_pipeline.clear();
        m_sinkPtr.clear();
        m_srcPtr.clear();
        m_volumePtr.clear();
        m_levelPtr.clear();
    }

    virtual ~AbstractAudioSourcePrivate() {
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

    GenericSink* m_appSink;         ///< A pointer to the GenericSink used by the AbstractAudioSource.
    GenericSource* m_appSrc;        ///< A pointer to the GenericSource used by the AbstractAudioSource.
    QGst::BinPtr m_binPtr;          ///< A shared pointer to the bin that handles the elements of this AbstractAudioSource.
    QGst::PipelinePtr m_pipeline;   ///< A shared pointer to the pipeline that manipulates the activity state of this AbstractAudioSource.
    QGst::ElementPtr m_sinkPtr;     ///< A shared pointer to the application sink element.
    QGst::ElementPtr m_srcPtr;      ///< A shared pointer to the application source element.
    QGst::ElementPtr m_volumePtr;   ///< A shared pointer to the volume element.
    QGst::ElementPtr m_levelPtr;    ///< A shared pointer to the level element.
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
