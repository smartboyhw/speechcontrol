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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 19:51:41 PM
 */

#include "services-pane.hpp"
#include "voxforge-pane.hpp"
#include "ui_settingspane-services.h"

SPCHCNTRL_USE_NAMESPACE
SPCHCNTRL_UI_USE_NAMESPACE

ServicesSettingsPane::ServicesSettingsPane() :
    ui (new Ui::ServicesSettingsPane)
{
    qDebug() << "[ServicesSettingsPane::{constructor}] Building services settings pane...";
    ui->setupUi (this);
    addPane (new VoxforgeSettingsPane);
    updateUi();
    qDebug() << "[ServicesSettingsPane::{constructor}] Built services settings pane.";
}

ServicesSettingsPane::~ServicesSettingsPane()
{
    delete ui;
}

void ServicesSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString SpeechControl::Windows::ServicesSettingsPane::title() const
{
    return "Services";
}

QString SpeechControl::Windows::ServicesSettingsPane::id() const
{
    return "srvcs";
}

QPixmap ServicesSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("services").pixmap (32, 32);
}

void ServicesSettingsPane::restoreDefaults()
{

}

void ServicesSettingsPane::updateUi()
{

}

#include "ui/settings/services-pane.moc"
// kate: indent-mode cstyle; replace-tabs on;
