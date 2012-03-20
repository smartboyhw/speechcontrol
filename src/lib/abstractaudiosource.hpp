/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef ABSTRACTAUDIOSOURCE_HPP
#define ABSTRACTAUDIOSOURCE_HPP

#include <QMap>
#include <QList>
#include <QUuid>
#include <QObject>

#include <QGlib/Value>
#include <QGst/Bin>
#include <QGst/Pad>
#include <QGst/Global>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Element>
#include <QGst/Pipeline>
#include <QGst/ChildProxy>
#include <QGst/ElementFactory>
#include <QGst/PropertyProbe>
#include <QGst/StreamVolume>

#include <export.hpp>
#include <QGst/Utils/ApplicationSource>
#include <QGst/Utils/ApplicationSink>

namespace SpeechControl
{
class GenericSink;
class GenericSource;
class AbstractAudioSource;
class DeviceAudioSource;
class StreamAudioSource;

/**
 * @brief Represents a shorthand for denoating a list of @see AbstractAudioSource.
 **/
typedef QList<AbstractAudioSource*> AbstractAudioSourceList;

/**
 * @brief Represents a named mapping of AbstractAudioSources.
 **/
typedef QMap<QUuid, AbstractAudioSource*> AbstractAudioSourceMap;

class SPCH_EXPORT GenericSink : public QGst::Utils::ApplicationSink
{
public:
    explicit GenericSink();
    virtual ~GenericSink();
    GenericSource* source();
    void setSource (SpeechControl::GenericSource* p_source);

protected:
    virtual void eos();
    virtual QGst::FlowReturn newBuffer();

private:
    GenericSource* m_src;
};

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

private:
    AbstractAudioSource* m_audioSrc;
};

/**
 * @brief Represents a handle of an audio input device on this computer.
 *
 * The AbstractAudioSource class allows developers to not worry too much about handling
 * audio devices for SpeechControl. It automatically obtains a listing of all
 * input devices in the system and allows this to be attached to other classes
 * that'd require the use of an audio input device like AbstractSphinx.
 **/
class SPCH_EXPORT AbstractAudioSource : public QObject
{
    Q_OBJECT
    friend class GenericSource;

public:
    virtual ~AbstractAudioSource();
    bool isRecording() const;
    bool isNull() const;
    bool isMuted() const;
    double volume() const;
    void setMuted (const bool p_muted);
    void setVolume (const double p_volume);

signals:
    void recordingBegun();
    void recordingEnded();
    void bufferObtained(const QByteArray p_buffer);

public slots:
    void startRecording();
    void stopRecording();

protected:
    Q_DISABLE_COPY (AbstractAudioSource)
    explicit AbstractAudioSource (QObject* parent = 0);
    AbstractAudioSource (const QObject& p_other);
    virtual QString pipelineDescription() const = 0;
    QString caps() const;
    QString pipelineStr() const;
    virtual void buildPipeline();
    GenericSink* m_appSink;
    GenericSource* m_appSrc;
    QGst::BinPtr m_binPtr;
    QGst::PipelinePtr m_pipeline;
    QGst::ElementPtr m_sinkPtr;
    QGst::ElementPtr m_srcPtr;
    QGst::ElementPtr m_volumePtr;
    QGst::ElementPtr m_levelPtr;

private slots:
    void onPipelineBusmessage (const QGst::MessagePtr& message);
};

class SPCH_EXPORT DeviceAudioSource : public AbstractAudioSource
{
    Q_OBJECT
    Q_DISABLE_COPY (DeviceAudioSource)

public:
    explicit DeviceAudioSource();
    DeviceAudioSource (const AbstractAudioSource& p_other);
    virtual ~DeviceAudioSource();
    QString deviceName() const;
    QString humanName() const;
    static AbstractAudioSourceList allDevices();
    static DeviceAudioSource* defaultDevice();
    static DeviceAudioSource* obtain(const QString& p_deviceName);

protected:
    DeviceAudioSource (const QString& p_deviceName);
    virtual QString pipelineDescription() const;
    virtual void buildPipeline();

private:
    void obtainDevice (const QString& p_deviceName);
    QGlib::Value m_device;
    QGst::ElementPtr m_devicePtr;
    static QMap<QString,DeviceAudioSource*> s_map;
};

class SPCH_EXPORT StreamAudioSource : public AbstractAudioSource
{
    Q_OBJECT
    Q_DISABLE_COPY (StreamAudioSource)

public:
    explicit StreamAudioSource ();
    StreamAudioSource (QDataStream& p_stream);
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

#endif // ABSTRACTAUDIOSOURCE_HPP


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
