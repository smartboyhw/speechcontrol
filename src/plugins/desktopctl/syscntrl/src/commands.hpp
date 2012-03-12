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
#include <app/desktopcontrol/agent.hpp>
#include <app/desktopcontrol/command.hpp>

using SpeechControl::DesktopControl::AbstractCommand;
using SpeechControl::DesktopControl::AbstractCategory;


APPLIST_NAMESPACE_BEGIN

/**
 * @brief ...
 **/
class ApplicationListCategory : public AbstractCategory
{
    Q_OBJECT
    friend class Plugin;

public:
    /**
     * @brief ...
     *
     * @return const QString
     **/
    virtual const QString id() const;
    /**
     * @brief ...
     *
     * @return const QString
     **/
    virtual const QString title();
    /**
     * @brief ...
     *
     * @return :Plugins::DesktopControl::ApplicationListing::ApplicationListCategory*
     **/
    static ApplicationListCategory* instance();

private:
    /**
     * @brief ...
     *
     **/
    explicit ApplicationListCategory ();
    static ApplicationListCategory* s_inst;
};

/**
 * @brief ...
 **/
class StartCommand : public AbstractCommand
{
    Q_OBJECT
    friend class Plugin;

public:
    /**
     * @brief ...
     *
     * @return QString
     **/
    virtual QString id();
    /**
     * @brief ...
     *
     * @param p_statement ... Defaults to QString::null.
     * @return bool
     **/
    virtual bool invoke (const QString& p_statement = QString::null) const;

private:
    /**
     * @brief ...
     *
     **/
    explicit StartCommand ();
    QStringList m_cmds;
};

APPLIST_NAMESPACE_END

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
