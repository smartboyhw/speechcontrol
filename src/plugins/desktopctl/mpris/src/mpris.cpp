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
#include <app/core.hpp>
#include <QDBusInterface>
#include <qdbusreply.h>

using namespace SpeechControl;
USING_NAMESPACE_MPRIS

Mpris* Mpris::s_inst = 0;

Mpris::Mpris() : QObject (Core::instance()), m_interface (0)
{
    m_interface = new QDBusInterface ("org.mpris.MediaPlayer2",
                                      "/Player",
                                      "org.freedesktop.MediaPlayer"
                                     );
}

void Mpris::play()
{
    m_interface->call ("Pause");
}

void Mpris::nextTrack()
{
    m_interface->call ("Next");
}

void Mpris::pause()
{
    m_interface->call ("Pause");
}

void Mpris::previousTrack()
{
    m_interface->call ("Prev");
}

void Mpris::stop()
{
    m_interface->call ("Stop");
}

void Mpris::setRepeat (const bool p_repeatState)
{
    m_interface->call ("Repeat", p_repeatState);
}

void Mpris::setVolume (const quint8 p_volume)
{
    m_interface->call ("VolumeSet", p_volume);
}

quint8 Mpris::volume()
{
    QDBusReply<quint8> reply = m_interface->call (QDBus::BlockWithGui, "VolumeGet");
    return reply.value();
}

Mpris::~Mpris()
{
    m_interface->deleteLater();
}

#include "mpris.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
