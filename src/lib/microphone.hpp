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

#ifndef MICROPHONE_HPP
#define MICROPHONE_HPP

#include <QMap>
#include <QList>
#include <QUuid>
#include <QObject>

#include <QGlib/Value>
#include <QGst/Bin>
#include <QGst/Pad>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Element>
#include <QGst/Pipeline>
#include <QGst/ChildProxy>
#include <QGst/ElementFactory>
#include <QGst/PropertyProbe>
#include <QGst/StreamVolume>

#include <export.hpp>

namespace SpeechControl {

class Microphone;

/**
 * @brief Represents a shorthand for denoating a list of @see Microphone.
 **/
typedef QList<Microphone*> MicrophoneList;

/**
 * @brief Represents a named mapping of Microphones.
 **/
typedef QMap<QUuid, Microphone*> MicrophoneMap;

/**
 * @brief Represents a handle of an audio input device on this computer.
 *
 * The Microphone class allows developers to not worry too much about handling
 * audio devices for SpeechControl. It automatically obtains a listing of all
 * input devices in the system and allows this to be attached to other classes
 * that'd require the use of an audio input device like AbstractSphinx.
 **/
class SPCH_EXPORT Microphone : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Microphone )
    Q_PROPERTY ( const bool Active READ active )                    ///< Determines the Active state of the microphone.
    Q_PROPERTY ( const QString Name READ name )                     ///< Obtains a name to recognize this device with.
    Q_PROPERTY ( const QString ID READ id )                         ///< The ID of the Microphone, as recognized by SpeechControl.
    Q_PROPERTY ( const QByteArray Data READ data )                  ///< The data currently fed in from the microphone.
    Q_PROPERTY ( const double Volume READ volume WRITE setVolume )  ///< Represents the volume of the Microphone.
    Q_PROPERTY ( const bool Muted READ isMuted WRITE mute )         ///< Represents the muted state of the Microphone.

public:

    /**
     * @brief Default constructor.
     * @param p_glibPointer Defaults to 0.
     **/
    explicit Microphone ( QGlib::Value p_glibPointer = 0 );

    /**
     * @brief Destructor.
     **/
    virtual ~Microphone();

    /**
     * @brief Get microphone by UUID
     * @param p_uuid UUID of the wanted microphone.
     * @return Pointer to the microphone.
     */
    static Microphone* getMicrophone ( const QUuid& p_uuid );

    /**
     * @brief Get default microphone
     * @return Pointer to the default microphone.
     */
    static Microphone* defaultMicrophone();

    /**
     * @brief Obtains a QList of all of the Microphone objects.
     * @return A MicrophoneList.
     **/
    static MicrophoneList allMicrophones();

    /**
     * @brief Initializes the Microphone's internal workings.
     * @attention This method should be called only once. Subsequent calls invoke warnings.
     * @return void
     **/
    static void init();

    /**
     * @brief Determines if the Microphone is active.
     * @return TRUE if the Microphone is being actively used, FALSE otherwise.
     **/
    bool active() const;

    /**
     * @brief Obtains the data held by the Microphone during its current recording.
     * @see stream()
     * @return A QByteArray holding the data.
     **/
    const QByteArray& data() const;

    /**
     * @brief Obtains an ID representing this Microphone.
     * @return A QString holding the ID value.
     **/
    QString id() const;

    /**
     * @brief Obtains the name of the microphone as it's recognized by the system.
     * @note This value currently doesn't return a user-friendly name.
     * @return A QString holding the name of the Microphone.
     **/
    QString name() const;

    /**
     * @brief Obtains the volume of the microphone.
     * @return A double ranging from 0.0 to 10.0.
     **/
    double volume() const;

    /**
     * @brief Determines if the Microphone is muted.
     * @return TRUE if the Microphone is muted, FALSE otherwise.
     **/
    bool isMuted() const;

    /**
     * @brief Determines if this Microphone object is valid.
     * @return TRUE if the Microphone is valid, FALSE otherwise.
     **/
    bool isValid() const;

    /**
     * @brief Determines if this Microphone is currently recording.
     * @return TRUE if the Microphone is recording (in the Playing state), FALSE otherwise.
     **/
    bool isRecording() const;

    /**
     * @brief Obtains a QDataStream that holds and updates whenever the Microphone obtains data.
     * @return A pointer to a QDataStream object if data's available, NULL otherwise.
     **/
    QDataStream* stream() const;

    /**
     * @brief Sets the volume of the Microphone.
     * @param p_volume The new value of the volume.
     **/
    void setVolume ( const double& p_volume );

    /**
     * @brief Sets whether or not the Microphone is muted.
     * @param p_muted The new value of the muted state.
     **/
    void mute ( const bool& p_muted );

signals:
    /**
     * @brief Emitted when the Microphone has begun listening.
     * @see isListening()
     **/
    void startedListening();

    /**
     * @brief Emitted when the Microphone has stopped listening.
     * @see isListening()
     **/
    void stoppedListening();

public slots:
    /**
     * @brief Starts the recording p
     **/
    void startRecording();
    /**
     * @brief ...
     *
     * @return void
     **/
    void stopRecording();

private slots:
    void release();
    void onPipelineBusmessage ( const QGst::MessagePtr& p_glibMsgPtr );
    void onSinkAudioEos ( const QGlib::Value& p_glibValue );
    void onSinkAudioNewbuffer ( const QGlib::Value& p_glibValue );

private:
    /**
     * @brief Find all microphones attached to the machine
     */
    static void findMicrophones();

    static MicrophoneMap p_micLst;
    static QGst::ElementPtr s_src;
    static QGst::PropertyProbePtr s_propProbe;
    static QGst::ChildProxyPtr s_chldPrxy;

    void obtain();
    QGst::BinPtr m_binAudio;
    QGst::PadPtr m_padAudio;
    QGst::ElementPtr m_srcAudio;
    QGst::ElementPtr m_srcVolume;
    QGst::ElementPtr m_sinkAudio;
    QGst::PipelinePtr m_pipeline;
    QGlib::Value m_device;
    QUuid m_id;
    QByteArray m_data;

};
}

#endif // MICROPHONE_HPP


// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
