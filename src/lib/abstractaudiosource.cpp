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
QMap<QString, DeviceAudioSource*> DeviceAudioSource::s_map;

AbstractAudioSource::AbstractAudioSource (const AbstractAudioSource& p_other) : QObject (p_other.parent()), m_appSink (p_other.m_appSink),
    m_binPtr (p_other.m_binPtr), m_pipeline (p_other.m_pipeline), m_sinkPtr (p_other.m_sinkPtr),
    m_srcPtr (p_other.m_srcPtr), m_volumePtr (p_other.m_volumePtr), m_levelPtr (p_other.m_levelPtr)
{

}

AbstractAudioSource::AbstractAudioSource (QObject* parent) : QObject (parent),
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

    qDebug() << "[AbstractAudioSource::startRecording()] Bin active, recording started.";
    emit recordingBegun();
}

void AbstractAudioSource::stopRecording()
{
    if (isNull()) {
        qCritical() << "[AbstractAudioSource::stopRecording()] "
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

    qDebug() << "[AbstractAudioSource::startRecording()] Bin inactive, recording stopped.";
    emit recordingEnded();
}

bool AbstractAudioSource::isRecording() const
{
    if (isNull())
        return false;

    qDebug() << "[AbstractAudioSource::isRecording()] State of bin" << getStateText (m_binPtr->currentState());
    return (m_binPtr->currentState() == QGst::StatePlaying);
}

AbstractAudioSource::~AbstractAudioSource()
{
    stopRecording();

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

GenericSource::GenericSource (AbstractAudioSource* p_audioSource) : m_audioSrc (p_audioSource)
{
    setParent (p_audioSource);
    qDebug() << "[GenericSource::{constructor}] Built source for " << m_audioSrc->pipelineStr() << ".";
}

GenericSource::GenericSource (const GenericSource& p_other) : QObject (p_other.parent()), ApplicationSource(), m_audioSrc (p_other.m_audioSrc)
{

}

QGst::FlowReturn GenericSource::endOfStream()
{
    return QGst::FlowOk;
}

QGst::FlowReturn GenericSource::pushBuffer (const QGst::BufferPtr& p_buffer)
{
    quint8* bufferInt = p_buffer->data();
    QByteArray buffer;
    buffer.resize (1);
    buffer[0] = (qint8) * bufferInt;
    emit bufferObtained (buffer);

    //qDebug() << "[GenericSource::pushBuffer()] Buffer obtained from AbstractAudioSource" << p_buffer->data() << bufferInt << buffer;

    return QGst::FlowCustomSuccess;
}

GenericSource::~GenericSource()
{
    if (!element().isNull())
        element()->setState (QGst::StateNull);
}

GenericSink::GenericSink() : m_src (0)
{

}

GenericSink::GenericSink (const GenericSink& p_other) : QObject(p_other.parent()), ApplicationSink(), m_src(p_other.m_src)
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

    //qDebug() << "[GenericSink::newBuffer()] Buffer: " << * (buffer->data());

    return m_src->pushBuffer (buffer);
}

void GenericSink::setSource (GenericSource* p_source)
{
    qDebug() << "[GenericSink::setSource()] Set a new source." << p_source->caps()->toString();
    m_src = p_source;
}

GenericSource* GenericSink::source()
{
    return m_src;
}

GenericSink::~GenericSink()
{
    if (!element().isNull())
        element()->setState (QGst::StateNull);
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

DeviceAudioSource* DeviceAudioSource::obtain (const QString& p_deviceName)
{
    if (s_map.contains (p_deviceName))
        return s_map.value (p_deviceName);
    else {
        DeviceAudioSource* device = new DeviceAudioSource (p_deviceName);

        if (device->isNull())
            return 0;
        else {
            s_map.insert (p_deviceName, device);
            return device;
        }
    }

    return 0;
}

DeviceAudioSource* DeviceAudioSource::defaultDevice()
{
    return (DeviceAudioSource*) allDevices().first();
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
                Q_FOREACH (QGlib::Value device, devices) {
                    qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << device.toString();
                    QList<QGlib::ParamSpecPtr> specs = propProbe->listProperties();
                    propProbe->setProperty ("device", device);

                    Q_FOREACH (const QGlib::ParamSpecPtr spec, specs) {
                        qDebug() << "[DeviceAudioSource::allDevices()] Device:" << device.toString()
                                 << spec->name() << propProbe->property (spec->name().toStdString().c_str()).toString()
                                 << spec->description();
                        ;
                    }

                    if (!s_map.contains (device.toString()))
                        s_map.insert (device.toString(), new DeviceAudioSource (device.toString()));

                    list << DeviceAudioSource::obtain (device.toString());
                }
            }
        }
    }
    else {
        qDebug() << QString ("[DeviceAudioSource::allDevices()] Failed to create element \"%1\". Make sure you have "
                             "gstreamer-plugins-good installed").arg (audioSrc);
    }

    return list;
}

