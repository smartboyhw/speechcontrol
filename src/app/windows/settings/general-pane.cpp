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
#include "general-pane.hpp"
#include "core.hpp"
#include <desktopcontrol/agent.hpp>
#include "ui_settingspane-general.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

GeneralSettingsPane::GeneralSettingsPane ( QWidget *parent ) :
    QFrame ( parent ),
    ui ( new Ui::GeneralSettingsPane ) {
    ui->setupUi ( this );
    ui->lblSessionCount->setText ( QString::number ( Session::allSessions().count() ) );
    ui->lblAccuracyRating->setText ( "<i>n/a</i>" );
    ui->checkBoxEnableDesktopControl->setChecked(Core::configuration("DesktopControl/AutoStart").toBool());
    ui->checkBoxEnableDictation->setChecked(Core::configuration("Dictation/AutoStart").toBool());
}

GeneralSettingsPane::~GeneralSettingsPane() {
    delete ui;
}

void GeneralSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

const QString GeneralSettingsPane::title() const {
    return "General";
}

const QString GeneralSettingsPane::id() const {
    return "gnrl";
}

void GeneralSettingsPane::on_checkBoxEnableDictation_toggled ( bool p_checked ) {
    Core::setConfiguration("Dictation/AutoStart",p_checked );
}

void GeneralSettingsPane::on_checkBoxEnableDesktopControl_toggled ( bool p_checked ) {
    Core::setConfiguration("DesktopControl/AutoStart",p_checked );
}

#include "general-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
