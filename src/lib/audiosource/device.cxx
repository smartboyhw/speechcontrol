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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "lib/audiosource/device.hxx"
#include "lib/audiosource/device.hpp"

using namespace SpeechControl;

DeviceAudioSourcePrivate::DeviceAudioSourcePrivate (DeviceAudioSource* p_obj) :
    AbstractAudioSourcePrivate(), m_device(), m_devicePtr(), m_deviceObj (p_obj)
{
    m_device.clear();
    m_devicePtr.clear();
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

DeviceAudioSourcePrivate::~DeviceAudioSourcePrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
