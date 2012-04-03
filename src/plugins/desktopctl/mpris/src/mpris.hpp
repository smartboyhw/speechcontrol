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

#ifndef SPCHCNTRL_MPRIS_MPRIS_HPP
#define SPCHCNTRL_MPRIS_MPRIS_HPP

#include <QObject>

#include <app/macros.hpp>

#include "config.hpp"

class QDBusInterface;

MPRIS_NAMESPACE_BEGIN
namespace DBus {
class Player : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Player)
    SC_SINGLETON(Player)

public:
    virtual ~Player();
    void play();
    void pause();
    void stop();
    void nextTrack();
    void previousTrack();

    void setRepeat(const bool p_repeatState);
    void setVolume(const quint8 p_volume);

    quint8 volume();

private:
    QDBusInterface* m_interface;
};

}

MPRIS_NAMESPACE_END
#endif // MPRIS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
