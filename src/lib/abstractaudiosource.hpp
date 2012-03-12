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

namespace SpeechControl {
class GenericSink;
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

class SPCH_EXPORT GenericSink : public QGst::Utils::ApplicationSink {
public:
    explicit GenericSink();
    QGst::Utils::ApplicationSource* source();
    void setSource ( QGst::Utils::ApplicationSource* p_source );

protected:
    virtual void eos();
    virtual QGst::FlowReturn newBuffer();

private:
    QGst::Utils::ApplicationSource *m_src;
};

/**
 * @brief Represents a handle of an audio input device on this computer.
 *
 * The AbstractAudioSource class allows developers to not worry too much about handling
 * audio devices for SpeechControl. It automatically obtains a listing of all
 * input devices in the system and allows this to be attached to other classes
 * that'd require the use of an audio input device like AbstractSphinx.
 **/
class SPCH_EXPORT AbstractAudioSource : public QObject {
    Q_OBJECT
    QGst::PipelinePtr m_pipeline;

public:
    virtual ~AbstractAudioSource();
    bool isRecording() const;

signals:
    void recordingBegun();
    void recordingEnded();

public slots:
    void startRecording();
    void stopRecording();

protected:
    explicit AbstractAudioSource ( QObject* parent = 0 );
    AbstractAudioSource ( const QObject& p_other );
    Q_DISABLE_COPY ( AbstractAudioSource )
    virtual QString pipelineDescription() = 0;
    QString caps() const;
    virtual void buildPipeline();
    GenericSink* m_sink;
    QGlib::Value m_device;
    QGst::BinPtr m_binAudio;
    QGst::ElementPtr m_sinkAudio;
    QGst::ElementPtr m_srcAudio;
    QGst::ElementPtr m_srcVolume;

private:
    void onPipelineBusmessage ( const QGst::MessagePtr & message );
};

class SPCH_EXPORT DeviceAudioSource : public AbstractAudioSource {
    Q_OBJECT
    Q_DISABLE_COPY ( DeviceAudioSource )

public:
    explicit DeviceAudioSource();
    DeviceAudioSource ( const QString& p_deviceName );
    DeviceAudioSource ( const AbstractAudioSource& p_other );
    virtual ~DeviceAudioSource();
    QString deviceName() const;
    static AbstractAudioSourceList allDevices();
    static DeviceAudioSource* defaultDevice();

protected:
    virtual QString pipelineDescription();
    virtual void buildPipeline();

private:
    void obtainDevice ( const QString& p_deviceName );
    QGst::ElementPtr m_devicePtr;
};

class SPCH_EXPORT StreamAudioSource : public AbstractAudioSource {


};

}

#endif // ABSTRACTAUDIOSOURCE_HPP


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
