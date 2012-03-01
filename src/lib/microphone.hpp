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
 **/

class SPCH_EXPORT Microphone : public QObject {
    Q_OBJECT
    Q_PROPERTY ( const bool Active READ active )
    Q_PROPERTY ( const QString Name READ friendlyName )
    Q_PROPERTY ( const QString ID READ id )
    Q_PROPERTY ( const QByteArray Data READ data )
    Q_PROPERTY ( const double Volume READ volume WRITE setVolume )
    Q_PROPERTY ( const bool Muted READ isMuted WRITE mute )

public:
    Q_DISABLE_COPY ( Microphone )
    /**
     * @brief ...
     *
     * @param p_glibPointer ... Defaults to 0.
     **/
    explicit Microphone ( QGlib::Value p_glibPointer = 0 );
    /**
     * @brief ...
     *
     **/
    virtual ~Microphone();

    /**
     * @brief Get microphone by UUID
     * @param micUuid UUID of the wanted microphone.
     * @returns Pointer to the microphone.
     */
    static Microphone* getMicrophone ( const QUuid& p_uuid );

    /**
     * @brief Get default microphone
     * @returns Pointer to the default microphone.
     */
    static Microphone* defaultMicrophone();

    /**
     * @brief ...
     *
     * @return :MicrophoneList
     **/
    static MicrophoneList allMicrophones();

    /**
     * @brief ...
     *
     * @return void
     **/
    static void init();

    /**
     * @brief ...
     *
     * @return bool
     **/
    bool active() const;

    /**
     * @brief ...
     *
     * @return const QByteArray&
     **/
    const QByteArray& data() const;
    /**
     * @brief ...
     *
     * @return QString
     **/

    QString id() const;
    /**
     * @brief ...
     *
     * @return QString
     **/
    QString friendlyName() const;
    /**
     * @brief ...
     *
     * @return double
     **/
    double volume() const;
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isMuted() const;
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isValid() const;
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isRecording() const;
    /**
     * @brief ...
     *
     * @return QDataStream*
     **/
    QDataStream* stream() const;

    /**
     * @brief ...
     *
     * @param p_volume ...
     * @return void
     **/
    void setVolume ( const double& p_volume );
    /**
     * @brief ...
     *
     * @param p_muted ...
     * @return void
     **/
    void mute ( const bool& p_muted );

signals:
    /**
     * @brief ...
     *
     * @return void
     **/
    void startedListening();
    /**
     * @brief ...
     *
     * @return void
     **/
    void stoppedListening();

public slots:
    /**
     * @brief ...
     *
     * @return void
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

    static MicrophoneMap micMap;
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
