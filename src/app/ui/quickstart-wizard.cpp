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


#include "core.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "quickstart-wizard.hpp"
#include "wizards/quickstart/userinfo.hpp"
#include "wizards/micsetup/micselect.hpp"

#include <QIcon>
#include <QVariantMap>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

QuickStart::QuickStart (QWidget* parent) :
    WizardBase (parent)
{
    this->setWindowTitle (tr ("Quick Start :: SpeechControl"));
    QIcon l_icon = QIcon::fromTheme ("preferences-desktop-personal");
    setWindowTitle (tr ("Quick Start - SpeechControl"));
    setPixmap (QWizard::LogoPixmap, l_icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setPage (QuickStart::IntroductionPage,
             (new Wizards::Pages::IntroductionPage (tr ("This wizard allows you to tweak SpeechControl to your personal configuration."))));
    setPage (QuickStart::UserCreationPage,
             (new Wizards::Pages::UserInitialization));
    setPage (QuickStart::MicrophoneCreationPage,
             (new Wizards::Pages::MicrophoneSelection));
    setPage (QuickStart::ConclusionPage,
             (new Wizards::Pages::ConclusionPage(tr("You've successfully set up SpeechControl to settings of your preference."))));
}

/// @todo The user's country could be automatically detected by QLocale.
void QuickStart::accept()
{
    Core* core = Core::instance();
    QVariantMap language;
    QString gender;

    language["Spoken"] = field ("language-spoken");
    language["Native"] = field ("language-native");

    if (field ("is-gender-male").toBool()) {
        gender = "Male";
    }
    else {
        gender = "Female";
    }

    core->setConfiguration ("User/Gender", gender);
    core->setConfiguration ("User/Language", language);
    core->setConfiguration ("User/Age", property ("age"));
    core->setConfiguration ("User/Country", property ("country"));
    core->setConfiguration ("Microphone/Default", property ("mic-id"));

    this->QDialog::accept();
}

QuickStart::~QuickStart()
{
}

#include "quickstart-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
