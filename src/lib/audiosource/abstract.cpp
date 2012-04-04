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

#include <QDebug>
#include <QVariant>
#include <QGlib/Error>
#include <QGlib/Connect>

#include "lib/audiosource/sink.hpp"
#include "lib/audiosource/source.hpp"
#include "lib/audiosource/abstract.hpp"

using namespace SpeechControl;

AbstractAudioSource::AbstractAudioSource (const AbstractAudioSource& p_other) : QObject (p_other.parent()), m_appSink (p_other.m_appSink),
    m_binPtr (p_other.m_binPtr), m_pipeline (p_other.m_pipeline), m_sinkPtr (p_other.m_sinkPtr),
    m_srcPtr (p_other.m_srcPtr), m_volumePtr (p_other.m_volumePtr), m_levelPtr (p_other.m_levelPtr)
{

}

AbstractAudioSource::AbstractAudioSource (QObject* p_parent) : QObject (p_parent),
    m_appSink (new GenericSink), m_binPtr(), m_pipeline(),
    m_sinkPtr(), m_srcPtr(), m_volumePtr(), m_levelPtr()
{
    // Ensure that the pointer's are empty.
    m_binPtr.clear();
    m_pipeline.clear();
    m_sinkPtr.clear();
    m_srcPtr.clear();
    m_volumePtr.clear();
    m_levelPtr.clear();
}

QString AbstractAudioSource::caps() const
{
    return "audio/x-raw-int,channels=1,rate=8000,"
           "signed=(boolean)true,width=16,depth=16,endianness=1234";
}

QString AbstractAudioSource::pipelineStr() const
{
    return QString ("%1 ! level name=level ! audioconvert ! audioresample ! audiorate ! volume name=volume ! appsink name=sink").arg (pipelineDescription());
}

/// @todo Delete m_appSrc if it exists but disconnect its slot/signal first.
void AbstractAudioSource::buildPipeline()
{
    try {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Building pipeline " << pipelineStr();
        m_binPtr = QGst::Bin::fromDescription (pipelineStr());
    }
    catch (const QGlib::Error& error) {
        qCritical() << QString ("[AbstractAudioSource::buildPipeline()] Failed to create audio source bin from pipeline '%1': %2").arg (pipelineStr()).arg (error.what());
        m_binPtr.clear();
        return;
    }

    if (m_binPtr.isNull()) {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built as NULL using description" << pipelineStr();
        return;
    }
    else {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built using description" << pipelineStr() << m_binPtr->pathString();

        // Obtain tools for recording like the encoder and the source.
        m_sinkPtr = m_binPtr->getElementByName ("sink");
        m_srcPtr = m_binPtr->getElementByName ("src");
        m_volumePtr = m_binPtr->getElementByName ("volume");
        m_levelPtr = m_binPtr->getElementByName ("level");
        qDebug() << "[AbstractAudioSource::buildPipeline()] Obtained pipeline elements.";

        if (m_sinkPtr.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the application sink.";
        }

        if (m_srcPtr.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the dynamic source.";
        }

        if (isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] NULL elements discovered, aborting the building of the pipeline.";
            m_binPtr.clear();
            return;
        }

        // build our magical sink and sources.
        m_appSink->setCaps (QGst::Caps::fromString (caps()));
        m_appSink->setElement (m_sinkPtr);

        m_appSrc = new GenericSource (this);
        m_appSrc->setCaps (QGst::Caps::fromString (caps()));
        connect (m_appSrc, SIGNAL (bufferObtained (QByteArray)), this, SIGNAL (bufferObtained (QByteArray)));
        m_appSink->setSource (m_appSrc);

        m_binPtr->setState (QGst::StatePaused);
        qDebug() << "[AbstractAudioSource::buildPipeline()] Paused elements in pipeline.";
    }
}

double AbstractAudioSource::volume() const
{
    if (isNull())
        return -1.0;

    qDebug() << "[AbstractContentSource::volume()] Level: " << m_levelPtr->property ("peak-falloff").toString();
    return m_volumePtr->property ("volume").toString().toDouble();
}

bool AbstractAudioSource::isMuted() const
{
    if (isNull())
        return -1.0;

    return m_volumePtr->property ("mute").toBool();
}

void AbstractAudioSource::setVolume (const double p_volume)
{
    if (!isNull()) {
        m_volumePtr->setProperty<double> ("volume", p_volume);
        qDebug() << "[AbstractContentSource::setVolume()] Set volume of " << p_volume << ".";
    }
}

void AbstractAudioSource::setMuted (const bool p_muted)
{
    if (!isNull()) {
        m_volumePtr->setProperty<bool> ("mute", p_muted);
        qDebug() << "[AbstractContentSource::setVolume()] Set mute state of '" << p_muted << "'";
    }
}

