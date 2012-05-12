/*
    Copyright (c) 2012, Synthetic Intellect Institute <sii@thesii.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Synthetic Intellect Institute nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Synthetic Intellect Institute <sii@thesii.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Synthetic Intellect Institute <sii@thesii.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "devicemanager.hpp"

#include <QDebug>

using namespace SpeechControl::Audio;

QStringList DeviceManager::devices;
QString DeviceManager::chosenDevice;
QMap<QString, QString> DeviceManager::muxers;
QString DeviceManager::chosenMux;

void DeviceManager::setup()
{
    DeviceManager::muxers.insert("Wav", "wavenc");
    DeviceManager::muxers.insert("Ogg", "oggmux");
    DeviceManager::muxers.insert("Avi", "avimux");
}

void DeviceManager::add ( QString device )
{
    DeviceManager::devices.append(device);
}

void DeviceManager::chooseDevice ( QString device )
{
    chosenDevice = device;
}

void DeviceManager::chooseDevice ( int deviceID )
{
    chosenDevice = devices.at(deviceID);
}

void DeviceManager::setMux ( QString audioType )
{
    chosenMux = muxers.value(audioType);
}

QString DeviceManager::device()
{
    return chosenDevice;
}

QString DeviceManager::mux()
{
    return chosenMux;
}

DeviceManager::DeviceManager ( QObject* parent ) : QObject ( parent )
{

}

DeviceManager::~DeviceManager()
{

}

#include "devicemanager.moc"
