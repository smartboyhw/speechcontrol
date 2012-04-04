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

#include "lib/audiosource/abstract.hpp"
#include "lib/audiosource/sink.hpp"
#include "lib/audiosource/source.hpp"
#include "lib/audiosource/stream.hpp"

using namespace SpeechControl;

GenericSource::GenericSource (AbstractAudioSource* p_audioSource) : m_audioSrc (p_audioSource)
{
    setParent (p_audioSource);
    qDebug() << "[GenericSource::{constructor}] Built source for " << m_audioSrc->pipelineStr() << ".";
}

GenericSource::GenericSource (const GenericSource& p_other) : QObject (p_other.parent()), ApplicationSource(), m_audioSrc (p_other.m_audioSrc)
{

}

QGst::FlowReturn GenericSource::endOfStream()
{
    return QGst::FlowOk;
}

QGst::FlowReturn GenericSource::pushBuffer (const QGst::BufferPtr& p_buffer)
{
    quint8* bufferInt = p_buffer->data();
    QByteArray buffer;
    buffer.resize (1);
    buffer[0] = (qint8) * bufferInt;
    emit bufferObtained (buffer);

    //qDebug() << "[GenericSource::pushBuffer()] Buffer obtained from AbstractAudioSource" << p_buffer->data() << bufferInt << buffer;

    return QGst::Utils::ApplicationSource::pushBuffer (p_buffer);
}

GenericSource::~GenericSource()
{
    if (!element().isNull())
        element()->setState (QGst::StateNull);
}

StreamSource::StreamSource (StreamAudioSource* p_audioSource) : GenericSource (p_audioSource)
{

}

StreamSource::StreamSource (const StreamSource& p_source) : GenericSource (p_source)
{

}

QGst::FlowReturn StreamSource::endOfStream()
{
    return GenericSource::endOfStream();
}

QGst::FlowReturn StreamSource::pushBuffer (const QGst::BufferPtr& p_buffer)
{
    qDebug() << "[StreamSource::pushBuffer()] Buffer:" << p_buffer->data();
    return GenericSource::pushBuffer (p_buffer);
}

StreamSource::~StreamSource()
{

}

AudioSourceSphinxSource::AudioSourceSphinxSource (AudioSourceSphinx* p_sphinx) : QObject(), m_sphinx (p_sphinx)
{
}

#include "audiosource/source.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
