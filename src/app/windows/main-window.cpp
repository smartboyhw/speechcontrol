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
#include <QErrorMessage>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>

#include <sphinx.hpp>
#include <corpus.hpp>

#include "core.hpp"
#include "indicator.hpp"
#include "sessions/session.hpp"
#include "dictation/agent.hpp"
#include "desktopcontrol/agent.hpp"
#include "desktopcontrol/command.hpp"
#include "main-window.hpp"
#include "training-dialog.hpp"
#include "settings-dialog.hpp"
#include "books-manager.hpp"
#include "session-manager.hpp"
#include "quickstart-wizard.hpp"

#include "ui_main-window.h"

#ifndef HAVE_KDE
#include "about-dialog.hpp"
#else
#include <KMenu>
#endif

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

/// @todo Add icons to the QActions.
Main::Main() : m_ui ( new Ui::MainWindow ), m_prgStatusbar ( 0 ) {
    m_ui->setupUi ( this );
    m_ui->retranslateUi ( this );
    this->setWindowIcon ( QIcon ( ":/logo/sc" ) );
    m_prgStatusbar = new QProgressBar ( this );

    // Do a bit of cleanup on the status bar.
    m_ui->statusBar->addPermanentWidget ( m_prgStatusbar );
    m_prgStatusbar->setMaximum ( 100 );
    setProgress ( 0.0 );
    setStatusMessage ( "Welcome to SpeechControl, speech recognition for Linux.",4000 );

    // Update icons.
    m_ui->actionQuit->setIcon ( QIcon::fromTheme ( "application-exit" ) );
    m_ui->actionAboutSpeechControl->setIcon ( QIcon ( ":/logo/sc" ) );
    m_ui->actionReportBug->setIcon ( QIcon::fromTheme ( "tools-report-bug" ) );
    m_ui->actionOptions->setIcon ( QIcon::fromTheme ( "configure" ) );
    m_ui->actionWizardMicrophone->setIcon ( QIcon::fromTheme ( "audio-input-microphone" ) );
    m_ui->actionWizardBooks->setIcon ( QIcon::fromTheme ( "x-office-document" ) );
    m_ui->actionWizardSessions->setIcon ( QIcon::fromTheme ( "application-x-tar" ) );
    m_ui->actionDesktopControlActive->setIcon ( QIcon::fromTheme ( "audio-headset" ) );
    m_ui->actionDesktopControlCommands->setIcon ( QIcon::fromTheme ( "view-list-text" ) );
    m_ui->actionStartTraining->setIcon ( QIcon::fromTheme ( "system-run" ) );

    // Link up the buttons to their corresponding actions.
    m_ui->btnDsktpCntrl->setChecked ( m_ui->actionDesktopControlActive->isChecked() );
    m_ui->btnDctn->setChecked ( m_ui->actionDictationActive->isChecked() );

    connect ( DesktopControl::Agent::instance(),SLOT ( onStateChanged() ),this,SLOT ( desktopControlStateChanged() ) );
    connect ( Dictation::Agent::instance(),SLOT ( onStateChanged() ),this,SLOT ( dictationStateChanged() ) );
#ifdef HAVE_KDE
    // Update the Help menu.
    m_ui->menuHelp->clear();
    m_ui->menuHelp->addActions ( SC_MW::helpMenu()->actions() );
#endif

    Indicator::show();
}

void Main::show() {
    if ( Microphone::allMicrophones().empty() ) {
        QErrorMessage* l_msg = new QErrorMessage ( this );
        l_msg->setModal ( true );
        l_msg->setWindowTitle ( "No Microphones Found" );
        l_msg->showMessage ( tr ( "No microphones were found on your system. Please ensure that you have one installed and detectable by " ) +
                             tr ( "the audio system and make sure that <b>gstreamer-plugins-good</b> is installed on your system." ),
                             "NoMicrophonesFoundOnStart" );
    }

    updateContent();
    SC_MW::show();
}

void Main::close() {
    SC_MW::close();
}

