/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QIcon>
#include <QApplication>

// libappindicator includes

#include "core.hpp"
#include "panelicon.hpp"

using namespace SpeechControl;

PanelIcon* PanelIcon::s_inst = 0;

/// @bug The icon isn't showing. I cheated by having the Main window add its icon when created.
PanelIcon::PanelIcon() :
    QSystemTrayIcon(Core::instance()), m_menu(0)
{
    m_menu = new QMenu("SpeechControl",QApplication::activeWindow());
    QAction* l_actionClose = m_menu->addAction("Close");
    connect(l_actionClose,SIGNAL(triggered()),QApplication::activeWindow(),SLOT(hide()));
    setIcon(QIcon("qrc:///logo/sc"));
    setToolTip("SpeechControl");
    setContextMenu(m_menu);
}

PanelIcon::~PanelIcon() {
    QSystemTrayIcon::hide();
}

PanelIcon* PanelIcon::instance()
{
    if (s_inst == 0)
        s_inst = new PanelIcon;

    return s_inst;
}

/// @todo Change the configuration option for this.
void PanelIcon::setVisible(const bool &p_vsbl)
{
    QSystemTrayIcon::setVisible(p_vsbl);
}
