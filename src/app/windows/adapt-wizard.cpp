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

#include "adapt-wizard.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/adapt/modelselection.hpp"

#include <QIcon>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

AdaptWizard::AdaptWizard (QWidget* parent) :
    WizardBase (parent)
{
    // http://wiki.thesii.org/SpeechControl/UserGuides/AdaptAcousticModels
    QIcon icon = QIcon::fromTheme ("configure");
    setPixmap (QWizard::LogoPixmap, icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setWindowTitle (tr ("Adaption Wizard - SpeechControl"));
    setPage (AdaptWizard::IntroductionPage,
             new Pages::IntroductionPage (tr ("This wizard will help you configure your microphone for optimal performance in SpeechControl."
                                              " If this is the first time you've used a microphone or an alternative audio input device, please "
                                              "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels\">http://wiki.thesii.org/SpeechControl/UserGuides/Microphones</a> "
                                              "for more detailed information about correct and recommended use of the microphone whilst using SpeechControl."
            )));
    setPage (AdaptWizard::SelectionPage, new Pages::ModelSelection);
    setPage (AdaptWizard::ConclusionPage,
             new Pages::ConclusionPage (tr ("You've configured your microphone to work optimally with SpeechControl. Click <b>Finish</b> to finalize your results and commit them to SpeechControl."
             " If this is the first time you've used a microphone or an alternative audio input device, please "
             "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels\">http://wiki.thesii.org/SpeechControl/UserGuides/Microphones</a> "
             "for more detailed information about correct and recommended use of the microphone whilst using SpeechControl."
             )));
}

void AdaptWizard::accept()
{
    this->QDialog::accept();
}

AdaptWizard::~AdaptWizard()
{

}

#include "adapt-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
