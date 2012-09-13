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
 * @date 05/26/12 10:57:10 AM
 */

#include "userinfo.hpp"
#include "ui_quickstart-userinfo.h"

#include <QDebug>
#include <QLocale>
#include <QVariantMap>

using SpeechControl::Windows::Pages::UserInitialization;

UserInitialization::UserInitialization (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::UserInitialization)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    registerField ("age", ui->comboBoxAge);
    registerField ("country", ui->comboBoxCountry);
    registerField ("language-spoken", ui->comboBoxLanguageSpoken);
    registerField ("language-native", ui->comboBoxLanguageNative);
    registerField ("is-gender-male", ui->radioButtonMale);
    registerField ("is-gender-female", ui->radioButtonFemale);
}

UserInitialization::~UserInitialization()
{
    delete ui;
}

bool SpeechControl::Windows::Pages::UserInitialization::validatePage()
{
    wizard()->setProperty ("age", ui->comboBoxAge->currentText());
    wizard()->setProperty ("country", ui->comboBoxCountry->currentText());
    return true;
}

/// @todo Load all of the countries.
void SpeechControl::Windows::Pages::UserInitialization::initializePage()
{
    ui->comboBoxCountry->clear();
    QLocale l_sys = QLocale::system();
    QList<QLocale::Country> l_countries = QLocale::countriesForLanguage (l_sys.language());
    Q_FOREACH (const QLocale::Country & l_country, l_countries) {
        ui->comboBoxCountry->addItem (QLocale::countryToString (l_country));
    }

    updateLanguages();
}

void SpeechControl::Windows::Pages::UserInitialization::updateLanguages()
{
    QLocale l_locale;
}

void SpeechControl::Windows::Pages::UserInitialization::clearPage()
{
    ui->comboBoxAge->clear();
    ui->comboBoxCountry->clear();
    ui->comboBoxLanguageNative->clear();
    ui->comboBoxLanguageSpoken->clear();
}

#include "ui/wizards/quickstart/userinfo.moc"
// kate: indent-mode cstyle; replace-tabs on;
