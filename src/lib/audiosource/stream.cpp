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

#include "audiosource/sink.hpp"
#include "audiosource/streamprivate.hpp"
#include "audiosource/abstract.hpp"
#include "audiosource/stream.hpp"

using namespace SpeechControl;

StreamAudioSourcePrivate::StreamAudioSourcePrivate() :
    AbstractAudioSourcePrivate(), m_strm (0)
{

}

StreamAudioSourcePrivate::~StreamAudioSourcePrivate()
{

}

StreamAudioSource::StreamAudioSource() :
    AbstractAudioSource (new StreamAudioSourcePrivate)
{
}

StreamAudioSource::StreamAudioSource (const AbstractAudioSource& p_other) :
    AbstractAudioSource (p_other)
{

}

StreamAudioSource::StreamAudioSource (const StreamAudioSource& p_other) :
    AbstractAudioSource (p_other)
{
}

StreamAudioSource::StreamAudioSource (QDataStream* p_stream) :
    AbstractAudioSource (new StreamAudioSourcePrivate)
{
    Q_D (StreamAudioSource);
    d->m_strm = p_stream;
}

void StreamAudioSource::buildPipeline()
{
    Q_D (StreamAudioSource);
    AbstractAudioSource::buildPipeline();

    if (isNull()) {
        qDebug() << "[StreamAudioSource::buildPipeline()] Failed to render stream, invalid base pipeline.";
    }

    d->appSource = new StreamSource (this);
    d->appSource->setCaps (QGst::Caps::fromString (caps()));
    d->appSource->setElement (d->ptrAudioSource);

    d->appSink = new StreamSink (this);
    d->appSink->setCaps (QGst::Caps::fromString (caps()));
    d->appSink->setElement (d->ptrAudioSink);
}

QString StreamAudioSource::pipelineDescription() const
{
    return QString ("appsrc name=src ! decodebin");
}

QDataStream* StreamAudioSource::stream() const
{
    Q_D (const StreamAudioSource);
    return d->m_strm;
}

StreamAudioSource::~StreamAudioSource()
{

}
#include "audiosource/stream.moc"
// kate: indent-mode cstyle; replace-tabs on; 
