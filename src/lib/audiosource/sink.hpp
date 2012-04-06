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
#ifndef SPCHCNTRL_LIB_AUDIOSOURCE_SINK_HPP_
#define SPCHCNTRL_LIB_AUDIOSOURCE_SINK_HPP_

#include <QObject>

#include <config.hpp>
#include <export.hpp>

#include <QGst/Utils/ApplicationSink>

namespace SpeechControl
{

class GenericSource;
class GenericSink;
class GenericSinkPrivate;
class StreamSink;
class StreamAudioSource;
class StreamSinkPrivate;

class SPCH_EXPORT GenericSink : public QObject, public QGst::Utils::ApplicationSink
{
    Q_OBJECT
public:
    explicit GenericSink(QObject* p_parent = 0);
    virtual ~GenericSink();
    GenericSource* source();
    void setSource (GenericSource* p_source);

protected:
    Q_DISABLE_COPY (GenericSink)
    Q_DECLARE_PRIVATE (GenericSink)
    virtual void eos();
    virtual QGst::FlowReturn newBuffer();

private:
    QScopedPointer<GenericSinkPrivate> d_ptr;

};

class SPCH_EXPORT StreamSink : public GenericSink
{
    Q_OBJECT
    Q_DISABLE_COPY (StreamSink)
    Q_DECLARE_PRIVATE (StreamSink)

public:
    explicit StreamSink (StreamAudioSource* p_audioSrc);
    StreamSink (const GenericSink& p_other);
    virtual ~StreamSink();
    virtual void eos();
    virtual QGst::BufferPtr pullBuffer();
    uint bufferSize() const;
    void setBufferSize (const uint& p_bufferSize);

private:
    QScopedPointer<StreamSinkPrivate> d_ptr;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
