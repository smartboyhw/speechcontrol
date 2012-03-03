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
#include "core.hpp"
#include "indicator.hpp"

using SpeechControl::Indicator;
using SpeechControl::Core;

Indicator* Indicator::s_inst = 0;

/// @todo Check for a configuration value to determine whether or not the indicator should be shown on initialization.
Indicator::Indicator ( QObject* parent ) : QObject ( parent ),
    m_indctr(0), m_indctrSvr(0) {
    s_inst = this;

    m_indctrSvr = QIndicate::Server::defaultInstance();
    m_indctrSvr->setType("speechcontrol");
    //m_indicateServer->setDesktopFile();
    m_indctrSvr->show();

    m_indctr = new QIndicate::Indicator(m_indctrSvr);
    m_indctr->setNameProperty("SpeechControl");
    m_indctr->setTimeProperty(QDateTime::currentDateTime());
    m_indctr->setDrawAttentionProperty(true);
    m_indctr->show();

    connect(m_indctr, SIGNAL(display(QIndicate::Indicator*)), SLOT(displayIndicator(QIndicate::Indicator*)));
    connect(m_indctrSvr, SIGNAL(serverDisplay()), SLOT(showMainWindow()));

    presentMessage ( "Test message!" );
}

/// @todo Implement the appropriate code using QtIndicate to hide the indicator.
void Indicator::hide() {
    instance()->m_indctr->hide();
}

/// @todo Implement the appropriate code using QtIndicate to show the indicator.
void Indicator::show() {
    instance()->m_indctr->show();
    instance()->m_indctr->emitDisplay();
}

void Indicator::showMainWindow() {
    Core::mainWindow()->show();
}

void Indicator::displayIndicator ( QIndicate::Indicator* p_indctr ) {
    p_indctr->show();
}

Indicator* Indicator::instance() {
    if ( s_inst == 0 )
        s_inst = new Indicator ( Core::instance() );

    return s_inst;
}

/// @todo Implement the appropriate code using QtIndicate to present a non-blocking message to the user.
void Indicator::presentMessage ( const QString& p_message ) {
    qDebug() << "Indicator message:" << p_message;
    instance()->m_indctr->emitDisplay();
}

Indicator::~Indicator() {

}

#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
