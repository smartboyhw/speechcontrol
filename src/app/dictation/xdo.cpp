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
#include <QDebug>

#include "xdo.hxx"

using SpeechControl::Core;
using SpeechControl::Dictation::KeyboardEmulator;

KeyboardEmulator* KeyboardEmulator::s_inst = 0;

KeyboardEmulator::KeyboardEmulator() : QObject (Core::instance()), m_xdo (0),
    m_win (0)
{
    m_xdo = xdo_new (0);
    SC_ASSERT (m_xdo != 0, "Failed to create 'xdo' instance.");

    int l_x, l_y;
    qDebug() << "[KeyboardEmulator::{constructor}] " << xdo_mouselocation (m_xdo, &l_x, &l_y, 0);
}

/// @todo Allow the value of the delay to be adjustable.
bool KeyboardEmulator::sendKey (const QChar& p_character)
{
    qDebug() << "[Dictation::KeyboardEmulator] Echoing character " << p_character;
    const char l_char = p_character.toAscii();
    xdo_type (m_xdo, m_win, &l_char, 1);
    return false;
}

bool KeyboardEmulator::sendKeys (const QString& p_characters)
{
    Q_FOREACH (const QChar l_char, p_characters) {
        if (!sendKey (l_char)) {
            return false;
        }
    }

    return false;
}

KeyboardEmulator::~KeyboardEmulator()
{

}

#include "xdo.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
