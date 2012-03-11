/***
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QFile>
#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <QProcess>
#include <QErrorMessage>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QMenu>

#include <app/sessions/corpus.hpp>

#include "core.hpp"
#include "indicator.hpp"
#include "sessions/session.hpp"
#include "dictation/agent.hpp"
#include "desktopcontrol/agent.hpp"
#include "desktopcontrol/command.hpp"
#include "main-window.hpp"
#include "training-dialog.hpp"
#include "settings-dialog.hpp"
#include "content-manager.hpp"
#include "session-manager.hpp"
#include "quickstart-wizard.hpp"
#include "about-dialog.hpp"
#include "micsetup-wizard.hpp"
#include "contents-wizard.hpp"
#include "sessions-wizard.hpp"
#include "ui_main-window.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

using SpeechControl::DesktopControl::AbstractCommand;
using SpeechControl::DesktopControl::AbstractCategory;
using SpeechControl::DesktopControl::CommandList;

/// @todo Add icons to the QActions.
Main::Main() : m_ui ( new Ui::MainWindow ), m_prgStatusbar ( 0 ) {
    m_ui->setupUi ( this );
    m_ui->retranslateUi ( this );
    m_prgStatusbar = new QProgressBar ( this );

    // Do a bit of cleanup on the status bar.
    m_ui->statusBar->addPermanentWidget ( m_prgStatusbar );
    m_prgStatusbar->setMaximum ( 100 );
    setProgress ( 0.0 );

    // Update icons.
    m_ui->actionQuit->setIcon ( QIcon::fromTheme ( "application-exit" ) );
    m_ui->actionAboutSpeechControl->setIcon ( QIcon ( ":/logo/sc" ) );
    m_ui->actionReportBug->setIcon ( QIcon::fromTheme ( "tools-report-bug" ) );
    m_ui->actionOptions->setIcon ( QIcon::fromTheme ( "configure" ) );
    m_ui->actionWizardMicrophone->setIcon ( QIcon::fromTheme ( "audio-input-microphone" ) );
    m_ui->menuDictation->setIcon ( QIcon::fromTheme ( "audio-input-microphone" ) );
    m_ui->menuDesktopControl->setIcon ( QIcon::fromTheme ( "audio-headset" ) );
    m_ui->actionWizardContent->setIcon ( QIcon::fromTheme ( "x-office-document" ) );
    m_ui->actionWizardSessions->setIcon ( QIcon::fromTheme ( "application-x-tar" ) );
    m_ui->actionDesktopControlCommands->setIcon ( QIcon::fromTheme ( "view-list-text" ) );
    m_ui->actionStartTraining->setIcon ( QIcon::fromTheme ( "system-run" ) );

    // Update the actions and buttons.
    connect ( DesktopControl::Agent::instance(), SIGNAL ( stateChanged ( ActivityState ) ), this, SLOT ( desktopControlStateChanged() ) );
    connect ( Dictation::Agent::instance(), SIGNAL ( stateChanged ( ActivityState ) ), this, SLOT ( dictationStateChanged() ) );
    desktopControlStateChanged();
    dictationStateChanged();
    on_actionDesktopControlActive_triggered ( DesktopControl::Agent::instance()->isActive() );
    on_actionDictationActive_triggered ( Dictation::Agent::instance()->isActive() );

    // Greet the user :)
    setStatusMessage ( tr ( "Welcome to %1, speech recognition for Linux." ).arg ( QApplication::applicationName() ), 4000 );
}

void Main::show() {
    if ( Microphone::allMicrophones().empty() ) {
        QErrorMessage* l_msg = new QErrorMessage ( this );
        l_msg->setModal ( true );
        l_msg->setWindowTitle ( tr ( "No Microphones Found" ) );
        l_msg->showMessage ( tr ( "No microphones were found on your system. Please ensure that you have one installed and detectable by "
                                  "the audio system and make sure that <b>%1</b> is installed on your system." ).arg ( "gstreamer-plugins-good" ),
                             "NoMicrophonesFoundOnStart" );
    }

    updateContent();
    QMainWindow::show();
}

void Main::hide() {
    if ( Indicator::instance()->isVisible() ){
        QErrorMessage* l_msg = new QErrorMessage ( this );
        l_msg->setModal ( true );
        l_msg->setWindowTitle ( tr ( "Hidden From View" ) );
        l_msg->showMessage ( tr ( "SpeechControl has been hidden into the system's tray."
        "You can restore it by clicking the menu and selecting 'Restore'." ),"HiddenToTray" );
        QMainWindow::hide();
    } else {
        QApplication::quit();
    }
}

void Main::setStatusMessage ( const QString& p_message , const int p_timeout ) {
    m_ui->statusBar->showMessage ( p_message, p_timeout );
}

void Main::desktopControlStateChanged() {
    switch ( DesktopControl::Agent::instance()->state() ) {
    case AbstractAgent::ActivityState::Enabled:
        setStatusMessage ( tr ( "Desktop control enabled." ) );
        break;
    case AbstractAgent::ActivityState::Disabled:
        setStatusMessage ( tr ( "Desktop control disabled." ) );
        break;
    default:
        break;
    }

    m_ui->btnDctn->setEnabled ( DesktopControl::Agent::instance()->state() == AbstractAgent::ActivityState::Disabled );
    m_ui->btnDsktpCntrl->setEnabled ( Dictation::Agent::instance()->state() == AbstractAgent::ActivityState::Enabled );
}

void Main::dictationStateChanged() {
    switch ( Dictation::Agent::instance()->state() ) {
    case AbstractAgent::ActivityState::Enabled:
        setStatusMessage ( tr ( "Dictation enabled." ) );
        break;
    case AbstractAgent::ActivityState::Disabled:
        setStatusMessage ( tr ( "Dictation disabled." ) );
        break;
    default:
        break;
    }

    m_ui->btnDctn->setEnabled ( Dictation::Agent::instance()->state() == AbstractAgent::ActivityState::Enabled );
    m_ui->btnDsktpCntrl->setEnabled ( Dictation::Agent::instance()->state() == AbstractAgent::ActivityState::Disabled );
}

/// @todo Instead of this constant ticking, use signals to update this code.
void Main::updateContent() {
    m_ui->lblSessions->setText ( QString::number ( Session::allSessions().count() ) );
    m_ui->lblContent->setText ( QString::number ( Content::allContents().count() ) );
    m_ui->lblAccuracy->setText ( "100.0%" );
    m_ui->lblSpeechIndex->setText ( "10.0" );
}

void Main::setProgress ( const double p_progress ) {
    const int l_val = ( int ) ( p_progress * 100 );

    if ( l_val == 0 || l_val == 100 ) {
        m_prgStatusbar->hide();
    } else {
        m_prgStatusbar->show();
        m_prgStatusbar->setValue ( l_val );
    }
}

void Main::on_actionDesktopControlOptions_triggered() {
    Settings::switchToPanel ( "dsktpcntrl" );
}

void Main::on_actionOptions_triggered() {
    Settings::switchToPanel ( "gnrl" );
}

/// @todo Invoke the process of adapting a model. If anything, have a special window for such a process.
void Main::on_actionAdaptModels_triggered() {
}

void Main::on_actionStartTraining_triggered () {
    Session* l_session = SessionManager::pickSession();

    if ( l_session ) {
        TrainingDialog::startTraining ( l_session );
        setStatusMessage ( tr ( "Training session \"%1\"" ).arg ( l_session->content()->title() ) , 3000 );
    }
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDesktopControlActive_triggered ( bool p_checked ) {
    if ( p_checked && Dictation::Agent::instance()->isActive() )
        return;

    DesktopControl::Agent::instance()->setState ( p_checked ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled );
    setStatusMessage ( ( p_checked ? tr ( "Desktop control enabled." ) : tr ( "Desktop control disabled." ) ) , 3000 );
    m_ui->btnDsktpCntrl->setIcon ( ( ( p_checked == true ) ? QIcon::fromTheme ( "media-record" ) : QIcon::fromTheme ( "media-playback-pause" ) ) );
    m_ui->btnDsktpCntrl->setChecked ( p_checked );
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDictationActive_triggered ( const bool p_checked ) {
    if ( p_checked && DesktopControl::Agent::instance()->isActive() )
        return;

    Dictation::Agent::instance()->setState ( ( p_checked ) ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled );
    setStatusMessage ( ( ( p_checked ) ? tr ( "Dictation enabled." ) : tr ( "Dictation disabled." ) )  , 3000 );
    m_ui->btnDctn->setIcon ( ( ( p_checked ) ? QIcon::fromTheme ( "media-record" ) : QIcon::fromTheme ( "media-playback-pause" ) ) );
    m_ui->btnDctn->setChecked ( p_checked );
}

void Main::on_actionAboutQt_triggered() {
    QApplication::aboutQt();
}

void Main::on_actionAboutSpeechControl_triggered() {
    AboutDialog l_dlg ( this );
    l_dlg.exec();
}

void Main::on_actionPluginOptions_triggered() {
    Settings::switchToPanel ( "dsktpcntrl" );
}

void Main::on_actionDictationOptions_triggered() {
    Settings::switchToPanel ( "dctn" );
}

void Main::on_actionWizardMicrophone_triggered() {
    MicrophoneSetup* l_wiz = new MicrophoneSetup;
    l_wiz->exec();
}

void Main::on_actionWizardContent_triggered() {
    ContentWizard* l_wiz = new ContentWizard;
    l_wiz->exec();
}

void Main::on_actionWizardSessions_triggered() {
    SessionWizard* l_wiz = new SessionWizard;
    l_wiz->exec();
}

/// @todo Build the Voxforge Wizard.
void Main::on_actionWizardVoxForge_triggered() {

}

void Main::on_actionReportBug_triggered() {
    QProcess::startDetached ( "sensible-browser", ( QStringList() << "http://dev.thesii.org/reporting-bugs" ) );
}

Main::~Main() {
    delete m_ui;
}

#include "main-window.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

