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

#include <QtTest/QtTest>

#include <lib/abstractaudiosource.hpp>
#include <lib/system.hpp>

#include "device.h"

using namespace SpeechControl;

void TestDevice::init()
{
    SpeechControl::System::start();

    if (DeviceAudioSource::allDevices().length() == 0)
        QSKIP ("This test requires at least one input device operational on the test environment.", SkipAll);
}

void TestDevice::cleanup()
{
    SpeechControl::System::stop();
}

void TestDevice::listDevices()
{
    AbstractAudioSourceList devices = DeviceAudioSource::allDevices();

    Q_FOREACH (const AbstractAudioSource * device, devices) {
        DeviceAudioSource* mic = (DeviceAudioSource*) device;
        qDebug() << "[TestDevice::listDevices()] " << mic->humanName() << mic->defaultDevice();
    }
}

void TestDevice::defaultDevice()
{
    DeviceAudioSource* defaultDevice = DeviceAudioSource::defaultDevice();
    QCOMPARE (defaultDevice != 0 && !DeviceAudioSource::allDevices().isEmpty(), true);
    QCOMPARE (defaultDevice->isNull() == true, true);
    qDebug() << "[TestDevice::defaultDevice()] Default mic is:" << defaultDevice->humanName() << defaultDevice->deviceName();
}

void TestDevice::adjustVolume()
{
    DeviceAudioSource* defaultDevice = DeviceAudioSource::defaultDevice();
    QCOMPARE (defaultDevice != 0 && !DeviceAudioSource::allDevices().isEmpty(), true);
    QCOMPARE (defaultDevice->isNull() == true, true);
    defaultDevice->setVolume (0.8);
    QCOMPARE (defaultDevice->volume() == 0.8, true);
}

void TestDevice::toggleMuteState()
{
    DeviceAudioSource* defaultDevice = DeviceAudioSource::defaultDevice();
    QCOMPARE (defaultDevice != 0 && !DeviceAudioSource::allDevices().isEmpty(), true);
    const bool l_muted = defaultDevice->isMuted();
    defaultDevice->setMuted (!l_muted);
    QCOMPARE (defaultDevice->isMuted(), !l_muted);
}

QTEST_MAIN (TestDevice)

#include "device.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
