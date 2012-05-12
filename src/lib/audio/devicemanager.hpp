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


#ifndef DEVICEMANAGER_HPP
#define DEVICEMANAGER_HPP

#include <QtCore/QObject>
#include <QStringList>
#include <QString>
#include <QMap>

namespace SpeechControl
{

class System;

namespace Audio
{

class DeviceManager : public QObject
{
    Q_OBJECT
    friend class SpeechControl::System;
    
public:
    explicit DeviceManager(QObject* parent = 0);
    virtual ~DeviceManager();
    
    static void add(QString device);
    static void chooseDevice(QString device);
    static void chooseDevice(int deviceID);
    static void setMux(QString audioType);

    static QString device();
    static QString mux();
    static QString audioFormat();
    
private:
    static QStringList devices;
    static QString chosenDevice;
    static QString chosenFormat;
    static QString chosenMux;
    
    static QMap<QString, QString> muxers;
    static void setup();
};

}
}
#endif // DEVICEMANAGER_HPP
