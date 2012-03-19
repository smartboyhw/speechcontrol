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
#include "abstractaudiosource.hpp"

using namespace SpeechControl;

AbstractAudioSource::AbstractAudioSource (const AbstractAudioSource& p_other) : QObject (p_other.parent()), m_sink (0),
    m_binPtr (p_other.m_binPtr), m_pipeline (p_other.m_pipeline), m_sinkPtr (p_other.m_sinkPtr),
    m_srcPtr (p_other.m_srcPtr), m_volumePtr (p_other.m_volumePtr), m_appSrc (p_other.m_appSrc)
{

}

AbstractAudioSource::AbstractAudioSource (QObject* parent) : QObject (parent), m_sink (0),
    m_binPtr(), m_pipeline(), m_sinkPtr(), m_srcPtr(), m_volumePtr(), m_appSrc (0)
{

}

QString AbstractAudioSource::caps() const
{
    return "audio/x-raw-int,channels=1,rate=8000,"
           "signed=(boolean)true,width=16,depth=16,endianness=1234";
}

QString AbstractAudioSource::pipelineStr() const
{
    return QString ("%1 ! audioconvert ! audioresample ! audiorate ! volume name=volume ! appsink name=sink").arg (pipelineDescription());
}

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

        // build our magical sink.
        m_sink = new GenericSink;
        m_sink->setCaps (QGst::Caps::fromString (caps()));

        // Obtain tools for recording like the encoder and the source.
        m_sinkPtr = m_binPtr->getElementByName ("sink");
        m_srcPtr = m_binPtr->getElementByName ("src");
        m_volumePtr = m_binPtr->getElementByName ("volume");
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

        m_appSrc = new QGst::Utils::ApplicationSource;
        m_appSrc->setElement (m_srcPtr);

        m_sink->setElement (m_sinkPtr);
        m_sink->setSource (m_appSrc);

        m_binPtr->setState (QGst::StatePaused);
        qDebug() << "[AbstractAudioSource::buildPipeline()] Paused elements in pipeline.";
    }
}

double AbstractAudioSource::volume() const
{
    if (isNull())
        return -1.0;

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
    }
}

void AbstractAudioSource::setMuted (const bool p_muted)
{
    if (!isNull()) {
        m_volumePtr->setProperty<bool> ("mute", p_muted);
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

        switch (streamStatusMessage->statusType()) {
        default:
            break;
        }
    }
    break;

    default:
        qWarning() << "[AbstractContentSource::onPipelineBusmessage()] Unexpected message:" << message->typeName();
        break;
    }
}

bool AbstractAudioSource::isNull() const
{
    qDebug() << "[AbstractAudioSource::isNull()] Bin null?" << m_binPtr.isNull();
    qDebug() << "[AbstractAudioSource::isNull()] Sink null?" << m_sinkPtr.isNull();
    qDebug() << "[AbstractAudioSource::isNull()] Source null?" << m_srcPtr.isNull();
    qDebug() << "[AbstractAudioSource::isNull()] Volume null?" << m_volumePtr.isNull();
    return (m_binPtr.isNull() || m_sinkPtr.isNull() || m_srcPtr.isNull() || m_volumePtr.isNull());
}

void AbstractAudioSource::startRecording()
{
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::startRecording()]"
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

    qDebug() << "[AbstractAudioSource::startRecording()] Pipeline active, recording started.";
    emit recordingBegun();
}

void AbstractAudioSource::stopRecording()
{
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::stopRecording()]"
                    << "One or more elements could not be created. "
                    << "Verify that you have all the necessary element plug-ins installed.";
        return;
    }

    m_pipeline->bus()->removeSignalWatch();
    m_pipeline->setState (QGst::StateNull);
    m_pipeline.clear();
    delete this->m_appSrc;
    qDebug() << "[AbstractAudioSource::startRecording()] Pipeline inactive, recording stopped.";
    emit recordingEnded();
}

bool AbstractAudioSource::isRecording() const
{
    if (isNull())
        return false;

    qDebug() << "[AbstractAudioSource::isRecording()] State of pipeline" << getStateText (m_binPtr->currentState());
    return (m_binPtr->currentState() == QGst::StatePlaying);
}

