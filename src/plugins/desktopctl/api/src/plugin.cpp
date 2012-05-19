/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/13/12 4:01:09 PM
 */

#include <QDebug>
#include <QtPlugin>
#include <QIcon>

#include <app/core.hpp>
#include "service.hpp"
#include "plugin.hpp"

DSKTPCTLAPI_NAMESPACE_BEGIN

Plugin::Plugin (QObject* parent) : AbstractPlugin (PLUGIN_ID, parent)
{
}

void Plugin::initialize()
{
    const bool dsktpCntrlState = Core::configuration ("DesktopControl/AutoStart", false).toBool();
    DesktopControl::Service::instance()->setState ( (dsktpCntrlState) ? AbstractModule::Enabled  : AbstractModule::Disabled);
    qDebug() << "Plug-in loaded! (dsktpctlapi)";
}

void Plugin::deinitialize()
{
    qDebug() << "Plug-in unloaded! (dsktpctlapi)";
}

QPixmap Plugin::pixmap() const
{
    return QIcon::fromTheme ("help").pixmap (64, 64);
}

Plugin::~Plugin()
{

}

#include "plugin.moc"

DSKTPCTLAPI_NAMESPACE_END

Q_EXPORT_PLUGIN2 (spchcntrl-dsktpctlapi, SpeechControl::Plugins::DesktopControl::Plugin)

// kate: indent-mode cstyle; replace-tabs on;
