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

APPLIST_NAMESPACE_BEGIN

ApplicationListCategory* ApplicationListCategory::s_inst = 0;

ApplicationListCategory::ApplicationListCategory () : AbstractCategory (global())
{
}

const QString ApplicationListCategory::id() const
{
    return "applicationlist";
}

ApplicationListCategory* ApplicationListCategory::instance()
{
    if (s_inst == 0) {
        s_inst = new ApplicationListCategory;
    }

    return s_inst;
}

const QString ApplicationListCategory::title()
{
    return "Application List";
}

QString StartCommand::id()
{
    return "start-applist";
}

StartCommand::StartCommand () : AbstractCommand (ApplicationListCategory::instance() ,
            (QStringList() << "Start application"
             << "Run application"
             << "Open application"))
{
}

/// @todo Detect the application name and invoke it.
bool StartCommand::invoke (const QString& p_statement) const
{
    if (!isValidStatement (p_statement)) {
        return false;
    }

    const QString tokenArgument = AbstractCommand::obtainArgumentFromStatement (p_statement);

    return true;
}

#include "commands.moc"

APPLIST_NAMESPACE_END

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
