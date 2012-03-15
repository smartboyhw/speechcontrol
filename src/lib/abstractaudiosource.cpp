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

AbstractAudioSource::AbstractAudioSource (const AbstractAudioSource& p_other) : QObject (p_other.parent()), m_sink (0),
    m_device (p_other.m_device), m_pipeline (p_other.m_pipeline), m_sinkAudio (p_other.m_sinkAudio),
    m_srcAudio (p_other.m_srcAudio), m_srcVolume (p_other.m_srcVolume)
{

}

AbstractAudioSource::AbstractAudioSource (QObject* parent) : QObject (parent), m_sink (0), m_device(),
    m_pipeline(), m_sinkAudio(), m_srcAudio(), m_srcVolume()
{

}



QString AbstractAudioSource::caps() const
{
    return "audio/x-raw-int,channels=1,rate=8000,"
           "signed=(boolean)true,width=16,depth=16,endianness=1234";
}

QString AbstractAudioSource::pipelineStr() const
{
    return pipelineDescription() + " ! "
           + QString ("audioconvert ! "
                      "audioresample ! "
                      "volume name=volume ! "
                      "appsink name=sink caps='%1'").arg (caps());
}

void AbstractAudioSource::buildPipeline()
{
    try {
        m_pipeline = QGst::Pipeline::create (pipelineStr().toAscii());
    }
    catch (const QGlib::Error& error) {
        qCritical() << QString ("[AbstractAudioSource::buildPipeline()] Failed to create audio source bin from pipeline '%1': %2").arg (pipelineStr()).arg (error.what());
        m_pipeline.clear();
        return;
    }

    if (m_pipeline.isNull()) {
        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built as NULL using description" << pipelineStr();
        return;
    }
    else {

        qDebug() << "[AbstractAudioSource::buildPipeline()] Pipeline built using description" << pipelineStr();

        // build our magical sink.
        m_sink = new GenericSink;
        m_sink->setCaps (QGst::Caps::fromString (caps()));

        // Obtain tools for recording like the encoder and the source.
        m_sinkAudio = m_pipeline->getElementByName ("sink");
        m_srcAudio = m_pipeline->getElementByName ("src");
        m_srcVolume = m_pipeline->getElementByName ("volume");
        qDebug() << "[AbstractAudioSource::buildPipeline()] Obtained pipeline elements.";

        if (m_sinkAudio.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the application sink.";
            return;
        }
        else if (m_srcAudio.isNull()) {
            qDebug() << "[AbstractAudioSource::buildPipeline()] Invalid element pointer obtained for the dynamic source.";
            return;
        }
        else {
            m_sink->setElement (m_sinkAudio);
            m_pipeline->setState (QGst::StatePaused);
            QGlib::connect (m_pipeline->bus(), "message", this, &AbstractAudioSource::onPipelineBusmessage);
            qDebug() << "[AbstractAudioSource::buildPipeline()] Paused elements in pipeline.";
        }
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

    case QGst::MessageAsyncDone: {
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous stop invoked.";
    }
    break;

    case QGst::MessageAsyncStart: {
        QGst::AsyncDoneMessagePtr l_asyncDoneMsg = message.staticCast<QGst::AsyncDoneMessage>();
        qDebug() << "[AbstractContentSource::onPipelineBusmessage()] Asynchronous start invoked.";
    }
    break;

    default:
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Unexpected message:" << message->typeName();
        break;
    }
}

void AbstractAudioSource::startRecording()
{
    if (!m_pipeline) {
        qCritical() << "[AbstractAudioSource::startRecording()]"
                    << "One or more elements could not be created. "
                    << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }

    // Connect the bus to this Microphone to detect changes in the pipeline.
    m_pipeline->bus()->addSignalWatch();

    // Get the party started :)
    m_pipeline->setState (QGst::StatePlaying);

    qDebug() << "[AbstractAudioSource::startRecording()] Pipeline active, recording started.";
    emit recordingBegun();
}

void AbstractAudioSource::stopRecording()
{
    if (!m_pipeline) {
        qCritical() << "[AbstractAudioSource::stopRecording()]"
        << "One or more elements could not be created. "
        << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }

    m_pipeline->bus()->removeSignalWatch();
    m_pipeline->setState(QGst::StateNull);
    qDebug() << "[AbstractAudioSource::startRecording()] Pipeline inactive, recording stopped.";
    emit recordingEnded();
}

bool AbstractAudioSource::isRecording() const
{
    qDebug() << "[AbstractAudioSource::isRecording()] State of pipeline" << getStateText (m_pipeline->currentState());
    return (m_pipeline->currentState() == QGst::StatePlaying);
}

AbstractAudioSource::~AbstractAudioSource()
{

}

GenericSink::GenericSink()
{

}

