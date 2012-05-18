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
#include <QGst/Bus>
#include <QGlib/Error>
#include <QGlib/Connect>

#include "audiosource/sink.hpp"
#include "audiosource/source.hpp"
#include "audiosource/abstractprivate.hpp"
#include "audiosource/abstract.hpp"

using namespace SpeechControl;

AbstractAudioSourcePrivate::AbstractAudioSourcePrivate() : appSink (0),
    appSource (0), ptrBin (), m_pipeline (), ptrAudioSink (),
    ptrAudioSource (), ptrVolume (), ptrLevel ()
{
    ptrBin.clear();
    m_pipeline.clear();
    ptrAudioSink.clear();
    ptrAudioSource.clear();
    ptrVolume.clear();
    ptrLevel.clear();
}

AbstractAudioSourcePrivate::~AbstractAudioSourcePrivate()
{
    // Clean up your junk!
    if (!ptrBin.isNull())
        ptrBin->setState (QGst::StateNull);

    if (!m_pipeline.isNull())
        m_pipeline->setState (QGst::StateNull);

    if (!ptrAudioSink.isNull())
        ptrAudioSink->setState (QGst::StateNull);

    if (!ptrAudioSource.isNull())
        ptrAudioSource->setState (QGst::StateNull);

    if (!ptrVolume.isNull())
        ptrVolume->setState (QGst::StateNull);

    if (!ptrLevel.isNull())
        ptrLevel->setState (QGst::StateNull);

    // Clean memory.
    ptrBin.clear();
    m_pipeline.clear();
    ptrAudioSink.clear();
    ptrAudioSource.clear();
    ptrVolume.clear();
    ptrLevel.clear();
}

AbstractAudioSource::AbstractAudioSource (const AbstractAudioSource& p_other) : QObject (p_other.parent()),
    d_ptr (const_cast<AbstractAudioSourcePrivate*> (p_other.d_ptr.data()))
{

}

AbstractAudioSource::AbstractAudioSource (QObject* p_parent) : QObject (p_parent),
    d_ptr (new AbstractAudioSourcePrivate)
{
}

AbstractAudioSource::AbstractAudioSource (AbstractAudioSourcePrivate* p_dPtr, QObject* p_parent) :
    QObject (p_parent), d_ptr (p_dPtr)
{

}

QString AbstractAudioSource::caps() const
{
    return "audio/x-raw-int,channels=1,rate=8000,"
           "signed=(boolean)true,width=16,depth=16,endianness=1234";
}

QString AbstractAudioSource::pipelineStr() const
{
    return QString ("%1 ! level name=level ! audioconvert ! "
    "audioresample ! audiorate ! volume name=volume ! "
    "appsink name=sink").arg (pipelineDescription());
}

/// @todo Delete d_func()->m_appSrc if it exists but disconnect its slot/signal first.
void AbstractAudioSource::buildPipeline()
{
    try {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Building pipeline " << pipelineStr();
        d_func()->ptrBin = QGst::Bin::fromDescription (pipelineStr());
    }
    catch (const QGlib::Error& error) {
        qCritical() << QString ("[AbstractAudioSource::buildPipeline()] Failed to create audio source bin from pipeline '%1': %2").arg (pipelineStr()).arg (error.what());
        d_func()->ptrBin.clear();
        return;
    }

    if (d_func()->ptrBin.isNull()) {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built as NULL using description" << pipelineStr();
        return;
    }
    else {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built using description" << pipelineStr() << d_func()->ptrBin->pathString();

        // Obtain elements.
        d_func()->ptrAudioSink   = d_func()->ptrBin->getElementByName ("sink");
        d_func()->ptrAudioSource = d_func()->ptrBin->getElementByName ("src");
        d_func()->ptrVolume      = d_func()->ptrBin->getElementByName ("volume");
        d_func()->ptrLevel       = d_func()->ptrBin->getElementByName ("level");

        qDebug() << "[AbstractAudioSource::buildPipeline()] Obtained pipeline elements.";

        if (d_func()->ptrAudioSink.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the application sink.";
        }

        if (d_func()->ptrAudioSource.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the dynamic source.";
        }

        if (isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] NULL elements discovered, aborting the building of the pipeline.";
            d_func()->ptrBin.clear();
            return;
        }

        // build our magical sink and sources.
        d_func()->appSink = new GenericSink (this);
        d_func()->appSink->setCaps (QGst::Caps::fromString (caps()));
        d_func()->appSink->setElement (d_func()->ptrAudioSink);

        d_func()->appSource = new GenericSource (this);
        d_func()->appSource->setCaps (QGst::Caps::fromString (caps()));
        connect (d_func()->appSource, SIGNAL (bufferObtained (QByteArray)), this, SIGNAL (bufferObtained (QByteArray)));
        d_func()->appSink->setSource (d_func()->appSource);

        d_func()->ptrBin->setState (QGst::StatePaused);
        qDebug() << "[AbstractAudioSource::buildPipeline()] Paused elements in pipeline.";
    }
}

