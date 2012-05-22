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

#include <QIcon>
#include <QVariantMap>

#include "core.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/quickstart/userinfo.hpp"
#include "wizards/micsetup/micselect.hpp"
#include "wizards/micsetup/micsample.hpp"
#include "wizards/contents/selection.hpp"

#include "quickstart-wizard.hpp"

SPCHCNTRL_USE_NAMESPACE
SPCHCNTRL_UI_USE_NAMESPACE

QuickStart::QuickStart (QWidget* parent) :
    AbstractWizard (parent)
{
    this->setWindowTitle (tr ("Quick Start :: SpeechControl"));
    setWindowTitle (tr ("Quick Start - SpeechControl"));
    setPixmap (QWizard::LogoPixmap, QIcon::fromTheme ("preferences-desktop-personal").pixmap (32, 32, QIcon::Active, QIcon::On));
    setPage(QuickStart::IntroductionPage,
            new Windows::Pages::IntroductionPage(tr("This wizard allows you to tweak SpeechControl to your personal configuration."))
           );
    /*setPage (QuickStart::UserCreationPage,
             (new Pages::UserInitialization));
    setPage (QuickStart::MicrophoneCreationPage,
             (new Pages::MicrophoneSelection));
    setPage (QuickStart::AccuracySamplingPage,
             (new Pages::MicrophoneSample));
    setPage (QuickStart::ContentAdditionPage,
             (new Pages::SourceSelectionPage));*/
    setPage (QuickStart::ConclusionPage,
             (new Windows::Pages::ConclusionPage (tr ("You've successfully set up SpeechControl to settings of your preference."))));
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

#include "ui/quickstart-wizard.moc"
// kate: indent-mode cstyle; replace-tabs on;
