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

#include "xdo.hpp"
#include "core.hpp"

using SpeechControl::Core;
using SpeechControl::Dictation::KeyboardEmulator;

KeyboardEmulator* KeyboardEmulator::s_inst = 0;

/// @bug We can't use libxdo because one of the methods in the header uses a C++ keyword (class). This is not only a serious problem, it prevents us from using the library in C++ code altogether.
KeyboardEmulator::KeyboardEmulator() : QObject ( Core::instance() ) {
//    m_xdo = xdo_new(0);
//    Q_ASSERT ( m_xdo != 0 );

    int l_x, l_y;
//    qDebug() << xdo_mouselocation ( m_xdo,&l_x,&l_y,0 );
}

KeyboardEmulator* KeyboardEmulator::instance() {
    if ( s_inst == 0 ) {
        s_inst = new KeyboardEmulator;
    }

    return s_inst;
}

bool KeyboardEmulator::sendKey ( const QChar& p_character ) {
    return false;
}

bool KeyboardEmulator::sendKeys ( const QString& p_characters ) {
    Q_FOREACH ( const QChar l_char, p_characters ) {
        if ( !sendKey ( l_char ) ) {
            return false;
        }
    }

    return false;
}

KeyboardEmulator::~KeyboardEmulator() {

}

#include "xdo.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
