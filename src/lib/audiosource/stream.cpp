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

#include "lib/audiosource/sink.hpp"
#include "lib/audiosource/abstract.hpp"
#include "lib/audiosource/stream.hxx"
#include "lib/audiosource/stream.hpp"

using namespace SpeechControl;

StreamAudioSource::StreamAudioSource() : AbstractAudioSource(new StreamAudioSourcePrivate)
{

}

StreamAudioSource::StreamAudioSource (const AbstractAudioSource& p_other) : AbstractAudioSource (p_other)
{

}

StreamAudioSource::StreamAudioSource (const StreamAudioSource& p_other) : AbstractAudioSource (p_other)
{
}

StreamAudioSource::StreamAudioSource (QDataStream* p_stream) : AbstractAudioSource (new StreamAudioSourcePrivate)
{
    Q_D(StreamAudioSource);
    d->m_strm = p_stream;
}

void StreamAudioSource::buildPipeline()
{
    AbstractAudioSource::buildPipeline();

    if (isNull()) {
        qDebug() << "[StreamAudioSource::buildPipeline()] Failed to render stream, invalid base pipeline.";
    }

    // Replace the appsrc used by AbstractAudioSource usually with the stream source.
    d_func()->m_appSrc = new StreamSource (this);
    d_func()->m_appSrc->setCaps (QGst::Caps::fromString (caps()));
    d_func()->m_appSrc->setElement (d_func()->m_srcPtr);

    d_func()->m_appSink = new StreamSink (this);
    d_func()->m_appSink->setCaps (QGst::Caps::fromString (caps()));
    d_func()->m_appSink->setElement (d_func()->m_sinkPtr);
}

QString StreamAudioSource::pipelineDescription() const
{
    return QString ("appsrc name=src ! decodebin");
}

QDataStream* StreamAudioSource::stream() const
{
    Q_D(const StreamAudioSource);
    return d->m_strm;
}

StreamAudioSource::~StreamAudioSource()
{

}
#include "audiosource/stream.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
