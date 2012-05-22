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

#include "audiosource/streamprivate.hpp"
#include "audiosource/abstract.hpp"
#include "audiosource/stream.hpp"
#include "audiosource/source.hpp"
#include "audiosource/sinkprivate.hpp"
#include "audiosource/sink.hpp"

using namespace SpeechControl;

GenericSink::GenericSink (QObject* p_parent) : QObject (p_parent),
    ApplicationSink(), d_ptr (new GenericSinkPrivate)
{

}

GenericSink::GenericSink (const GenericSink& p_other) : QObject (p_other.parent()),
    ApplicationSink(), d_ptr (new GenericSinkPrivate)
{

}

void GenericSink::eos()
{
    Q_D (GenericSink);
    qDebug() << "[GenericSink::eos()] End of stream in generic sink.";
    d->m_src->endOfStream();
    QGst::Utils::ApplicationSink::eos();
}

QGst::FlowReturn GenericSink::newBuffer()
{
    Q_D (GenericSink);

    if (!isEos()) {
        QGst::BufferPtr buffer = pullBuffer();

        qDebug() << "[GenericSink::newBuffer()] Buffer: " << *(buffer->data())
                 << buffer->size() << buffer->timeStamp();

        return d->m_src->pushBuffer (buffer);
    }
    else {
        qDebug() << "[GenericSink::newBuffer()] Sink is at end-of-stream!";
        return QGst::FlowError;
    }
}

void GenericSink::setSource (GenericSource* p_source)
{
    qDebug() << "[GenericSink::setSource()] Set a new source." << p_source->caps()->toString();
    d_func()->m_src = p_source;
}

GenericSource* GenericSink::source()
{
    return d_func()->m_src;
}

GenericSink::~GenericSink()
{
    if (!element().isNull())
        element()->setState (QGst::StateNull);
}

StreamSink::StreamSink (StreamAudioSource* p_audioSrc) :
    GenericSink(), d_ptr (new StreamSinkPrivate)
{
    d_func()->m_audioSrc = p_audioSrc;
}

StreamSink::StreamSink (const StreamSink& p_other) :
    GenericSink (p_other), d_ptr (const_cast<StreamSinkPrivate*> (p_other.d_func()))
{

}

StreamSink::StreamSink (const GenericSink& p_other) :
    GenericSink (p_other), d_ptr (new StreamSinkPrivate)
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
    * (d_func()->m_audioSrc->stream()) >> data;
    qDebug() << "[StreamSink::pullBuffer()] Obtained buffer from stream: " << data;
    return QGst::Buffer::create (data);
}

uint StreamSink::bufferSize() const
{
    QGlib::Value bufferSizeVal = d_func()->m_audioSrc->d_func()->ptrAudioSource->property ("blocksize");
    const int bufferSize = bufferSizeVal.toUInt();

    if (bufferSize == -1)
        return 4096;
    else
        return (const uint) bufferSize;
}

void StreamSink::setBufferSize (const uint& p_bufferSize)
{
    d_func()->m_audioSrc->d_func()->ptrAudioSource->setProperty ("blocksize", p_bufferSize);
}

StreamSink::~StreamSink()
{

}

#include "audiosource/sink.moc"
// kate: indent-mode cstyle; replace-tabs on; 
