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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QVariantMap>

#include "app/core.hpp"
#include "login.hpp"
#include "ui_voxforgewizard-login.h"

using namespace SpeechControl::Windows::Pages;

LoginPortal::LoginPortal (QWidget* parent) : QWizardPage (parent),
    ui (new Ui::LoginPortal), m_successLogin (false)
{
    ui->setupUi (this);
    this->setWindowIcon (QIcon::fromTheme ("encrypted"));
    this->setLayout (ui->gridLayout);

    QVariantMap map = Core::configuration ("Voxforge/Authentication").toMap();
    const bool withAuth = Core::configuration ("Voxforge/ProvideAuthentication").toBool();

    if (withAuth) {
        ui->lineEditUsername->setText (map.value ("Username").toString());
        ui->lineEditPassword->setText (map.value ("Password").toString());
    }

    ui->progressBarLogin->setVisible (false);
}

void LoginPortal::on_checkBoxRemeberCreds_toggled (const bool& p_checked)
{
    if (p_checked) {
        QVariantMap map = Core::configuration ("Voxforge/Authentication").toMap();
        map["Username"] = ui->lineEditUsername->text();
        map["Password"] = ui->lineEditPassword->text();
        Core::setConfiguration ("Voxforge/Authentication", map);
    }
}

void LoginPortal::on_lineEditUsername_textChanged (const QString& p_text)
{
    emit completeChanged();
}

void LoginPortal::on_lineEditPassword_textChanged (const QString& p_text)
{
    emit completeChanged();
}

bool LoginPortal::isComplete() const
{
    return !ui->lineEditPassword->text().isEmpty() && !ui->lineEditUsername->text().isEmpty() && m_successLogin;
}

void LoginPortal::on_btnLogin_clicked()
{
    ui->progressBarLogin->setVisible (true);
    emit completeChanged();
}

LoginPortal::~LoginPortal()
{

}

#include "ui/wizards/voxforge/login.moc"
// kate: indent-mode cstyle; replace-tabs on; 
