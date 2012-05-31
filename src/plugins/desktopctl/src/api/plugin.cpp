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
#include <QAction>
#include <QMenu>

#include <app/core.hpp>
#include <app/services/module.hpp>
#include "service.hpp"
#include "plugin.hpp"
#include "settings-pane.hpp"
#include <ui/settings-dialog.hpp>

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

Plugin::Plugin (QObject* parent) : AbstractPlugin (PLUGIN_ID, parent), m_actionToggle (0),
    m_actionOptions (0), m_menuDesktopControl (0)
{
    m_menuDesktopControl = new QMenu ("Desktop Control", 0);
    m_actionOptions = m_menuDesktopControl->addAction (QIcon::fromTheme ("configure"), "Configure..",
                      this, SLOT (doMenuConfigure()));
    m_actionToggle = m_menuDesktopControl->addAction ("Toggle", this, SLOT (doMenuToggle (bool)));
    m_actionToggle->setCheckable (true);
    m_actionToggle->setChecked (DesktopControl::Service::instance()->isEnabled());
}

void Plugin::initialize()
{
    const bool dsktpCntrlState = Core::configuration ("DesktopControl/AutoStart", false).toBool();
    DesktopControl::Service::instance()->setState ( (dsktpCntrlState) ? AbstractServiceModule::Enabled  : AbstractServiceModule::Disabled);
    Windows::SettingsDialog::addPane(new Windows::DesktopControlSettingsPane);
}

void Plugin::deinitialize()
{
    qDebug() << "Plug-in unloaded! (dsktpctlapi)";
}

QPixmap Plugin::pixmap() const
{
    return QIcon::fromTheme ("help").pixmap (64, 64);
}

void Plugin::doMenuToggle (bool& p_checked)
{
    p_checked ? DesktopControl::Service::instance()->start() : DesktopControl::Service::instance()->stop();
}

void Plugin::doMenuOptions()
{
    Windows::SettingsDialog::displayPane("dskctl");
}


Plugin::~Plugin()
{

}

#include "plugin.moc"

Q_EXPORT_PLUGIN2 (spchcntrl-dskctlapi, SpeechControl::DesktopControl::Plugin)

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