QString DeviceAudioSource::deviceName() const
{
    return m_device.toString();
}

QString DeviceAudioSource::humanName() const
{
    if (m_devicePtr.isNull())
        return deviceName();
    else {
        m_devicePtr->setState(QGst::StatePlaying);
        QString name = m_devicePtr->property ("device-name").toString();
        m_devicePtr->setState(QGst::StatePaused);

        if (name.isEmpty() || name.isNull())
            return deviceName();
        else
            return name;
    }
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
            qDebug() << "[DeviceAudioSource::obtain()] Obtained device" << m_device.toString();
            realSrc->setProperty ("device", m_device.toString());
            QList<QGlib::ParamSpecPtr> properties = realSrc->listProperties();
            Q_FOREACH (QGlib::ParamSpecPtr property, properties) {
                QString name = property->name();
                QGlib::Value value = realSrc->property (property->name().toStdString().c_str());
                qDebug() << "[DeviceAudioSource::obtain()] Property" << name << "=" << value.toString();
            }
        }

        m_devicePtr = m_binPtr->getElementByName ("src");

        if (m_devicePtr.isNull())
            qDebug() << "[DeviceAudioSource::obtain()] Warning! The obtained device pointer is NULL!" << m_device.toString();
        else {
            m_devicePtr->setProperty<const char*> ("client", "SpeechControl");
            m_devicePtr->setProperty<bool> ("do-timestamp", true);
            m_devicePtr->setProperty<int> ("blocksize", -1);
            m_devicePtr->setProperty<bool> ("message-forward", true);
        }
    }
}

DeviceAudioSource::~DeviceAudioSource()
{

}

StreamSource::StreamSource (StreamAudioSource* p_audioSource) : GenericSource (p_audioSource)
{

}

StreamSource::StreamSource (const StreamSource& p_source) : GenericSource (p_source)
{

}

QGst::FlowReturn StreamSource::endOfStream()
{
    return SpeechControl::GenericSource::endOfStream();
}

QGst::FlowReturn StreamSource::pushBuffer (const QGst::BufferPtr& p_buffer)
{
    return SpeechControl::GenericSource::pushBuffer (p_buffer);
}

StreamSource::~StreamSource()
{

}

StreamSink::StreamSink(StreamAudioSource* p_audioSrc) : GenericSink(), m_audioSrc(p_audioSrc)
{

}

StreamSink::StreamSink (const StreamSink& p_other) : GenericSink(p_other)
{

}

StreamSink::StreamSink (const GenericSink& p_other) : GenericSink(p_other)
{

}

void StreamSink::eos()
{
    SpeechControl::GenericSink::eos();
}

QGst::BufferPtr StreamSink::pullBuffer()
{
    quint8 data = 0;
    *(m_audioSrc->stream()) >> data;
    return QGst::Buffer::create(data);
}

StreamSink::~StreamSink()
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

StreamAudioSource::StreamAudioSource (QDataStream* p_stream) : AbstractAudioSource (0), m_strm (p_stream)
{

}

void StreamAudioSource::buildPipeline()
{
    SpeechControl::AbstractAudioSource::buildPipeline();

    // Replace the appsrc used by AbstractAudioSource usually with the stream source.
    m_appSrc = new StreamSource(this);
    m_appSrc->setCaps (QGst::Caps::fromString (caps()));
    m_appSrc->setElement(this->m_srcPtr);
}

QString StreamAudioSource::pipelineDescription() const
{
    return QString ("appsrc name=src ! decodebin");
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
