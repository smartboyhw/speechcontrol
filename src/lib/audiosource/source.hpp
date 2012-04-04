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
#ifndef SPCHCNTRL_LIB_AUDIOSOURCE_SOURCE_HPP_
#define SPCHCNTRL_LIB_AUDIOSOURCE_SOURCE_HPP_

#include <QObject>
#include <QByteArray>

#include <QGst/Buffer>

#include <lib/config.hpp>
#include <lib/export.hpp>

#include <QGst/Utils/ApplicationSource>

namespace SpeechControl
{

class AudioSourceSphinx;
class AbstractAudioSource;
class StreamAudioSource;
class GenericSink;

class SPCH_EXPORT GenericSource : public QObject, public QGst::Utils::ApplicationSource
{
    Q_OBJECT

signals:
    void bufferObtained (const QByteArray p_bufferData);

public:
    explicit GenericSource (AbstractAudioSource* p_audioSource);
    virtual ~GenericSource();
    virtual QGst::FlowReturn endOfStream();
    virtual QGst::FlowReturn pushBuffer (const QGst::BufferPtr& p_buffer);

protected:
    Q_DISABLE_COPY (GenericSource)
    AbstractAudioSource* m_audioSrc;
};

class SPCH_EXPORT StreamSource : public GenericSource
{
    Q_OBJECT
    Q_DISABLE_COPY (StreamSource)

public:
    explicit StreamSource (StreamAudioSource* p_audioSource);
    virtual ~StreamSource();
    virtual QGst::FlowReturn endOfStream();
    virtual QGst::FlowReturn pushBuffer (const QGst::BufferPtr& p_buffer);
};

class AudioSourceSphinxSource : public QObject, public QGst::Utils::ApplicationSource
{
    Q_OBJECT
    Q_DISABLE_COPY (AudioSourceSphinxSource)

public:
    AudioSourceSphinxSource (AudioSourceSphinx* p_sphinx);

private:
    AudioSourceSphinx* m_sphinx;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
