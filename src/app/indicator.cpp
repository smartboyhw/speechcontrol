/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it -under the terms of the GNU Library General Public
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

#include <QImage>
#include <QDateTime>
#include <QDebug>

// Local
#include "core.hpp"
#include "indicator.hpp"
#include "windows/main-window.hpp"
#include <ui_main-window.h>

// Qt
#include <QImage>
#include <QMenu>
#include <QDebug>

using SpeechControl::Indicator;
using SpeechControl::Core;

Indicator* Indicator::s_inst = 0;

/// @todo Check for a configuration value to determine whether or not the indicator should be shown on initialization.
Indicator::Indicator () : QObject (Core::instance()),
    m_icon (0)
{
    s_inst = this;

    m_icon = new QSystemTrayIcon (icon(), this);
    buildMenu();
}

void Indicator::buildMenu()
{
    QMenu* l_menu = new QMenu;
    QMenu* menuDesktopControl = l_menu->addMenu (Core::mainWindow()->m_ui->menuDesktopControl->icon(),
                                                 Core::mainWindow()->m_ui->menuDesktopControl->title());
    QMenu* menuDictation      = l_menu->addMenu (Core::mainWindow()->m_ui->menuDictation->icon(),
                                                 Core::mainWindow()->m_ui->menuDictation->title());
    QMenu* menuPlugins        = l_menu->addMenu (Core::mainWindow()->m_ui->menuPlugins->icon(),
                                                 Core::mainWindow()->m_ui->menuPlugins->title());
    QMenu* menuHelp           = l_menu->addMenu (Core::mainWindow()->m_ui->menuHelp->icon(),
                                                 Core::mainWindow()->m_ui->menuHelp->title());

    menuDesktopControl->addActions (Core::mainWindow()->m_ui->menuDesktopControl->actions());
    menuDictation->addActions (Core::mainWindow()->m_ui->menuDictation->actions());
    menuPlugins->addActions (Core::mainWindow()->m_ui->menuPlugins->actions());
    menuHelp->addActions (Core::mainWindow()->m_ui->menuHelp->actions());

    l_menu->addMenu (menuDesktopControl);
    l_menu->addMenu (menuDictation);
    l_menu->addMenu (menuPlugins);
        l_menu->addSeparator();
    l_menu->addMenu (menuHelp);
    l_menu->addAction (Core::mainWindow()->m_ui->actionOptions);
        l_menu->addSeparator();
    l_menu->addAction ("Restore", Core::mainWindow(), SLOT (open()));
    l_menu->addAction (QIcon::fromTheme ("application-exit"), "Quit", QApplication::instance(), SLOT (quit()));

    m_icon->setContextMenu (l_menu);
}


QIcon Indicator::icon()
{
    const QString state = Core::configuration ("Indicator/Icon").toString();
    qDebug() << "[Indicator::icon] " << state;

    if (state == "White")
        return QIcon (":/indicator/white");
    else if (state == "Black")
        return QIcon (":/indicator/black");
    else if (state == "Default")
        return QIcon (":/logo/sc");

    return QApplication::windowIcon();
}

void Indicator::hide()
{
    instance()->m_icon->hide();
}

void Indicator::show()
{
    instance()->m_icon->setIcon (icon().pixmap (48, 48));
    instance()->m_icon->show();
}

void Indicator::showMainWindow()
{
    Core::mainWindow()->open();
}

/// @todo Add an enumeration that allows the callee to specify the kind of message icon they'd  want to appear.
void Indicator::presentMessage (const QString& p_title, const QString& p_message, const int& p_timeout)
{
    instance()->m_icon->showMessage (p_title, p_message, QSystemTrayIcon::Information, p_timeout);
}

bool Indicator::isVisible()
{
    return m_icon->isVisible();
}

Indicator::~Indicator()
{

}

#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
