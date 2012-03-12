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

#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QGlib/Error>
#include <QGlib/Connect>
#include "abstractaudiosource.hpp"

using namespace SpeechControl;

AbstractAudioSource::AbstractAudioSource ( const AbstractAudioSource& p_other ) : QObject(), m_sink ( 0 ) {

}

AbstractAudioSource::AbstractAudioSource ( QObject* parent ) : QObject ( parent ), m_sink(0) {

}

QString AbstractAudioSource::caps() const {
    return "audio/x-raw-int,channels=1,rate=8000,"
           "signed=(boolean)true,width=16,depth=16,endianness=1234";
}

void AbstractAudioSource::buildPipeline() {
    const QString resultingPipeline = pipelineDescription() + " ! " + QString ( "audioconvert !"
                                      "audioresample ! appsink name=genericsink caps=\"%1\"" ).arg ( caps() );
    try {
        m_binAudio = QGst::Bin::fromDescription ( resultingPipeline );
    } catch ( const QGlib::Error & error ) {
        qCritical() << QString ( "[Microphone::obtain()] Failed to create audio source bin from pipeline '%1': %2" ).arg ( resultingPipeline ).arg ( error.what() );
        m_binAudio.clear();
        return;
    }

    m_sink->setElement ( m_binAudio->getElementByName ( "mysink" ) );


    // Obtain tools for recording like the encoder and the source.
    m_sinkAudio = m_binAudio->getElementByName ( "sink" );
    m_srcAudio = m_binAudio->getElementByName ( "src" );
    m_srcVolume = m_binAudio->getElementByName ( "volume" );

    m_binAudio->setState ( QGst::StatePaused );
}

void AbstractAudioSource::onPipelineBusmessage ( const QGst::MessagePtr & message ) {
    switch ( message->type() ) {
    case QGst::MessageError: {
        QGst::ErrorMessagePtr l_errMsg = message.staticCast<QGst::ErrorMessage>();
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Error message:" << l_errMsg->debugMessage();
    }
    break;

    case QGst::MessageStateChanged: {
        QGst::StateChangedMessagePtr l_stateMsg = message.staticCast<QGst::StateChangedMessage>();
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Message started:" << l_stateMsg->newState();
    }
    break;

    case QGst::MessageAsyncDone: {
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous stop.";
    }
    break;

    case QGst::MessageAsyncStart: {
        QGst::AsyncDoneMessagePtr l_asyncDoneMsg = message.staticCast<QGst::AsyncDoneMessage>();
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous start:" << l_asyncDoneMsg->sequenceNumber();
    }
    break;

    default:
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Unhandled message:" << message->typeName();
        break;
    }
}

void AbstractAudioSource::startRecording() {
    if ( !m_sinkAudio ) {
        qCritical() << "Microphone::startRecording()"
                    << tr ( "One or more elements could not be created. "
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

    QGlib::connect ( m_pipeline->bus(), "message", this, &AbstractAudioSource::onPipelineBusmessage );

    // Get the party started :)
    m_pipeline->setState ( QGst::StatePlaying );

    qDebug() << "[AbstractAudioSource::startRecording()] Pipeline active, recording started.";
    emit recordingBegun();
}

void AbstractAudioSource::stopRecording() {

}

bool AbstractAudioSource::isRecording() const {

}

AbstractAudioSource::~AbstractAudioSource() {

}

DeviceAudioSource::DeviceAudioSource ( const DeviceAudioSource& p_other ) : AbstractAudioSource ( p_other ) {

}

DeviceAudioSource::DeviceAudioSource ( const QString& p_deviceName ) : AbstractAudioSource() {
    obtainDevice ( p_deviceName );
}

DeviceAudioSource::DeviceAudioSource() {

}

DeviceAudioSource::DeviceAudioSource ( const AbstractAudioSource& p_other ) : AbstractAudioSource ( p_other ) {

}

DeviceAudioSource* DeviceAudioSource::defaultDevice() {

}

void DeviceAudioSource::obtainDevice ( const QString& p_deviceName ) {

}

AbstractAudioSourceList DeviceAudioSource::allDevices() {
    const QString audioSrc ( "autoaudiosrc" );
    QGst::ElementPtr audioSrcPtr = QGst::ElementFactory::make ( audioSrc );
    AbstractAudioSourceList list;

    if ( audioSrcPtr ) {
        audioSrcPtr->setState ( QGst::StateReady );
        QGst::ChildProxyPtr chldPrxy = audioSrcPtr.dynamicCast<QGst::ChildProxy>();
        QGst::PropertyProbePtr propProbe;

        if ( chldPrxy ) {
            propProbe = chldPrxy->childByIndex ( 0 ).dynamicCast<QGst::PropertyProbe>();
        }

        if ( propProbe ) {
            QList<QGlib::Value> devices = propProbe->probeAndGetValues ( "device" );
            audioSrcPtr->setState ( QGst::StateNull );

            if ( propProbe && propProbe->propertySupportsProbe ( "device" ) ) {
                Q_FOREACH ( QGlib::Value l_device, devices ) {
                    qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << l_device;
                    propProbe->setProperty ( "device", l_device );
                    list << new DeviceAudioSource ( l_device.toString() );
                }
            }

        }

    } else {
        qDebug() << tr ( "Failed to create element \"%1\". Make sure you have "
                         "gstreamer-plugins-good installed" ).arg ( audioSrc );

    }

    return list;
}

QString DeviceAudioSource::deviceName() const {
    return QString::null;
}

QString DeviceAudioSource::pipelineDescription() {
    return QString ( "autoaudiosrc name=deviceaudiosrc ! decodebin2" );
}

void DeviceAudioSource::buildPipeline() {
    SpeechControl::AbstractAudioSource::buildPipeline();
    QGst::ChildProxyPtr childProxy = m_srcAudio.dynamicCast<QGst::ChildProxy>();
    if ( childProxy && childProxy->childrenCount() > 0 ) {
        QGst::ObjectPtr realSrc = childProxy->childByIndex ( 0 );
        realSrc->setProperty ( "device", m_device.toString() );
        QList<QGlib::ParamSpecPtr> properties = realSrc->listProperties();
        Q_FOREACH ( QGlib::ParamSpecPtr property, properties ) {
            QString name = property->name();
            QGlib::Value value = realSrc->property ( property->name().toStdString().c_str() );
            qDebug() << QString ( "[Microphone::obtain()] Device property %1 = %2" ).arg ( name, value.toString() );
        }
        qDebug() << QString ( "[Microphone::obtain()] Obtained device %1" ).arg ( m_device.toString() );
    }
}

DeviceAudioSource::~DeviceAudioSource() {

}

#include "abstractaudiosource.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