AbstractAudioSource::~AbstractAudioSource()
{

}

GenericSink::GenericSink() : m_src (0)
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

DeviceAudioSource::DeviceAudioSource() : AbstractAudioSource (0), m_device(), m_devicePtr()
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

        audioSrcPtr->setState (QGst::StatePlaying);

        if (chldPrxy) {
            propProbe = chldPrxy->childByIndex (0).dynamicCast<QGst::PropertyProbe>();
        }

        audioSrcPtr->setState (QGst::StateNull);

        if (propProbe) {
            QList<QGlib::Value> devices = propProbe->probeAndGetValues ("device");
            audioSrcPtr->setState (QGst::StateNull);

            if (propProbe && propProbe->propertySupportsProbe ("device")) {
                Q_FOREACH (QGlib::Value l_device, devices) {
                    qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << l_device.toString();
                    QList<QGlib::ParamSpecPtr> specs = propProbe->listProperties();
                    propProbe->setProperty ("device", l_device);

                    Q_FOREACH (const QGlib::ParamSpecPtr spec, specs) {
                        qDebug() << "[DeviceAudioSource::allDevices()] Device:" << l_device.toString()
                                 << spec->name() << propProbe->property (spec->name().toStdString().c_str()).toString()
                                 << spec->description();
                        ;
                    }
                    list << new DeviceAudioSource (l_device.toString());
                }
            }
        }
    }
    else {
        qDebug() << QString ("Failed to create element \"%1\". Make sure you have "
                             "gstreamer-plugins-good installed").arg (audioSrc);
    }

    try {
        QGst::BinPtr bin = QGst::Bin::fromDescription ("autoaudiosrc name=src ! audioconvert ! audioresample ! audiorate ! volume name=volume ! appsink name=sink");
        qDebug() << "Obtained sample!" << bin.isNull() << bin->getElementByName ("src").isNull();
    }
    catch (const QGlib::Error& error) {
        qDebug() << error.message();
    }

    return list;
}

QString DeviceAudioSource::deviceName() const
{
    if (!isNull() && !m_devicePtr.isNull())
        return m_devicePtr->property ("device-name").toString();
    else
        return m_device.toString();
}

QString DeviceAudioSource::pipelineDescription() const
{
    return QString ("autoaudiosrc name=src");
}

void DeviceAudioSource::buildPipeline()
{
    AbstractAudioSource::buildPipeline();

    if (isNull()) {
        qDebug() << "[DeviceAudioSource::obtain()] Can't obtain device element, base pipeline is NULL.";
        return;
    }
    else {

        QGst::ChildProxyPtr childProxy = m_srcPtr.dynamicCast<QGst::ChildProxy>();

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

        m_devicePtr = m_binPtr->getElementByName ("src");

        if (m_devicePtr.isNull())
            qDebug() << "[DeviceAudioSource::obtain()] Warning! The obtained device pointer is NULL!" << m_device.toString();
        else {
            m_devicePtr->setProperty<const char*> ("client", "SpeechControl");
            m_devicePtr->setProperty<bool> ("do-timestamp", true);
            m_devicePtr->setProperty<int> ("blocksize", -1);
            m_devicePtr->setProperty<bool>("message-forward",true);
        }
    }
}

DeviceAudioSource::~DeviceAudioSource()
{

}

StreamAudioSource::StreamAudioSource() : AbstractAudioSource()
{

}

StreamAudioSource::StreamAudioSource (const AbstractAudioSource& p_other) : AbstractAudioSource (p_other)
{

}

StreamAudioSource::StreamAudioSource (const StreamAudioSource& p_other) : AbstractAudioSource (p_other), m_strm (p_other.m_strm)
{

}

StreamAudioSource::StreamAudioSource (QDataStream& p_stream) : AbstractAudioSource (0), m_strm (&p_stream)
{

}

void StreamAudioSource::buildPipeline()
{
    SpeechControl::AbstractAudioSource::buildPipeline();
}

QString StreamAudioSource::pipelineDescription() const
{
    return QString ("appsrc name=src");
}

QDataStream* StreamAudioSource::stream() const
{
    return m_strm;
}

StreamAudioSource::~StreamAudioSource()
{

}

#include "abstractaudiosource.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
