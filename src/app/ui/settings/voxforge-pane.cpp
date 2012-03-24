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

#include "voxforge-pane.hpp"
#include "ui_settingspane-voxforge.h"

using namespace SpeechControl::Windows;

VoxforgeSettingsPane::VoxforgeSettingsPane() :
    ui (new Ui::VoxforgeSettingsPane)
{
    qDebug() << "[VoxforgeSettingsPane::{constructor}] Building voxforge settings pane...";
    ui->setupUi (this);
    updateUi();
    qDebug() << "[VoxforgeSettingsPane::{constructor}] Built voxforge settings pane.";
}

VoxforgeSettingsPane::~VoxforgeSettingsPane()
{
    delete ui;
}

void VoxforgeSettingsPane::changeEvent (QEvent* e)
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

QString SpeechControl::Windows::VoxforgeSettingsPane::title() const
{
    return "VoxForge";
}

QString SpeechControl::Windows::VoxforgeSettingsPane::id() const
{
    return "vxfrg";
}

QPixmap VoxforgeSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void VoxforgeSettingsPane::restoreDefaults()
{

}

void VoxforgeSettingsPane::updateUi()
{

}

/// @todo Toggle the ability to upload content up to VoxForge.
void SpeechControl::Windows::VoxforgeSettingsPane::on_checkBox_clicked()
{

}

#include "voxforge-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
