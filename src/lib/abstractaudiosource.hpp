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

#include <config.hpp>
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

class SPCH_EXPORT GenericSink : public QObject, public QGst::Utils::ApplicationSink
{
    Q_OBJECT
public:
    explicit GenericSink();
    virtual ~GenericSink();
    GenericSource* source();
    void setSource (GenericSource* p_source);

protected:
    Q_DISABLE_COPY (GenericSink)
    virtual void eos();
    virtual QGst::FlowReturn newBuffer();
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

protected:
    Q_DISABLE_COPY (GenericSource)
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
    /**
     * @brief Destructor.
     **/
    virtual ~AbstractAudioSource();

    /**
     * @brief Determines if the AbstractAudioSource is active.
     *
     * @return TRUE if the AbstractAudioSource is currently active.
     **/
    bool isActive() const;

    /**
     * @brief Determines if the AbstractAudioSource is properly formed.
     *
     * @return TRUE if the building of the internal pipeline succeeded and all of the elements were formed.
     *         FALSE if that operation failed.
     **/
    bool isNull() const;

    /**
     * @brief Determines if the AbstractAudioSource is muted.
     *
     * @return TRUE if the mute state of this AbstractAudioSource is true.
     * @note This method always returns false if isNull() returns true.
     **/
    bool isMuted() const;

    /**
     * @brief Obtains the current volume of the AbstractAudioSource.
     *
     * @return The volume of the stream within AbstractAudioSource.
     *         If isNull() == true, this returns -1.0.
     **/
    double volume() const;

    /**
     * @brief Sets the mute state of the AbstractAudioSource to p_muted.
     *
     * @param p_muted The new mute state of the AbstractAudioSource.
     * @note If isNull() == true, this method does nothing.
     **/
    void setMuted (const bool p_muted);

    /**
     * @brief Sets the volume of the AbstractAudioSource of p_volume.
     *
     * @param p_volume The volume to use. (a range of 0.0 to 1.0, where 1.0 = 100%)
     * @note If isNull() == true, this method does nothing.
     **/
    void setVolume (const double p_volume);

signals:
    /**
     * @brief Emitted when this AbstractAudioSource begins activity.
     **/
    void begun();

    /**
     * @brief Emitted when this AbstractAudioSource ends activity.
     *
     **/
    void ended();

    /**
     * @brief Emitted whenever a new buffer has been generated within this AbstractAudioSource.
     *
     * @param p_buffer A buffer representing a single quint8 (unsigned 8-bit integer or unsigned char).
     **/

    void bufferObtained (const QByteArray& p_buffer);

public slots:
    /**
     * @brief Starts activity on this AbstractAudioSource.
     * @see begun()
     **/
    virtual void start();

    /**
     * @brief Ends activity on this AbstractAudioSource.
     * @see ended()
     **/
    virtual void stop();

protected:
    Q_DISABLE_COPY (AbstractAudioSource)

    /**
     * @brief Null constructor.
     *
     * @param parent The parent QObject. Defaults to 0.
     **/
    explicit AbstractAudioSource (QObject* p_parent = 0);

    /**
     * @brief Copy constructor.
     *
     * @param p_other The QObject to base this on.
     **/
    AbstractAudioSource (const QObject& p_other);

    /**
     * @brief Obtains the portion of a pipeline needed for a derived AbstractAudioSource to use.
     *
     * This method is implemented in derived classes to define specifically what
     * elements for GStreamer are needed for the adequate construction of the pipeline.
     *
     * @return A string representing the added portion of the description.
     **/
    virtual QString pipelineDescription() const = 0;

    /**
     * @brief Obtains a QString representing the recommend capabilities of sinks and sources.
     **/
    QString caps() const;

    /**
     * @brief Obtains a QString generating the resulting pipeline string.
     * @see pipelineDescription()
     **/
    QString pipelineStr() const;

    /**
     * @brief Builds the internal pipeline.
     *
     * Builds the necessary internal elements (like m_sinkPtr and m_levelPtr) to m_binPtr and
     * then connects the GenericSink and GenericSource objects to their respective elements.
     *
     **/
    virtual void buildPipeline();

    GenericSink* m_appSink;         ///< A pointer to the GenericSink used by the AbstractAudioSource.
    GenericSource* m_appSrc;        ///< A pointer to the GenericSource used by the AbstractAudioSource.
    QGst::BinPtr m_binPtr;          ///< A shared pointer to the bin that handles the elements of this AbstractAudioSource.
    QGst::PipelinePtr m_pipeline;   ///< A shared pointer to the pipeline that manipulates the activity state of this AbstractAudioSource.
    QGst::ElementPtr m_sinkPtr;     ///< A shared pointer to the application sink element.
    QGst::ElementPtr m_srcPtr;      ///< A shared pointer to the application source element.
    QGst::ElementPtr m_volumePtr;   ///< A shared pointer to the volume element.
    QGst::ElementPtr m_levelPtr;    ///< A shared pointer to the level element.

private slots:
    void onPipelineBusmessage (const QGst::MessagePtr& message);
};

/**
 * @brief Represents a device used as an audio source on the user's system.
 *
 * DeviceAudioSource objects are more commonly used to represent input devices
 * on a user's system to obtain audio directly from the device.
 **/
class SPCH_EXPORT DeviceAudioSource : public AbstractAudioSource
{
    Q_OBJECT
    Q_PROPERTY (QString DeviceName READ deviceName) ///< The internal name of this DeviceAudioSource.
    Q_PROPERTY (QString HumanName READ humanName)   ///< The presentable name of this DeviceAudioSource.
    Q_DISABLE_COPY (DeviceAudioSource)
    friend class AbstractAudioSource;

public:
    explicit DeviceAudioSource();
    DeviceAudioSource (const AbstractAudioSource& p_other);
    virtual ~DeviceAudioSource();
    QString deviceName() const;
    QString humanName() const;
    static AbstractAudioSourceList allDevices();
    static DeviceAudioSource* defaultDevice();
    static DeviceAudioSource* obtain (const QString& p_deviceName);

protected:
    DeviceAudioSource (const QString& p_deviceName);
    virtual QString pipelineDescription() const;
    virtual void buildPipeline();

private:
    void obtainDevice (const QString& p_deviceName);
    QGlib::Value m_device;
    QGst::ElementPtr m_devicePtr;
    static QMap<QString, DeviceAudioSource*> s_map;
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

class SPCH_EXPORT StreamSink : public GenericSink
{
    Q_OBJECT
    Q_DISABLE_COPY (StreamSink)

public:
    explicit StreamSink (StreamAudioSource* p_audioSrc);
    StreamSink (const GenericSink&);
    virtual ~StreamSink();
    virtual void eos();
    virtual QGst::BufferPtr pullBuffer();
    uint bufferSize() const;
    void setBufferSize (const uint& p_bufferSize);

private:
    StreamAudioSource* m_audioSrc;
};

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

#endif // ABSTRACTAUDIOSOURCE_HPP


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