void GenericSink::eos()
{
    qDebug() << "[GenericSink::eos()] End of stream in generic sink.";
    m_src->endOfStream();
}

QGst::FlowReturn GenericSink::newBuffer()
{
    QGst::BufferPtr buffer = pullBuffer();
    qDebug() << "[GenericSink::newBuffer()] Buffer: " << buffer->data();
    return m_src->pushBuffer (buffer);
}

void GenericSink::setSource (QGst::Utils::ApplicationSource* p_source)
{
    qDebug() << "[GenericSink::setSource()] Set a new source." << p_source->caps()->toString();
    m_src = p_source;
}

QGst::Utils::ApplicationSource* GenericSink::source()
{
    return m_src;
}

DeviceAudioSource::DeviceAudioSource() : AbstractAudioSource (0), m_devicePtr()
{

}

DeviceAudioSource::DeviceAudioSource (const QString& p_deviceName) : AbstractAudioSource (0), m_devicePtr()
{
    obtainDevice (p_deviceName);
}

DeviceAudioSource::DeviceAudioSource (const DeviceAudioSource& p_other) : AbstractAudioSource (p_other), m_devicePtr (p_other.m_devicePtr)
{

}

DeviceAudioSource::DeviceAudioSource (const AbstractAudioSource& p_other) : AbstractAudioSource (p_other), m_devicePtr()
{

}

DeviceAudioSource* DeviceAudioSource::defaultDevice()
{
    return 0;
}

void DeviceAudioSource::obtainDevice (const QString& p_deviceName)
{
    qDebug() << "[DeviceAudioSource::obtainDevice()] Obtaining device" << p_deviceName << "...";

    if (!m_device.isValid())
        m_device = QGlib::Value::create<QString> (p_deviceName);
    else
        m_device.set<QString> (p_deviceName);

    buildPipeline();

    if (m_devicePtr)
        m_devicePtr->setProperty ("device", m_device);
    else {
        qDebug() << "[DeviceAudioSource::obtainDevice()] Failed to set device for use; invalid device pointer.";
    }
}

AbstractAudioSourceList DeviceAudioSource::allDevices()
{
    const QString audioSrc ("autoaudiosrc");
    QGst::ElementPtr audioSrcPtr = QGst::ElementFactory::make (audioSrc);
    AbstractAudioSourceList list;

    if (audioSrcPtr) {
        audioSrcPtr->setState (QGst::StateReady);
        QGst::ChildProxyPtr chldPrxy = audioSrcPtr.dynamicCast<QGst::ChildProxy>();
        QGst::PropertyProbePtr propProbe;

        if (chldPrxy) {
            propProbe = chldPrxy->childByIndex (0).dynamicCast<QGst::PropertyProbe>();
        }

        if (propProbe) {
            QList<QGlib::Value> devices = propProbe->probeAndGetValues ("device");
            audioSrcPtr->setState (QGst::StateNull);

            if (propProbe && propProbe->propertySupportsProbe ("device")) {
                Q_FOREACH (QGlib::Value l_device, devices) {
                    qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << l_device;
                    propProbe->setProperty ("device", l_device);
                    list << new DeviceAudioSource (l_device.toString());
                }
            }

        }

    }
    else {
        qDebug() << tr ("Failed to create element \"%1\". Make sure you have "
                        "gstreamer-plugins-good installed").arg (audioSrc);

    }

    return list;
}

QString DeviceAudioSource::deviceName() const
{
    return m_devicePtr->pathString();
}

QString DeviceAudioSource::pipelineDescription() const
{
    return QString ("autoaudiosrc name=src ! decodebin2");
}

void DeviceAudioSource::buildPipeline()
{
    AbstractAudioSource::buildPipeline();
    QGst::ChildProxyPtr childProxy = m_srcAudio.dynamicCast<QGst::ChildProxy>();

    if (childProxy && childProxy->childrenCount() > 0) {
        QGst::ObjectPtr realSrc = childProxy->childByIndex (0);
        realSrc->setProperty ("device", m_device.toString());
        QList<QGlib::ParamSpecPtr> properties = realSrc->listProperties();
        Q_FOREACH (QGlib::ParamSpecPtr property, properties) {
            QString name = property->name();
            QGlib::Value value = realSrc->property (property->name().toStdString().c_str());
            qDebug() << QString ("[DeviceAudioSource::obtain()] Device property %1 = %2").arg (name, value.toString());
        }
        qDebug() << QString ("[DeviceAudioSource::obtain()] Obtained device %1").arg (m_device.toString());
    }

    m_devicePtr = m_pipeline->getElementByName ("src");

    if (m_devicePtr.isNull())
        qDebug() << "[DeviceAudioSource::obtain()] Warning! The obtained device pointer is NULL!" << m_device.toString();
}

DeviceAudioSource::~DeviceAudioSource()
{

}

#include "abstractaudiosource.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
