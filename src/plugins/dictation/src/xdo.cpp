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

#include <QDebug>
#include <hunspell.hxx>

#include "core.hpp"
#include "xdo.hpp"
#include "xdoprivate.hpp"

using SpeechControl::Core;
using SpeechControl::Dictation::KeyboardEmulator;

KeyboardEmulator* KeyboardEmulator::s_inst = 0;

KeyboardEmulator::KeyboardEmulator() : QObject (Core::instance()), m_xdo (0),
    m_win (0), m_spllchk (0)
{
    m_xdo = xdo_new (0);
    SC_ASSERT (m_xdo != 0, "Failed to create 'xdo' instance.");

    m_spllchk = new Hunspell ("", "");
}

/// @todo Allow the value of the delay to be adjustable.
bool KeyboardEmulator::sendKey (const QChar& p_character)
{
    qDebug() << "[Dictation::KeyboardEmulator::sendKey()] Echoing character " << p_character;
    const char l_char = p_character.toAscii();
    const int val = xdo_type (m_xdo, m_win, &l_char, 100);
    qDebug() << "[Dictation::KeyboardEmulator::sendKey()] xdo_type " <<  val;
    return false;
}

bool KeyboardEmulator::sendPhrase (const QString& p_characters)
{
    // for now, just send the entire phrase.
    qDebug() << "[Dictation::KeyboardEmulator::sendKeys()] Echoing phrase " << p_characters << "...";
    QString fixedPhrase = p_characters.toCaseFolded();
    fixedPhrase[0] = fixedPhrase.at (0).toUpper();

    const int val = xdo_type (m_xdo, m_win, fixedPhrase.toStdString().c_str(), 100);
    qDebug() << "[Dictation::KeyboardEmulator::sendKeys()] xdo_type " <<  val << "(text:" << val << ")";
    return true;
}

KeyboardEmulator::~KeyboardEmulator()
{

}

#include "xdo.moc"
// kate: indent-mode cstyle; replace-tabs on;
