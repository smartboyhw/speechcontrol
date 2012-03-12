/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#include "sessions/session.hpp"
#include "dictation-pane.hpp"
#include "core.hpp"
#include <dictation/agent.hpp>
#include <desktopcontrol/agent.hpp>
#include "ui_settingspane-dictation.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

DictationSettingsPane::DictationSettingsPane():
    m_ui ( new Ui::DictationSettingsPane ) {
        qDebug() << "[DictationSettingsPane::{constructor}] Building dictation settings pane...";
        m_ui->setupUi ( this );
        updateUi();
        qDebug() << "[DictationSettingsPane::{constructor}] Built dictation settings pane.";
    }

DictationSettingsPane::~DictationSettingsPane() {
    delete m_ui;
}

void DictationSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

QString DictationSettingsPane::title() const {
    return "Dictation";
}

QString DictationSettingsPane::id() const {
    return "dctn";
}

bool DictationSettingsPane::containsText ( const QString& p_query ) const {

}

QPixmap DictationSettingsPane::pixmap() const {

}

void DictationSettingsPane::resetPanel() {

}

void DictationSettingsPane::restoreDefaults() {

}

void DictationSettingsPane::updateUi() {
    m_ui->checkBoxEnable->setChecked ( Dictation::Agent::instance()->isEnabled() && !DesktopControl::Agent::instance()->isEnabled() );
    m_ui->checkBoxEnable->setEnabled ( !DesktopControl::Agent::instance()->isEnabled() );
}

void DictationSettingsPane::on_checkBoxEnable_toggled ( bool p_checked ) {
    if ( !DesktopControl::Agent::instance()->isEnabled() ) {
        Core::setConfiguration ( "Dictation/Enabled",p_checked );
        Dictation::Agent::instance()->setState ( ( ( p_checked ) ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled ) );
    }
}

void DictationSettingsPane::on_checkBoxEnableStartup_toggled ( bool p_checked ) {
    if ( !DesktopControl::Agent::instance()->isEnabled() ) {
        Core::setConfiguration ( "Dictation/AutoStart",p_checked );
    }
}

#include "dictation-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
