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

#include <QGst/PropertyProbe>
#include <QGst/ElementFactory>

#include "lib/audiosource/device.hxx"
#include "lib/audiosource/device.hpp"

using namespace SpeechControl;

QMap<QString, DeviceAudioSource*> DeviceAudioSourcePrivate::s_map;

DeviceAudioSource::DeviceAudioSource() : AbstractAudioSource (0), d_ptr (new DeviceAudioSourcePrivate(this))
{
}

DeviceAudioSource::DeviceAudioSource (const QString& p_deviceName) : AbstractAudioSource (0), d_ptr (new DeviceAudioSourcePrivate(this))
{
    d_func()->obtainDevice (p_deviceName);
}

DeviceAudioSource::DeviceAudioSource (const DeviceAudioSource& p_other) : AbstractAudioSource (p_other), d_ptr(const_cast<DeviceAudioSourcePrivate*> (p_other.d_ptr.data()))
{
    d_func()->m_deviceObj = this;
}

DeviceAudioSource::DeviceAudioSource (const AbstractAudioSource& p_other) : AbstractAudioSource (p_other), d_ptr(new DeviceAudioSourcePrivate(this))
{
    QString deviceName = p_other.property ("DeviceName").toString();

    if (deviceName.isEmpty() && !deviceName.isNull())
        d_func()->obtainDevice (deviceName);
    else {
        qDebug() << "[DeviceAudioSource::{constructor}] Conversion from a AbstractAudioSource failed.";
        Q_ASSERT (!deviceName.isNull());
    }
}

DeviceAudioSource* DeviceAudioSource::obtain (const QString& p_deviceName)
{
    if (DeviceAudioSourcePrivate::s_map.contains (p_deviceName))
        return DeviceAudioSourcePrivate::s_map.value (p_deviceName);
    else {
        DeviceAudioSource* device = new DeviceAudioSource (p_deviceName);

        if (device->isNull())
            return 0;
        else {
            DeviceAudioSourcePrivate::s_map.insert (p_deviceName, device);
            return device;
        }
    }

    return 0;
}

DeviceAudioSource* DeviceAudioSource::defaultDevice()
{
    return (DeviceAudioSource*) allDevices().first();
}

void DeviceAudioSourcePrivate::obtainDevice (const QString& p_deviceName)
{
    qDebug() << "[DeviceAudioSourcePrivate::obtainDevice()] Obtaining device" << p_deviceName << "...";

    if (!m_device.isValid())
        m_device = QGlib::Value::create<QString> (p_deviceName);
    else
        m_device.set<QString> (p_deviceName);

    m_deviceObj->buildPipeline();

    if (m_devicePtr)
        m_devicePtr->setProperty ("device", m_device);
    else {
        qDebug() << "[DeviceAudioSourcePrivate::obtainDevice()] Failed to set device for use; invalid device pointer.";
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

                    if (!DeviceAudioSourcePrivate::s_map.contains (device.toString()))
                        DeviceAudioSourcePrivate::s_map.insert (device.toString(), new DeviceAudioSource (device.toString()));

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
    Q_ASSERT (d_func()->m_device.isValid());

    if (d_func()->m_device.isValid())
        return d_func()->m_device.toString();
    else
        return QString::null;
}

QString DeviceAudioSource::humanName() const
{
    if (d_func()->m_devicePtr.isNull())
        return deviceName();
    else {
        d_func()->m_devicePtr->setState (QGst::StatePlaying);
        QString name = d_func()->m_devicePtr->property ("device-name").toString();
        d_func()->m_devicePtr->setState (QGst::StatePaused);

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

        QGst::ChildProxyPtr childProxy = d_func()->m_srcPtr.dynamicCast<QGst::ChildProxy>();

        if (childProxy && childProxy->childrenCount() > 0) {
            QGst::ObjectPtr realSrc = childProxy->childByIndex (0);
            qDebug() << "[DeviceAudioSource::obtain()] Obtained device" << d_func()->m_device.toString();
            realSrc->setProperty ("device", d_func()->m_device.toString());
            QList<QGlib::ParamSpecPtr> properties = realSrc->listProperties();
            Q_FOREACH (QGlib::ParamSpecPtr property, properties) {
                QString name = property->name();
                QGlib::Value value = realSrc->property (property->name().toStdString().c_str());
                qDebug() << "[DeviceAudioSource::obtain()] Property" << name << "=" << value.toString();
            }
        }

        d_func()->m_devicePtr = AbstractAudioSource::d_func()->m_binPtr->getElementByName ("src");

        if (d_func()->m_devicePtr.isNull())
            qDebug() << "[DeviceAudioSource::obtain()] Warning! The obtained device pointer is NULL!" << d_func()->m_device.toString();
        else {
            d_func()->m_devicePtr->setProperty<const char*> ("client", "SpeechControl");
            d_func()->m_devicePtr->setProperty<bool> ("do-timestamp", true);
            d_func()->m_devicePtr->setProperty<int> ("blocksize", -1);
            d_func()->m_devicePtr->setProperty<bool> ("message-forward", true);
        }
    }
}

DeviceAudioSource::~DeviceAudioSource()
{

}

#include "audiosource/device.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
