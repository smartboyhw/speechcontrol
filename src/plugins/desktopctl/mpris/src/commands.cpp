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

MPRIS_NAMESPACE_BEGIN

MprisCategory* MprisCategory::s_inst = 0;

MprisCategory::MprisCategory () : AbstractCategory (global())
{
    //addCommand((new ListCommand));
}

const QString MprisCategory::id() const
{
    return "applicationlist";
}

const QString MprisCategory::title()
{
    return "Application List";
}

QString PlayCommand::id()
{
    return "start-applist";
}

PlayCommand::PlayCommand () : AbstractCommand (MprisCategory::instance() ,
            (QStringList() << "Play application"
             << "Run application"
             << "Open application"))
{
}

/// @todo Detect the application name and invoke it.
bool PlayCommand::invoke (const QString& p_statement) const
{
    if (!isValidStatement (p_statement)) {
        return false;
    }

    const QString l_tokenArgument = AbstractCommand::santizeStatement (p_statement);

    return true;
}

#include "commands.moc"

MPRIS_NAMESPACE_END

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
