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

#include <QDebug>

#include <app/core.hpp>
#include <app/indicator.hpp>
#include <app/sessions/session.hpp>
#include <app/desktopcontrol/agent.hpp>

#include "general-pane.hpp"
#include "ui_settingspane-general.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

GeneralSettingsPane::GeneralSettingsPane ():
    ui ( new Ui::GeneralSettingsPane ) {
    qDebug() << "[GeneralSettingsPane::{constructor}] Building general settings pane...";
    ui->setupUi ( this );
    updateUi();
    qDebug() << "[GeneralSettingsPane::{constructor}] Built general settings pane.";
}

GeneralSettingsPane::~GeneralSettingsPane() {
    delete ui;
}

void GeneralSettingsPane::changeEvent ( QEvent *e ) {
    QWidget::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

QString GeneralSettingsPane::title() const {
    return "General";
}

QString GeneralSettingsPane::id() const {
    return "gnrl";
}

bool GeneralSettingsPane::containsText ( const QString& p_query ) const {

}

QPixmap GeneralSettingsPane::pixmap() const {
    return QIcon::fromTheme ( "configure" ).pixmap ( 32,32 );
}

void GeneralSettingsPane::resetPanel() {

}

void GeneralSettingsPane::restoreDefaults() {
    Core::setConfiguration ( "Indicator/Show",false );
    updateUi();
}

void GeneralSettingsPane::updateUi() {
    ui->lblSessionCount->setText ( QString::number ( Session::allSessions().count() ) );
    ui->lblAccuracyRating->setText ( "<i>n/a</i>" );
    ui->checkBoxIndicatorIcon->setChecked ( Core::configuration ( "DesktopControl/AutoStart" ).toBool() );
}

void GeneralSettingsPane::on_checkBoxIndicatorIcon_toggled ( bool p_checked ) {
    Core::setConfiguration ( "Indicator/Show",p_checked );
    ( p_checked ) ? Indicator::show() : Indicator::hide();
}
#include "general-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
