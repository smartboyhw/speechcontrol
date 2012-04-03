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

#include "app/core.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/voxforge/optin.hpp"
#include "wizards/voxforge/package.hpp"
#include "wizards/voxforge/login.hpp"
#include "wizards/voxforge/upload.hpp"
#include "voxforge-wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

VoxforgeWizard::VoxforgeWizard (QWidget* parent) :
    WizardBase (parent)
{
    // http://wiki.thesii.org/SpeechControl/UserGuides/Voxforge
    QIcon icon = QIcon::fromTheme ("internet-web-browser");
    setPixmap (QWizard::LogoPixmap, icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setWindowTitle (tr ("Voxforge Upload Wizard - SpeechControl"));
    setPage (VoxforgeWizard::IntroductionPage,
             new Pages::IntroductionPage (tr ("This wizard will help you upload your speech corpora."
                                          "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/Voxforge\">http://wiki.thesii.org/SpeechControl/UserGuides/Voxforge</a> "
                                          "for more detailed information about correct and recommended use of the microphone whilst using SpeechControl."
                                             )));
    setPage (VoxforgeWizard::OptInConfirmationPage, new Pages::OptinConfirmation);
    setPage (VoxforgeWizard::PackageSelectionPage, new Pages::PackageSelection);
    setPage (VoxforgeWizard::LoginPortalPage, new Pages::LoginPortal);
    setPage (VoxforgeWizard::UploadEnactionPage, new Pages::UploadEnaction);
    setPage (VoxforgeWizard::ConclusionPage,
             new Pages::ConclusionPage (tr ("You've successfully uploaded speech corpora to Voxforge."
                                            "Visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/Voxforge\">http://wiki.thesii.org/SpeechControl/UserGuides/Voxforge</a> "
                                            "for more detailed information about correct and recommended use of the microphone whilst using SpeechControl."
                                           )));
}

VoxforgeWizard::~VoxforgeWizard()
{

}

#include "voxforge-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
