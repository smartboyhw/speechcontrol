/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/22/12 23:02:17 PM
 */

#include <QDebug>

#include <core.hpp>
#include <indicator.hpp>

#include "general-pane.hpp"
#include "sessions/session.hpp"
#include "ui_settingspane-general.h"

SPCHCNTRL_USE_NAMESPACE
SPCHCNTRL_UI_USE_NAMESPACE

GeneralSettingsPane::GeneralSettingsPane () :
    ui (new Ui::GeneralSettingsPane)
{
    qDebug() << "[GeneralSettingsPane::{constructor}] Building general settings pane...";
    ui->setupUi (this);
    updateUi();
    qDebug() << "[GeneralSettingsPane::{constructor}] Built general settings pane.";
}

GeneralSettingsPane::~GeneralSettingsPane()
{
    delete ui;
}

void GeneralSettingsPane::changeEvent (QEvent* e)
{
    QWidget::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString GeneralSettingsPane::title() const
{
    return "General";
}

QString GeneralSettingsPane::id() const
{
    return "gnrl";
}

QPixmap GeneralSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void GeneralSettingsPane::restoreDefaults()
{
    Core::setAutoStart (false);
}

void GeneralSettingsPane::updateUi()
{
    ui->lblSessionCount->setText (QString::number (Session::allSessions().count()));
    ui->lblAccuracyRating->setText ("<i>n/a</i>");
    ui->checkBoxAutoStartApp->setChecked (Core::doesAutoStart());
    ui->radioButtonIconBlack->setChecked (Core::configuration ("Indicator/Icon").toString() == "Black");
    ui->radioButtonIconWhite->setChecked (Core::configuration ("Indicator/Icon").toString() == "White");
    ui->radioButtonIconDefault->setChecked (Core::configuration ("Indicator/Icon").toString() == "Default");
}

void GeneralSettingsPane::on_checkBoxAutoStartApp_toggled (bool p_checked)
{
    Core::setAutoStart (p_checked);
}

void GeneralSettingsPane::on_radioButtonIconBlack_toggled (bool p_checked)
{
    if (p_checked) {
        Core::setConfiguration ("Indicator/Icon", "Black");
        ui->lblSampleIcon->setPixmap (Indicator::icon().pixmap (48, 48));
    }
}

void GeneralSettingsPane::on_radioButtonIconDefault_toggled (bool p_checked)
{
    if (p_checked) {
        Core::setConfiguration ("Indicator/Icon", "Default");
        ui->lblSampleIcon->setPixmap (Indicator::icon().pixmap (48, 48));
    }
}

void GeneralSettingsPane::on_radioButtonIconWhite_toggled (bool p_checked)
{
    if (p_checked) {
        Core::setConfiguration ("Indicator/Icon", "White");
        ui->lblSampleIcon->setPixmap (Indicator::icon().pixmap (48, 48));
    }
}

#include "ui/settings/general-pane.moc"
// kate: indent-mode cstyle; replace-tabs on;