void Main::on_tabWidget_currentChanged ( const int p_index ) {
    switch ( p_index ) {
    case 0: { // main info
        updateContent();
    }
    break;

    case 1: { // desktop control commands
        using SpeechControl::DesktopControl::AbstractCommand;
        using SpeechControl::DesktopControl::AbstractCategory;
        using SpeechControl::DesktopControl::CommandList;

        AbstractCategory* l_glbl = DesktopControl::AbstractCategory::global();
        CommandList l_cmds = l_glbl->commands();
        QTableWidget* l_widg = m_ui->tableWidget;

        l_widg->clear();

        l_widg->setHorizontalHeaderLabels ( QStringList() << "Statement"
                                            << "Command"
                                          );

        Q_FOREACH ( AbstractCommand* l_cmd, l_cmds ) {
            l_widg->setRowCount ( l_cmd->statements().count() + l_widg->rowCount() );
            Q_FOREACH ( const QString l_statement, l_cmd->statements() ) {
                const int l_row = l_widg->rowCount();
                l_widg->setItem ( l_row,0, ( new QTableWidgetItem ( l_statement ) ) );
                l_widg->setItem ( l_row,1, ( new QTableWidgetItem ( l_cmd->id() ) ) );
                qDebug() << l_statement << l_cmd->id();
            }
        }
    }
    break;

    case 2:
    { } // voxforge info
    break;
    }
}

void Main::setStatusMessage ( const QString& p_message , const int p_timeout ) {
    m_ui->statusBar->showMessage ( p_message, p_timeout );
}

void Main::desktopControlStateChanged() {
    switch ( DesktopControl::Agent::instance()->state() ) {
    case AbstractAgent::OperationState::Enabled:
        setStatusMessage ( tr ( "Desktop control enabled." ) );
        break;
    case AbstractAgent::OperationState::Disabled:
        setStatusMessage ( tr ( "Desktop control disabled." ) );
        break;
    default:
        break;
    }
}

void Main::dictationStateChanged() {
    switch ( Dictation::Agent::instance()->state() ) {
    case AbstractAgent::OperationState::Enabled:
        setStatusMessage ( tr ( "Dictation enabled." ) );
        break;
    case AbstractAgent::OperationState::Disabled:
        setStatusMessage ( tr ( "Dictation disabled." ) );
        break;
    default:
        break;
    }
}

/// @todo Instead of this constant ticking, use signals to update this code.
void Main::updateContent() {
    m_ui->lblSessions->setText ( QString::number ( Session::allSessions().count() ) );
    m_ui->lblBooks->setText ( QString::number ( Content::allContents().count() ) );
    m_ui->lblAccuracy->setText ( "100.0%" );
    m_ui->lblSpeechIndex->setText ( "10.0" );
}

void Main::setProgress ( const double p_progress ) {
    const int l_val = ( int ) ( p_progress * 100 );
    if ( l_val == 0 || l_val == 100 )
        m_prgStatusbar->hide();
    else {
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
    setStatusMessage ( "<b>This feature hasn't been implemented yet.</b>" );
}

void Main::on_actionStartTraining_triggered () {
    Session* l_session = SessionManager::pickSession();
    if ( l_session ) {
        TrainingDialog::startTraining ( l_session );
        setStatusMessage ( "Training session \"" + l_session->content()->title() + "\"..." , 3000 );
    }
}

/// @todo Ensure that the style of the button updates with the status.
void Main::on_btnDsktpCntrl_clicked ( bool p_checked ) {
    on_actionDesktopControlActive_triggered ( p_checked );
}

/// @todo Ensure that the style of the button updates with the status.
void Main::on_btnDctn_clicked ( bool p_checked ) {
    on_actionDictationActive_triggered ( p_checked );
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDesktopControlActive_triggered ( const bool p_checked ) {
    if ( p_checked )
        DesktopControl::Agent::instance()->setState ( SpeechControl::AbstractAgent::Enabled );
    else
        DesktopControl::Agent::instance()->setState ( SpeechControl::AbstractAgent::Disabled );
    setStatusMessage ( ( ( p_checked == true ) ? "Enabling desktop control..." : "Disabling desktop control..." ) ,5 );
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDictationActive_triggered ( const bool p_checked ) {
    if ( p_checked )
        Dictation::Agent::instance()->setState ( SpeechControl::AbstractAgent::Enabled );
    else
        Dictation::Agent::instance()->setState ( SpeechControl::AbstractAgent::Disabled );
    setStatusMessage ( ( ( p_checked == true ) ? "Enabling dictation..." : "Disabling dictation..." ) ,5 );
}

void Main::on_actionAboutQt_triggered() {
#ifndef HAVE_KDE
    QApplication::aboutQt();
#endif
}

void Main::on_actionAboutSpeechControl_triggered() {
#ifndef HAVE_KDE
    AboutDialog l_dlg ( this );
    l_dlg.exec();
#endif
}

Main::~Main() {
    delete m_ui;
}

#include "main-window.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 

