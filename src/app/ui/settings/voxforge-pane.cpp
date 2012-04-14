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

#include <QVariant>
#include <QVariantMap>

#include "app/core.hpp"
#include "voxforge-pane.hpp"
#include "ui_settingspane-voxforge.h"

using namespace SpeechControl;
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
    Core::setConfiguration("Voxforge/Enabled",false);
    Core::setConfiguration("Voxforge/ProvideAuthentication",false);
    Core::setConfiguration("Voxforge/Authentication",QVariantMap());
}

void VoxforgeSettingsPane::updateUi()
{
    const bool isEnabled = Core::configuration ("Voxforge/Enabled").toBool();
    const bool withAuth  = Core::configuration ("Voxforge/ProvideAuthentication").toBool();

    ui->lineEditUserName->setText (Core::configuration ("Voxforge/Authentication").toMap().value ("Username").toString());

    if (withAuth)
        ui->lineEditPassWord->setText (Core::configuration ("Voxforge/Authentication").toMap().value ("Password").toString());

    ui->checkBoxEnabled->setChecked (isEnabled);
    ui->groupBoxAuthentication->setChecked (withAuth);
}

/// @todo Test the validity of the passed username and password.
void VoxforgeSettingsPane::on_btnTestLogin_clicked()
{
    const QString username = ui->lineEditUserName->text();
    const QString password = ui->lineEditPassWord->text();
}

void SpeechControl::Windows::VoxforgeSettingsPane::on_checkBoxEnabled_toggled (const bool& p_checked)
{
    Core::setConfiguration ("Voxforge/Enabled", p_checked);
}

void VoxforgeSettingsPane::on_groupBoxAuthenticated_toggled (const bool& p_checked)
{
    Core::setConfiguration ("Voxforge/ProvideAuthentication", p_checked);
}

void VoxforgeSettingsPane::on_lineEditPassWord_textChanged (const QString& p_text)
{
    QVariantMap map = Core::configuration ("Voxforge/Authentication").toMap();
    map["Password"] = p_text;
    Core::setConfiguration ("Voxforge/Authentication", map);
}

void VoxforgeSettingsPane::on_lineEditUserName_textChanged (const QString& p_text)
{
    QVariantMap map = Core::configuration ("Voxforge/Authentication").toMap();
    map["Username"] = p_text;
    Core::setConfiguration ("Voxforge/Authentication", map);
}

#include "ui/settings/voxforge-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
