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

// Qt
#include <QImage>
#include <QMenu>
#include <QDebug>

using SpeechControl::Indicator;
using SpeechControl::Core;

Indicator* Indicator::s_inst = 0;

/// @todo Check for a configuration value to determine whether or not the indicator should be shown on initialization.
Indicator::Indicator ( QObject* parent ) : QObject ( parent ),
    m_icon(0) {
    s_inst = this;

    m_icon = new QSystemTrayIcon(QIcon(":/logo/sc-large"),this);
    QMenu* l_menu = new QMenu;
    l_menu->addAction("Restore",Core::mainWindow(),SLOT(show()));
    l_menu->addAction(QIcon::fromTheme("application-exit"),"Quit",QApplication::instance(),SLOT(quit()));
    m_icon->setContextMenu(l_menu);
}

/// @todo Implement the appropriate code using QtIndicate to hide the indicator.
void Indicator::hide() {
    instance()->m_icon->hide();
}

/// @todo Implement the appropriate code using QtIndicate to show the indicator.
void Indicator::show() {
    instance()->m_icon->show();
}

void Indicator::showMainWindow() {
    Core::mainWindow()->show();
}

Indicator* Indicator::instance() {
    if ( s_inst == 0 ) {
        s_inst = new Indicator ( Core::instance() );
    }

    return s_inst;
}

/// @todo Add an enumeration that allows the callee to specify the kind of message icon they'd  want to appear.
void Indicator::presentMessage ( const QString& p_title, const QString& p_message, const int& p_timeout ) {
    instance()->m_icon->showMessage(p_title,p_message,QSystemTrayIcon::Information,p_timeout);
}

Indicator::~Indicator() {

}

#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
