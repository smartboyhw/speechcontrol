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

#ifndef SPCHCNTRL_LIB_AUDIOSOURCE_ABSTRACT_HPP_
#define SPCHCNTRL_LIB_AUDIOSOURCE_ABSTRACT_HPP_

#include <QMap>
#include <QList>
#include <QUuid>
#include <QObject>

#include <QGst/Message>

#include <lib/export.hpp>

namespace SpeechControl
{

class GenericSink;
class GenericSource;
class AbstractAudioSource;
struct AbstractAudioSourcePrivate;

/**
 * @brief Represents a shorthand for denoting a list of AbstractAudioSource objects.
 **/
typedef QList<AbstractAudioSource*> AudioSourceList;

/**
 * @brief Represents a named mapping of AbstractAudioSources.
 **/
typedef QMap<QUuid, AbstractAudioSource*> AudioSourceMap;

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
    Q_DECLARE_PRIVATE (AbstractAudioSource)

    QSharedPointer<AbstractAudioSourcePrivate> d_ptr;

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
     * @brief Copy constructor of private data.
     *
     * @param dd Private data to copy.
     * @param parent Parent to inherit from. Defaults to 0.
     **/
    AbstractAudioSource (AbstractAudioSourcePrivate* p_dPtr, QObject* p_parent = 0);

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

private slots:
    void onPipelineBusmessage (const QGst::MessagePtr& message);
};

}

#endif // SPCHCNTRL_LIB_AUDIOSOURCE_ABSTRACT_HPP_


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
