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

#include <QDebug>

#include "lib/audiosource/abstract.hpp"
#include "lib/audiosource/stream.hpp"
#include "lib/audiosource/source.hpp"
#include "lib/audiosource/sink.hpp"

using namespace SpeechControl;

GenericSink::GenericSink() : m_src (0)
{

}

GenericSink::GenericSink (const GenericSink& p_other) : QObject (p_other.parent()), ApplicationSink(), m_src (p_other.m_src)
{

}

void GenericSink::eos()
{
    qDebug() << "[GenericSink::eos()] End of stream in generic sink.";
    m_src->endOfStream();
    QGst::Utils::ApplicationSink::eos();
}

QGst::FlowReturn GenericSink::newBuffer()
{
    QGst::BufferPtr buffer = pullBuffer();

    //qDebug() << "[GenericSink::newBuffer()] Buffer: " << * (buffer->data());

    return m_src->pushBuffer (buffer);
}

void GenericSink::setSource (GenericSource* p_source)
{
    qDebug() << "[GenericSink::setSource()] Set a new source." << p_source->caps()->toString();
    m_src = p_source;
}

GenericSource* GenericSink::source()
{
    return m_src;
}

GenericSink::~GenericSink()
{
    if (!element().isNull())
        element()->setState (QGst::StateNull);
}

StreamSink::StreamSink (StreamAudioSource* p_audioSrc) : GenericSink(), m_audioSrc (p_audioSrc)
{

}

StreamSink::StreamSink (const StreamSink& p_other) : GenericSink (p_other)
{

}

StreamSink::StreamSink (const GenericSink& p_other) : GenericSink (p_other)
{

}

void StreamSink::eos()
{
    qDebug() << "[StreamSink::eos()] End-of-stream detected.";
    SpeechControl::GenericSink::eos();
}

QGst::BufferPtr StreamSink::pullBuffer()
{
    quint8 data = 0;
    *(m_audioSrc->stream()) >> data;
    qDebug() << "[StreamSink::pullBuffer()] Obtained buffer from stream: " << data;
    return QGst::Buffer::create (data);
}

StreamSink::~StreamSink()
{

}

#include "audiosource/sink.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
