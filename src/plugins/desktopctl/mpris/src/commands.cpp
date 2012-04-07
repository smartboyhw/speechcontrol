/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *            (C) 2012 Jacky Alcine <jacky.alcine@thesii.org>
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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

// local includes
#include "commands.hpp"
#include "mpris.hpp"

USING_NAMESPACE_MPRIS

MprisCategory* MprisCategory::s_inst = 0;

MprisCategory::MprisCategory () : AbstractCategory (global())
{
}

const QString MprisCategory::id() const
{
    return "mpris";
}

const QString MprisCategory::title()
{
    return "MPRIS";
}

QString PlayerCommand::id()
{
    return "mpris-player";
}

PlayerCommand::PlayerCommand () : AbstractCommand (MprisCategory::instance() ,
            QStringList() << tr ("Play music")
            << tr ("Pause music")
            << tr ("Resume playing music")
            << tr ("Stop playing music")
            << tr ("Stop music")
            << tr ("Set volume to")
            << tr ("Next track")
            << tr ("Repeat tracks")
            << tr ("Stop repeating tracks")
            << tr ("Previous track"))
{
}

/// @todo Detect the arguments.
bool PlayerCommand::invoke (const QString& p_statement) const
{
    qDebug() << "[PlayerCommand::invoke()] Invoking" << p_statement;
    if (!isValidStatement (p_statement)) {
        qDebug() << "[PlayerCommand::invoke()] Invalid statement" << p_statement;
        return false;
    }

    const QString tokenArgument = AbstractCommand::obtainArgumentFromStatement (p_statement);
    const QString command = AbstractCommand::obtainCommandFromStatement (p_statement).toLower();

    if (command == "play music" || command == "resume playing music") {
        qDebug() << "[PlayerCommand::invoke()] Playing music...";
        DBus::Player::instance()->play();
    }
    else if (command == "pause music") {
        qDebug() << "[PlayerCommand::invoke()] Pausing music...";
        DBus::Player::instance()->pause();
    }
    else if (command == "stop playing music" || command == "stop music") {
        qDebug() << "[PlayerCommand::invoke()] Stopping music...";
        DBus::Player::instance()->stop();
    }
    else if (command == "set volume to") {
        /// @todo Sanitize the argument to check if they said a percentage or some weird value.
        qDebug() << "[PlayerCommand::invoke()] Set volume of music to " << tokenArgument << "...";
        DBus::Player::instance()->setVolume (tokenArgument.toUInt());
    }
    else if (command == "next track") {
        qDebug() << "[PlayerCommand::invoke()] Advancing to next track...";
        DBus::Player::instance()->nextTrack();
    }
    else if (command == "previous track") {
        qDebug() << "[PlayerCommand::invoke()] Going back a track...";
        DBus::Player::instance()->previousTrack();
    }
    else if (command == "stop repeating tracks") {
        qDebug() << "[PlayerCommand::invoke()] Disabling repeat...";
        DBus::Player::instance()->setRepeat (false);
    }
    else if (command == "repeat tracks") {
        qDebug() << "[PlayerCommand::invoke()] Enabling repeat...";
        DBus::Player::instance()->setRepeat (true);
    }
    else {
        qDebug() << "[PlayerCommand::invoke()] No matching commands found.";
        return false;
    }

    return true;
}

LibraryCommand::LibraryCommand() : AbstractCommand (MprisCategory::instance(),
            QStringList() << tr ("Play track")
            << tr ("Play album")
            << tr ("Play all by artist")
            << tr ("Play genre"))
{

}

QString LibraryCommand::id()
{
    return "mpris-library";
}

bool LibraryCommand::invoke (const QString& p_statement) const
{
    if (!isValidStatement (p_statement)) {
        return false;
    }

    const QString tokenArgument = AbstractCommand::obtainArgumentFromStatement (p_statement);

    return true;
}

#include "commands.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
