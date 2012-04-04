/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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

#include "lib/audiosource/device.hpp"

using namespace SpeechControl;

QMap<QString, DeviceAudioSource*> DeviceAudioSource::s_map;

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
    QString deviceName = p_other.property ("DeviceName").toString();

    if (deviceName.isEmpty() && !deviceName.isNull())
        obtainDevice (deviceName);
    else {
        qDebug() << "[DeviceAudioSource::{constructor}] Conversion from a AbstractAudioSource failed.";
        Q_ASSERT (!deviceName.isNull());
    }
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

AudioSourceList DeviceAudioSource::allDevices()
{
    const QString audioSrc ("autoaudiosrc");
    QGst::ElementPtr audioSrcPtr = QGst::ElementFactory::make (audioSrc);
    AudioSourceList list;

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
                    //qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << device.toString();
                    QList<QGlib::ParamSpecPtr> specs = propProbe->listProperties();
                    propProbe->setProperty ("device", device);

                    /*Q_FOREACH (const QGlib::ParamSpecPtr spec, specs) {
                     *                        qDebug() << "[DeviceAudioSource::allDevices()] Device:" << device.toString()
                     *                                 << spec->name() << propProbe->property (spec->name().toStdString().c_str()).toString()
                     *                                 << spec->description();
                     *                        ;
                    }*/

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
    Q_ASSERT (m_device.isValid());

    if (m_device.isValid())
        return m_device.toString();
    else
        return QString::null;
}

QString DeviceAudioSource::humanName() const
{
    if (m_devicePtr.isNull())
        return deviceName();
    else {
        m_devicePtr->setState (QGst::StatePlaying);
        QString name = m_devicePtr->property ("device-name").toString();
        m_devicePtr->setState (QGst::StatePaused);

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

#include "audiosource/device.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