QString getStateText (const QGst::State& p_state)
{
    switch (p_state) {
    case QGst::StateReady:
        return "Ready";
        break;

    case QGst::StatePaused:
        return "Paused";
        break;

    case QGst::StatePlaying:
        return "Playing";
        break;

    case QGst::StateVoidPending:
        return "Void; pending";
        break;

    default:
    case QGst::StateNull:
        return "Null";
        break;
    }

    return "Unknown";
}

void AbstractAudioSource::onPipelineBusmessage (const QGst::MessagePtr& message)
{
    switch (message->type()) {
    case QGst::MessageError: {
        QGst::ErrorMessagePtr l_errMsg = message.staticCast<QGst::ErrorMessage>();
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Error message:" << l_errMsg->debugMessage();
    }
    break;

    case QGst::MessageStateChanged: {
        QGst::StateChangedMessagePtr stateMsg = message.staticCast<QGst::StateChangedMessage>();
        QString oldState = getStateText (stateMsg->oldState());
        QString newState = getStateText (stateMsg->newState());
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] State changed from" << oldState << "to" << newState;
    }
    break;

    case QGst::MessageAsyncDone:
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous stop invoked.";
        break;

    case QGst::MessageAsyncStart:
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous start invoked.";
        break;

    case QGst::MessageStreamStatus: {
        QGst::StreamStatusMessagePtr streamStatusMessage = message.staticCast<QGst::StreamStatusMessage>();
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] " << streamStatusMessage->streamStatusObject().toString();
    }
    break;

    case QGst::MessageElement: {
        QGst::ElementMessagePtr elementMessage = message.staticCast<QGst::ElementMessage>();
        //qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Element message: " << elementMessage->internalStructure()->toString();
    }
    break;

    default:
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Unexpected message:" << message->typeName();
        break;
    }
}

bool AbstractAudioSource::isNull() const
{
    qDebug() << "[AbstractAudioSource::isNull()] Bin (" << m_binPtr.isNull()
             << "); Sink (" << m_sinkPtr.isNull()
             << "); Source (" << m_srcPtr.isNull()
             << "); Volume (" << m_volumePtr.isNull()
             << "); Level (" << m_levelPtr.isNull()
             << ")";
    return (m_binPtr.isNull() || m_sinkPtr.isNull() || m_srcPtr.isNull() || m_volumePtr.isNull() || m_levelPtr.isNull());
}

void AbstractAudioSource::start()
{
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::start()]"
                    << "One or more elements could not be created. "
                    << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }

    m_pipeline = QGst::Pipeline::create();
    m_pipeline->add (m_binPtr);

    // Connect the bus to this AbstractAudioSource to detect changes in the pipeline.
    m_pipeline->bus()->addSignalWatch();
    QGlib::connect (m_pipeline->bus(), "message", this, &AbstractAudioSource::onPipelineBusmessage);

    // Get the party started :)
    m_pipeline->setState (QGst::StatePlaying);

    qDebug() << "[AbstractAudioSource::start()] Bin active, recording started.";
    emit begun();
}

void AbstractAudioSource::stop()
{
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::stop()] "
                    << "One or more elements could not be created. "
                    << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }

    if (!m_pipeline.isNull()) {
        m_pipeline->bus()->removeSignalWatch();
        m_pipeline->setState (QGst::StateNull);
        m_pipeline.clear();
        m_binPtr->setState (QGst::StatePaused);
    }

    qDebug() << "[AbstractAudioSource::start()] Bin inactive, recording stopped.";
    emit ended();
}

bool AbstractAudioSource::isActive() const
{
    if (isNull())
        return false;

    qDebug() << "[AbstractAudioSource::isRecording()] State of bin" << getStateText (m_binPtr->currentState());
    return (m_binPtr->currentState() == QGst::StatePlaying);
}

AbstractAudioSource::~AbstractAudioSource()
{
    stop();

    // Clean our goodies' memory.
    delete m_appSrc;
    delete m_appSink;

    // Clean up your junk!
    if (!m_binPtr.isNull())
        m_binPtr->setState (QGst::StateNull);

    if (!m_pipeline.isNull())
        m_pipeline->setState (QGst::StateNull);

    if (!m_sinkPtr.isNull())
        m_sinkPtr->setState (QGst::StateNull);

    if (!m_srcPtr.isNull())
        m_srcPtr->setState (QGst::StateNull);

    if (!m_volumePtr.isNull())
        m_volumePtr->setState (QGst::StateNull);

    if (!m_levelPtr.isNull())
        m_levelPtr->setState (QGst::StateNull);

    // Clean memory.
    m_binPtr.clear();
    m_pipeline.clear();
    m_sinkPtr.clear();
    m_srcPtr.clear();
    m_volumePtr.clear();
    m_levelPtr.clear();
}

#include "audiosource/abstract.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
