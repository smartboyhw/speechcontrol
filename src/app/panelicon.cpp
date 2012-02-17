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
/// @todo Fix the conflicting issues here. MOC keeps picking up code in GTK and messing things up.
//#include <libappindicator/app-indicator.h>

#include "core.hpp"
#include "panelicon.hpp"

using namespace SpeechControl;

PanelIcon* PanelIcon::s_inst = 0;

/// @bug The icon isn't showing. I cheated by having the Main window add its icon when created.
PanelIcon::PanelIcon() :
    QSystemTrayIcon(Core::instance())
{
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/logo/sc"), QSize(), QIcon::Normal, QIcon::Off);
    setIcon(icon);
    setToolTip("SpeechControl");
    //AppIndicator* m_indicator = app_indicator_new("spchcntrl","accessibilty",APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);
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
