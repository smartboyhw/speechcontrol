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

#include <QVariant>
#include <QGst/PropertyProbe>
#include <QGst/ElementFactory>

#include "audiosource/deviceprivate.hpp"
#include "audiosource/device.hpp"

using namespace SpeechControl;

QMap<QString, DeviceAudioSource*> DeviceAudioSourcePrivate::s_map;

DeviceAudioSourcePrivate::DeviceAudioSourcePrivate (DeviceAudioSource* p_obj) :
    AbstractAudioSourcePrivate(), device(), devicePtr(), q_ptr (p_obj)
{
    device.clear();
    devicePtr.clear();
}

void DeviceAudioSourcePrivate::obtainDevice (const QString& p_deviceName)
{
    qDebug() << "[DeviceAudioSourcePrivate::obtainDevice()] Obtaining device" << p_deviceName << "...";

    if (!device.isValid())
        device = QGlib::Value::create<QString> (p_deviceName);
    else
        device.set<QString> (p_deviceName);

    q_ptr->buildPipeline();
}

DeviceAudioSourcePrivate::~DeviceAudioSourcePrivate()
{

}

DeviceAudioSource::DeviceAudioSource() :
    AbstractAudioSource (new DeviceAudioSourcePrivate (this))
{
}

DeviceAudioSource::DeviceAudioSource (const QString& p_deviceName) :
    AbstractAudioSource (new DeviceAudioSourcePrivate (this))
{
    Q_D (DeviceAudioSource);
    d->obtainDevice (p_deviceName);
}

/// @todo Fix these copy constructor.
DeviceAudioSource::DeviceAudioSource (const DeviceAudioSource& p_other) :
    AbstractAudioSource (new DeviceAudioSourcePrivate (this), p_other.parent())
{
    Q_D (DeviceAudioSource);
    d->q_ptr = this;
}

/// @todo Fix these copy constructor.
DeviceAudioSource::DeviceAudioSource (const AbstractAudioSource& p_other) :
    AbstractAudioSource (new DeviceAudioSourcePrivate (this), p_other.parent())
{
    Q_D (DeviceAudioSource);
    QString deviceName = p_other.property ("DeviceName").toString();

    if (deviceName.isEmpty() && !deviceName.isNull())
        d->obtainDevice (deviceName);
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

AudioSourceList DeviceAudioSource::allDevices()
{
    const QString audioSrc ("autoaudiosrc");
    QGst::ElementPtr audioSrcPtr = QGst::ElementFactory::make (audioSrc);
    AudioSourceList list;

    if (audioSrcPtr) {
        audioSrcPtr->setState (QGst::StateReady);

        QGst::ChildProxyPtr chldPrxy = audioSrcPtr.dynamicCast<QGst::ChildProxy>();
        QGst::PropertyProbePtr propProbe;

        if (chldPrxy) {
            propProbe = chldPrxy->childByIndex (0).dynamicCast<QGst::PropertyProbe>();
        }

        audioSrcPtr->setState (QGst::StateNull);

        if (propProbe) {
            QList<QGlib::Value> devices = propProbe->probeAndGetValues ("device");

            if (propProbe && propProbe->propertySupportsProbe ("device")) {
                Q_FOREACH (QGlib::Value device, devices) {
                    qDebug() << "[DeviceAudioSource::allDevices()] Found audio device" << device.toString();
                    QList<QGlib::ParamSpecPtr> specs = propProbe->listProperties();

                    Q_FOREACH (const QGlib::ParamSpecPtr spec, specs) {
                        qDebug() << "[DeviceAudioSource::allDevices()] Device:" << device.toString()
                                 << spec->name() << propProbe->property (spec->name().toStdString().c_str()).toString()
                                 << spec->description();
                        ;
                    }

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
    Q_D (const DeviceAudioSource);
    Q_ASSERT (d->device.isValid());

    if (d->device.isValid())
        return d->device.toString();

    return QString::null;
}

QString DeviceAudioSource::humanName() const
{
    Q_D (const DeviceAudioSource);

    if (d->devicePtr.isNull())
        return deviceName();
    else {
        QString name = d->devicePtr->property ("device-name").toString();

        if (name.isEmpty() || name.isNull())
            return deviceName();
        else
            return name;
    }

    return QString::null;
}

QString DeviceAudioSource::pipelineDescription() const
{
    return QString ("alsasrc name=src");
}

void DeviceAudioSource::start()
{
    Q_D(DeviceAudioSource);
    SpeechControl::AbstractAudioSource::start();
    d->devicePtr->setState(QGst::StatePlaying);
}

void DeviceAudioSource::stop()
{
    Q_D(DeviceAudioSource);
    SpeechControl::AbstractAudioSource::stop();
    d->devicePtr->setState(QGst::StateNull);
}

void DeviceAudioSource::buildPipeline()
{
    AbstractAudioSource::buildPipeline();
    Q_D (DeviceAudioSource);

    if (isNull()) {
        qDebug() << "[DeviceAudioSource::buildPipeline()] The base pipeline is NULL.";
        return;
    }
    else {
        d->ptrAudioSource->setState (QGst::StatePaused);
        QGst::ChildProxyPtr childProxy = d->ptrAudioSource.dynamicCast<QGst::ChildProxy>();

        if (childProxy && childProxy->childrenCount() > 0) {
            QGst::ObjectPtr realSrc = childProxy->childByIndex (0);
            qDebug() << "[DeviceAudioSource::obtain()] Obtained device" << d->device.toString();

            QList<QGlib::ParamSpecPtr> properties = realSrc->listProperties();
            Q_FOREACH (QGlib::ParamSpecPtr property, properties) {
                QString name = property->name();
                QGlib::Value value = realSrc->property (property->name().toStdString().c_str());

                if (value.isValid())
                    qDebug() << "[DeviceAudioSource::obtain()] Property" << name << "=" << value.toString();
            }
        }

        d->devicePtr = d->ptrBin->getElementByName ("src");

        if (d->devicePtr.isNull()) {
            qWarning() << "[DeviceAudioSource::buildPipeline()] The obtained device pointer is NULL!" << d->device.toString();
        }
        else {
            qDebug() << "[DeviceAudioSource::buildPipeline()] Setting specific properties for" << d->device.toString() << "...";
            d->devicePtr->setProperty<const char*> ("client", "SpeechControl");
            d->devicePtr->setProperty<bool> ("do-timestamp", true);
            d->devicePtr->setProperty<bool> ("message-forward", true);
            d->devicePtr->setProperty<int> ("blocksize", 1024);
        }
    }
}

DeviceAudioSource::~DeviceAudioSource()
{

}

#include "audiosource/device.moc"
// kate: indent-mode cstyle; replace-tabs on; 
