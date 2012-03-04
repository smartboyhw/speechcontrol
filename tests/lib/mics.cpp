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
#include "mics.h"
#include <lib/microphone.hpp>
#include <lib/system.hpp>

using namespace SpeechControl;

void TestMicrophone::init() {
    SpeechControl::System::start();
}

void TestMicrophone::cleanup() {
    SpeechControl::System::stop();
}

void TestMicrophone::listMicrophones() {
    MicrophoneList l_mics = Microphone::allMicrophones();

    Q_FOREACH ( const Microphone* l_mic, l_mics ) {
        qDebug() << "(mic)" << l_mic->name();
    }
}

Microphone* TestMicrophone::defaultMicrophone() {
    Microphone* l_defaultMic = Microphone::defaultMicrophone();
    QCOMPARE ( l_defaultMic != 0 && !Microphone::allMicrophones().isEmpty(),true );
    QCOMPARE ( l_defaultMic->isValid() == true,true );
    qDebug() << "Default mic is:" << l_defaultMic->name();

    return l_defaultMic;
}

void TestMicrophone::adjustMicrophone() {
    Microphone* l_defaultMic = defaultMicrophone();
    l_defaultMic->setVolume ( 0.8 );
    QCOMPARE ( l_defaultMic->volume() == 0.8,true );
}

QTEST_MAIN ( TestMicrophone )
#include "mics.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
