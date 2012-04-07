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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QStringList>
#include <config.hpp>
#include "plugin.hpp"
#include <app/macros.hpp>
#include <app/services/desktopcontrol/command.hpp>

using SpeechControl::DesktopControl::AbstractCommand;
using SpeechControl::DesktopControl::AbstractCategory;


MPRIS_NAMESPACE_BEGIN

/**
 * @brief Represents the category of commands related to MPRIS.
 **/
class MprisCategory : public AbstractCategory
{
    Q_OBJECT
    Q_DISABLE_COPY (MprisCategory)
    SC_SINGLETON (MprisCategory)
    friend class Plugin;

public:
    virtual const QString id() const;
    virtual const QString title();
};


/**
 * @brief Player command for MPRIS.
 *
 * This command is based partly on the D-Bus specification for MPRIS
 * defined at http://www.mpris.org/2.1/spec/Player_Node.html.
 **/
class PlayerCommand : public AbstractCommand
{
    Q_OBJECT
    Q_DISABLE_COPY (PlayerCommand)
    friend class Plugin;
    friend class MprisCategory;

public:
    virtual QString id();
    virtual bool invoke (const QString& p_statement = QString::null) const;

private:
    explicit PlayerCommand ();
};

/**
 * @brief Library control command for MPRIS.
 *
 * This command is based partly on the D-Bus specification for MPRIS
 * defined at http://www.mpris.org/2.1/spec/Player_Node.html.
 **/
class LibraryCommand : public AbstractCommand
{
    Q_OBJECT
    Q_DISABLE_COPY (LibraryCommand)
    friend class Plugin;
    friend class MprisCategory;

public:
    virtual QString id();
    virtual bool invoke (const QString& p_statement = QString::null) const;

private:
    explicit LibraryCommand ();
};

MPRIS_NAMESPACE_END

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
