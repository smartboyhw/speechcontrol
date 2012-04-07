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

#include "mpris.hpp"
#include <core.hpp>
#include <QDBusInterface>
#include <qdbusreply.h>

using namespace SpeechControl;
USING_NAMESPACE_MPRIS

using DBus::Player;

Player* Player::s_inst = 0;

Player::Player() : QObject (Core::instance()), m_interface (0)
{
    m_interface = new QDBusInterface ("org.mpris.MediaPlayer2",
                                      "/org/mpris/MediaPlayer2",
                                      "org.mpris.MediaPlayer2"
                                     );
    play();
    pause();
    setVolume (100);
    setVolume (40);
}

void Player::play()
{
    m_interface->call (QDBus::BlockWithGui, "Pause");
}

void Player::nextTrack()
{
    m_interface->call (QDBus::BlockWithGui, "Next");
}

void Player::pause()
{
    m_interface->call (QDBus::BlockWithGui, "Pause");
}

void Player::previousTrack()
{
    m_interface->call (QDBus::BlockWithGui, "Prev");
}

void Player::stop()
{
    m_interface->call (QDBus::BlockWithGui, "Stop");
}

void Player::setRepeat (const bool p_repeatState)
{
    m_interface->call (QDBus::BlockWithGui, "Repeat", p_repeatState);
}

void Player::setVolume (const quint8 p_volume)
{
    m_interface->call (QDBus::BlockWithGui, "VolumeSet", p_volume);
}

quint8 Player::volume()
{
    QDBusReply<quint8> reply = m_interface->call (QDBus::BlockWithGui, "VolumeGet");
    return reply.value();
}

Player::~Player()
{
    m_interface->deleteLater();
}

#include "mpris.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
