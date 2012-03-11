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

#include "microphone.hpp"

#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QGlib/Error>
#include <QGlib/Connect>

using namespace SpeechControl;

MicrophoneMap Microphone::p_micLst;
QGst::ElementPtr Microphone::s_src;
QGst::PropertyProbePtr Microphone::s_propProbe;
QGst::ChildProxyPtr Microphone::s_chldPrxy;

Microphone::Microphone ( QGlib::Value device )
    : m_device ( device ), m_id ( device.toString() ) {
    connect ( this, SIGNAL ( destroyed() ), this, SLOT ( release() ) );
    obtain();
}

Microphone::Microphone ( const Microphone &p_mic ) : QObject ( p_mic.parent() ),
    m_device ( p_mic.m_device ), m_id ( p_mic.m_id ) {
    connect ( this, SIGNAL ( destroyed() ), &p_mic, SIGNAL ( destroyed() ) );
    obtain();
}

/// @todo Have the system detect when new microphones are added + removed to the system.
void Microphone::init() {
    findMicrophones();
}

void Microphone::findMicrophones() {
    const QString l_audioSrc ( "autoaudiosrc" );
    s_src = QGst::ElementFactory::make ( l_audioSrc );

    if ( s_src ) {
        s_src->setState ( QGst::StateReady );
        s_chldPrxy = s_src.dynamicCast<QGst::ChildProxy>();

        if ( s_chldPrxy ) {
            s_propProbe = s_chldPrxy->childByIndex ( 0 ).dynamicCast<QGst::PropertyProbe>();
        }

        if ( s_propProbe ) {
            QList<QGlib::Value> devices = s_propProbe->probeAndGetValues ( "device" );
            s_src->setState ( QGst::StateNull );

            if ( s_propProbe && s_propProbe->propertySupportsProbe ( "device" ) ) {
                Q_FOREACH ( QGlib::Value l_device, devices ) {
                    qDebug() << tr ( "Found microphone" ) << l_device;
                    s_propProbe->setProperty ( "device", l_device );
                    new Microphone ( l_device );
                }
            }
        }
    } else {
        qDebug() << tr ( "Failed to create element \"%1\". Make sure you have "
                         "gstreamer-plugins-good installed" ).arg ( l_audioSrc );
    }
}

Microphone* Microphone::getMicrophone ( const QUuid &p_uuid ) {
    return p_micLst.value ( p_uuid, 0 );
}

/// @todo How do you determine which microphone is the default one?
Microphone* Microphone::defaultMicrophone() {
    if ( !p_micLst.empty() ) {
        return p_micLst.values().first();
    }

    return 0;
}

bool Microphone::active() const {
    return ! ( !this->m_pipeline );
}

QString Microphone::name() const {
    return m_device.toString();
}

QString Microphone::id() const {
    return m_id;
}

MicrophoneList Microphone::allMicrophones() {
    return p_micLst.values();
}

/// @todo In addition, you will NEED TO MAKE SURE THAT YOU RECORD AT A SAMPLING RATE OF 16 KHZ (or 8 kHz if you adapt a telephone model) IN MONO WITH SINGLE CHANNEL.
/// @todo Use a buffer instead of using a file.
/// @note issue
void SpeechControl::Microphone::startRecording() {
    // Wipe any data already used for recording; their loss.
    m_data.clear();

    if ( !m_sinkAudio ) {
        qCritical() << tr ( "One or more elements could not be created. "
                            "Verify that you have all the necessary element plugins installed." );
        return;
    }

    //m_sinkAudio->setProperty("location", "file.wav");
    m_sinkAudio->setProperty ( "buffer-size", 1024 );

    // Build the pipeline.
    m_pipeline = QGst::Pipeline::create ( );
    m_pipeline->add ( m_binAudio );

    // Connect the bus to this Microphone to detect changes in the pipeline.
    m_pipeline->bus()->addSignalWatch();

    QGlib::connect ( m_pipeline->bus(), "message", this, &Microphone::onPipelineBusmessage );
    QGlib::connect ( m_sinkAudio, "eos", this, &Microphone::onSinkAudioEos );
    QGlib::connect ( m_sinkAudio, "new-buffer", this, &Microphone::onSinkAudioNewbuffer );

    // Get the party started :)
    m_sinkAudio->setState ( QGst::StatePlaying );
    m_srcAudio->setState ( QGst::StatePlaying );
    m_pipeline->setState ( QGst::StatePlaying );

    emit startedListening();
}

