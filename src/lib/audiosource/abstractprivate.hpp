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

struct AbstractAudioSourcePrivate {
public:
    explicit AbstractAudioSourcePrivate();
    virtual ~AbstractAudioSourcePrivate();

    GenericSink* appSink;         ///< A pointer to the GenericSink used by the AbstractAudioSource.
    GenericSource* appSource;        ///< A pointer to the GenericSource used by the AbstractAudioSource.
    QGst::BinPtr ptrBin;          ///< A shared pointer to the bin that handles the elements of this AbstractAudioSource.
    QGst::PipelinePtr m_pipeline;   ///< A shared pointer to the pipeline that manipulates the activity state of this AbstractAudioSource.
    QGst::ElementPtr ptrAudioSink;     ///< A shared pointer to the application sink element.
    QGst::ElementPtr ptrAudioSource;      ///< A shared pointer to the application source element.
    QGst::ElementPtr ptrVolume;   ///< A shared pointer to the volume element.
    QGst::ElementPtr ptrLevel;    ///< A shared pointer to the level element.
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
