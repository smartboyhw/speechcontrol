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

#include "training-pane.hpp"
#include "session-pane.hpp"
#include "content-pane.hpp"
#include "ui_settingspane-training.h"

using namespace SpeechControl::Windows;

TrainingSettingsPane::TrainingSettingsPane():
    ui ( new Ui::TrainingSettingsPane ) {
        qDebug() << "[TrainingSettingsPane::{constructor}] Building training settings pane...";
        ui->setupUi ( this );
        addPane(new SessionSettingsPane);
        addPane(new ContentSettingsPane);
        updateUi();
        qDebug() << "[TrainingSettingsPane::{constructor}] Built training settings pane.";
    }

TrainingSettingsPane::~TrainingSettingsPane() {
    delete ui;
}

void TrainingSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

QString SpeechControl::Windows::TrainingSettingsPane::title() const {
    return "Training";
}

QString SpeechControl::Windows::TrainingSettingsPane::id() const {
    return "trnng";
}

bool TrainingSettingsPane::containsText ( const QString& p_query ) const {

}

QPixmap TrainingSettingsPane::pixmap() const {
    return QIcon::fromTheme ( "configure" ).pixmap ( 32,32 );
}

void TrainingSettingsPane::resetPanel() {

}

void TrainingSettingsPane::restoreDefaults() {

}

void TrainingSettingsPane::updateUi() {

}


#include "training-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