double AbstractAudioSource::volume() const
{
    if (isNull())
        return -1.0;

    qDebug() << "[AbstractContentSource::volume()] Level: " << d_func()->ptrLevel->property ("peak-falloff").toString();
    return d_func()->ptrVolume->property ("volume").toString().toDouble();
}

bool AbstractAudioSource::isMuted() const
{
    if (isNull())
        return -1.0;

    return d_func()->ptrVolume->property ("mute").toBool();
}

void AbstractAudioSource::setVolume (const double p_volume)
{
    if (!isNull()) {
        d_func()->ptrVolume->setProperty<double> ("volume", p_volume);
        qDebug() << "[AbstractContentSource::setVolume()] Set volume of " << p_volume << ".";
    }
}

void AbstractAudioSource::setMuted (const bool p_muted)
{
    if (!isNull()) {
        d_func()->ptrVolume->setProperty<bool> ("mute", p_muted);
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
        QGst::ErrorMessagePtr errMsg = message.staticCast<QGst::ErrorMessage>();
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Error message:" << errMsg->debugMessage();
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
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous completion invoked.";
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

    case QGst::MessageClockProvide:
    case QGst::MessageClockLost: {
    }
    break;

    default:
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Unexpected message:" << message->typeName();
        break;
    }
}

bool AbstractAudioSource::isNull() const
{
    qDebug() << "[AbstractAudioSource::isNull()] Bin (" << d_func()->ptrBin.isNull()
             << "); Sink (" << d_func()->ptrAudioSink.isNull()
             << "); Source (" << d_func()->ptrAudioSource.isNull()
             << "); Volume (" << d_func()->ptrVolume.isNull()
             << "); Level (" << d_func()->ptrLevel.isNull()
             << ")";
    return (d_func()->ptrBin.isNull() || d_func()->ptrAudioSink.isNull() || d_func()->ptrAudioSource.isNull() || d_func()->ptrVolume.isNull() || d_func()->ptrLevel.isNull());
}

void AbstractAudioSource::start()
{
    Q_D(AbstractAudioSource);
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::start()]"
                    << "One or more elements could not be created. "
                    << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }


    d->appSink->setSource(d->appSource);
    d->appSource->setLive(true);

    d->m_pipeline = QGst::Pipeline::create();
    d->m_pipeline->add (d->ptrBin);

    // Connect the bus to this AbstractAudioSource to detect changes in the pipeline.
    d->m_pipeline->bus()->addSignalWatch();
    QGlib::connect (d->m_pipeline->bus(), "message", this, &AbstractAudioSource::onPipelineBusmessage);

    // Get the party started :)
    d->m_pipeline->setState (QGst::StatePlaying);

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

    if (!d_func()->m_pipeline.isNull()) {
        d_func()->m_pipeline->bus()->removeSignalWatch();
        d_func()->m_pipeline->setState (QGst::StateNull);
        d_func()->m_pipeline.clear();
    }

    qDebug() << "[AbstractAudioSource::start()] Bin inactive, recording stopped.";
    emit ended();
}

bool AbstractAudioSource::isActive() const
{
    if (isNull())
        return false;

    qDebug() << "[AbstractAudioSource::isRecording()] State of bin" << getStateText (d_func()->ptrBin->currentState());
    return (d_func()->ptrBin->currentState() == QGst::StatePlaying);
}

AbstractAudioSource::~AbstractAudioSource()
{
    stop();
    d_ptr.clear();
}

#include "audiosource/abstract.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
