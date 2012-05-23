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
 * @date 05/20/12 13:57:53 PM
 */

#include <QIcon>

#include "app/core.hpp"
#include "app/ui/wizards/intro.hpp"
#include "app/ui/wizards/outro.hpp"
#include "app/ui/micsetup-wizard.hpp"
#include "app/ui/wizards/micsetup/micselect.hpp"
#include "app/ui/wizards/micsetup/micsample.hpp"

SPCHCNTRL_UI_USE_NAMESPACE

MicrophoneSetup::MicrophoneSetup (QWidget* parent) :
    AbstractWizard (parent)
{
    // http://wiki.thesii.org/SpeechControl/UserGuides/Microphones
    QIcon l_icon = QIcon::fromTheme ("audio-headset");
    setPixmap (QWizard::LogoPixmap, l_icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setWindowTitle (tr ("Microphone Setup Wizard - SpeechControl"));
    setPage (MicrophoneSetup::IntroductionPage,
             new Pages::IntroductionPage (tr ("This wizard will help you configure your microphone for optimal performance in SpeechControl."
                                          " If this is the first time you've used a microphone or an alternative audio input device, please "
                                          "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/Microphones\">http://wiki.thesii.org/SpeechControl/UserGuides/Microphones</a> "
                                          "for more detialed information about correct and recommended use of the microphone whilst using SpeechControl."
                                             )));
    setPage (MicrophoneSetup::SelectionPage, new Pages::MicrophoneSelection);
    setPage (MicrophoneSetup::SamplingPage, new Pages::MicrophoneSample);
    setPage (MicrophoneSetup::ConclusionPage,
             new Pages::ConclusionPage (tr ("You've configured your microphone to work optimally with SpeechControl. Click <b>Finish</b> to finialize your results and commit them to SpeechControl."
                                            "Remember to use this wizard to tweak and adapt your microphone in SpeechControl so that it works optimally. "
                                            "If your accuracy is poor, consider training your acoustic models by using the Adaption Wizard."
                                            "Visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/Microphones\">http://wiki.thesii.org/SpeechControl/UserGuides/Microphones</a> "
                                            "for more detialed information about correct and recommended use of the microphone whilst using SpeechControl."
                                           )));
}

void MicrophoneSetup::accept()
{
    Core::instance()->setConfiguration ("Microphone/Default", property ("mic-id"));
    this->QDialog::accept();
}

MicrophoneSetup::~MicrophoneSetup()
{

}

#include "ui/micsetup-wizard.moc"
// kate: indent-mode cstyle; replace-tabs on;
