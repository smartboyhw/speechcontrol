/***
 *  This file is part of SpeechControl.
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

Indicator::Indicator ( QObject* parent ) : QObject ( parent ), m_indctr ( new QIndicate::Indicator ( this ) ) {
    s_inst = this;

    m_indctr->setIconProperty ( QImage ( ":/logo/sc-large" ) );
    m_indctr->setNameProperty ( "SpeechControl" );
}

void Indicator::hide() {
    instance()->m_indctr->hide();
}

void Indicator::show() {
    instance()->m_indctr->show();
}

Indicator* Indicator::instance() {
    if ( s_inst == 0 )
        s_inst = new Indicator ( Core::instance() );

    return s_inst;
}

void Indicator::showMessage ( const QString& p_message ) {

}

Indicator::~Indicator() {

}

#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
