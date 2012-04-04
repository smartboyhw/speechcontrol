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
#ifndef SPCHCNTRL_LIB_AUDIOSOURCE_STREAM_HPP_
#define SPCHCNTRL_LIB_AUDIOSOURCE_STREAM_HPP_

#include <lib/audiosource/source.hpp>

namespace SpeechControl
{

/**
 * @brief Represents an audio source coming from data from a data stream.
 *
 * At times, developers might find it convenient to use arbitrary data streams
 * to pipe data to a listening source as if it were to be a local device providing
 * said audio. StreamAudioSource allows the piping of data from a QDataStream into
 * whatever chooses to listen.
 **/
class SPCH_EXPORT StreamAudioSource : public AbstractAudioSource
{
    Q_OBJECT
    Q_DISABLE_COPY (StreamAudioSource)
    friend class StreamSink;
    friend class StreamSource;

public:
    explicit StreamAudioSource ();
    StreamAudioSource (QDataStream* p_stream);
    StreamAudioSource (const AbstractAudioSource& p_other);
    virtual ~StreamAudioSource();
    QDataStream* stream() const;

protected:
    virtual QString pipelineDescription() const;
    virtual void buildPipeline();

private:
    QDataStream* m_strm;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
