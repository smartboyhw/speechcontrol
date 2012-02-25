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
#include "micsetup-wizard.hpp"
#include "wizards/micsetup/micselect.hpp"

#include <QIcon>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

MicrophoneSetup::MicrophoneSetup ( QWidget *parent ) :
    WizardBase ( parent ) {
    QIcon l_icon = QIcon::fromTheme ( "audio-headset" );
    setPixmap ( QWizard::LogoPixmap,l_icon.pixmap ( 32,32,QIcon::Active,QIcon::On ) );
    setWindowTitle ( tr ( "Microphone Setup Wizard - SpeechControl" ) );
    setPage ( MicrophoneSetup::IntroductionPage,
              new Pages::IntroductionPage ( tr ( "This wizard will help you configure your microphone " ) +
                                            tr ( "for optimal performance in SpeechControl." ) ) );
    setPage ( MicrophoneSetup::SelectionPage, new Pages::MicrophoneSelection );
    setPage ( MicrophoneSetup::ConclusionPage,
              new Pages::ConclusionPage ( tr ( "You've configured your microphone to work optimally with SpeechControl. "
                                          "Click <b>Finish</b> to finialize your results and commit them to SpeechControl." ) ) );
}

void MicrophoneSetup::accept() {
    Core::instance()->setConfiguration ( "Microphone/Default",property ( "mic-id" ) );
    this->QDialog::accept();
}

MicrophoneSetup::~MicrophoneSetup() {

}

#include "micsetup-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