void SpeechControl::Microphone::stopRecording() {
    m_sinkAudio->setState ( QGst::StateNull );
    m_srcAudio->setState ( QGst::StateNull );
    m_pipeline->setState ( QGst::StateNull );

    emit stoppedListening();
}

const QByteArray& Microphone::data() const {
    return m_data;
}

double SpeechControl::Microphone::volume() const {
    return qVariantFromValue ( m_srcVolume->property ( "volume" ).toString() ).toDouble();
}

bool SpeechControl::Microphone::isMuted() const {
    return m_srcVolume->property ( "mute" ).toBool();
}

bool Microphone::isRecording() const {
    return m_srcAudio->currentState() == QGst::StatePlaying;
}

QDataStream* Microphone::stream() const {
    return new QDataStream ( new QByteArray ( m_data ),QIODevice::ReadOnly );
}

void SpeechControl::Microphone::setVolume ( const double &p_volume ) {
    if ( p_volume < 0.0 || p_volume > 10.0 ) {
        qDebug() << tr ( "[Microphone] Got invalid volume:" ) << p_volume;
        return;
    }

    m_srcVolume->setProperty ( "volume", p_volume );
}

void SpeechControl::Microphone::mute ( const bool &p_muted ) {
    m_srcVolume->setProperty ( "mute", p_muted );
}

void SpeechControl::Microphone::obtain() {
    try {
        m_binAudio = QGst::Bin::fromDescription ( "autoaudiosrc name=src ! audioconvert ! "
                     "audioresample ! audiorate ! volume name=volume ! "
                     "appsink name=sink" );
    } catch ( const QGlib::Error & error ) {
        qCritical() << "Failed to create audio source bin:" << error;
        m_binAudio.clear();
        return;
    }

    p_micLst.insert ( m_id, const_cast<Microphone*> ( this ) );

    // Obtain tools for recording like the encoder and the source.
    m_sinkAudio = m_binAudio->getElementByName ( "sink" );
    m_srcAudio = m_binAudio->getElementByName ( "src" );
    m_srcVolume = m_binAudio->getElementByName ( "volume" );

    QGst::ChildProxyPtr childProxy = m_srcAudio.dynamicCast<QGst::ChildProxy>();
    if ( childProxy && childProxy->childrenCount() > 0 ) {
        QGst::ObjectPtr realSrc = childProxy->childByIndex ( 0 );
        realSrc->setProperty ( "device", m_device.toString() );
    }

    m_binAudio->setState ( QGst::StatePaused );
}

void SpeechControl::Microphone::release() {
    // Turn everything off.
    m_binAudio->setState ( QGst::StateNull );
    m_srcAudio->setState ( QGst::StateNull );
    m_sinkAudio->setState ( QGst::StateNull );
    m_pipeline->setState ( QGst::StateNull );

    // Free memory.
    m_binAudio.clear();
    m_srcAudio.clear();
    m_sinkAudio.clear();
    m_pipeline.clear();
}

bool Microphone::isValid() const {
    return !m_binAudio.isNull();
}

void Microphone::onPipelineBusmessage ( const QGst::MessagePtr & message ) {
    switch ( message->type() ) {
    case QGst::MessageError: {
        QGst::ErrorMessagePtr l_errMsg = message.staticCast<QGst::ErrorMessage>();
        qWarning() << "Pipeline Error: " << l_errMsg->debugMessage();
    }
    break;

    case QGst::MessageStateChanged: {
        QGst::StateChangedMessagePtr l_stateMsg = message.staticCast<QGst::StateChangedMessage>();
        qDebug() << "State: " << l_stateMsg->newState();
    }
    break;

    case QGst::MessageAsyncDone: {
        stopRecording();
    } break;

    case QGst::MessageAsyncStart: {
        QGst::AsyncDoneMessagePtr l_asyncDoneMsg = message.staticCast<QGst::AsyncDoneMessage>();
        qDebug() << l_asyncDoneMsg->sequenceNumber() << l_asyncDoneMsg->internalStructure()->numberOfFields();
    } break;
    default:
        qDebug() << message->typeName();
        break;
    }
}

void Microphone::onSinkAudioEos ( const QGlib::Value &p_eos ) {
    qDebug() << "EOS:" << p_eos;
}

void Microphone::onSinkAudioNewbuffer ( const QGlib::Value &p_strm ) {
    m_data.append ( p_strm.toByteArray() );
    qDebug() << "BUFFER:" << p_strm;
}

Microphone::~Microphone() {
    m_data.clear();
}

#include "microphone.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
