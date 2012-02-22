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
#include <QMessageBox>
#include <QPushButton>

#include <sphinx.hpp>
#include <corpus.hpp>

#include "ui_main-window.h"
#include "core.hpp"
#include "session.hpp"
#include "main-window.hpp"
#include "about-dialog.hpp"
#include "training-dialog.hpp"
#include "settings-dialog.hpp"
#include "books-manager.hpp"
#include "session-manager.hpp"
#include "quickstart-wizard.hpp"

#define BOLD_BUTTON "background-color: red; color: white; font-weight: bold;"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

/// @todo Add icons to the QActions.
Main::Main() : m_ui ( new Ui::MainWindow ), m_prgStatusbar ( 0 )
{
    m_ui->setupUi ( this );
    m_ui->retranslateUi ( this );
    m_prgStatusbar = new QProgressBar ( this );

    this->restoreGeometry ( Core::instance()->configuration ( "MainWindow/Geometry" ).toByteArray() );
    this->restoreState ( Core::instance()->configuration ( "MainWindow/State" ).toByteArray() );

    m_ui->statusBar->showMessage ( "Ready." );
    m_ui->statusBar->addPermanentWidget ( m_prgStatusbar );

    m_prgStatusbar->setValue ( 3 );
    m_prgStatusbar->setMaximum ( 10 );
    updateContent();
}

/// @todo Instead of this constant ticking, use signals to update this code.
void Main::updateContent()
{
    m_ui->lblSessions->setText ( QString::number ( Session::allSessions().count() ) );
    m_ui->lblBooks->setText ( QString::number ( Content::allContents().count() ) );
}

void Main::on_actionOptionsDesktopControl_triggered()
{
    Settings::switchToPanel ( "dsktp-cntrl" );
}


void Main::on_actionOptions_triggered()
{
    Settings::switchToPanel ( "gnrl" );
}

/// @todo Invoke the process of adapting a model. If anything, have a special window for such a process.
void Main::on_actionAdaptModels_triggered()
{
    m_ui->statusBar->showMessage ( "This feature hasn't been implemented yet." );
}

void Main::on_actionStartTraining_triggered ()
{
    Session* l_session = SessionManager::pickSession();
    if ( l_session )
    {
        Training::startTraining ( l_session );
        m_ui->statusBar->showMessage ( "Training session \"" + l_session->content()->title() + "\"..." );
    }
}

void Main::on_btnDsktpCntrl_checked ( bool checked )
{
    QPushButton* l_btn = m_ui->btnDsktpCntrl;
    if ( checked )
        l_btn->setStyleSheet ( BOLD_BUTTON );
    else
        l_btn->setStyleSheet ( QString::null );
}

void Main::on_btnDctn_checked ( bool checked )
{
    QPushButton* l_btn = m_ui->btnDctn;
    if ( checked )
        l_btn->setStyleSheet ( BOLD_BUTTON );
    else
        l_btn->setStyleSheet ( QString::null );
}

/// @todo Allow configuration option to show specific notifcations to prevent noise.
void Main::desktopControlToggled ( const bool p_val )
{
    QPushButton* l_btn = m_ui->btnDsktpCntrl;
    l_btn->setChecked ( p_val );

    m_ui->statusBar->showMessage ( ( ( p_val == true ) ? "Activating desktop control..." : "Deactivating desktop control..." ) ,5 );
}

/// @todo Allow configuration option to show specific notifcations to prevent noise.
void Main::dictationToggled ( const bool p_val )
{
    QPushButton* l_btn = m_ui->btnDctn;
    l_btn->setChecked ( p_val );

    m_ui->statusBar->showMessage ( ( ( p_val == true ) ? "Activating dictation..." : "Deactivating dictation..." ) ,5 );
}

void Main::on_actionStartDesktopControl_triggered()
{
    /*if (DesktopControl::Agent::isActive())
        DesktopControl::Agent::stop();
    else
        DesktopControl::Agent::start();
    */
}

void Main::on_actionStartDictation_triggered()
{
    /*if (Dictation::Agent::isActive())
        Dictation::Agent::stop();
    else
        Dictation::Agent::start();*/
}

void Main::on_actionAboutQt_triggered()
{
    SC_APP::aboutQt();
}

void Main::on_actionAboutSpeechControl_triggered()
{
    AboutDialog l_dlg ( this );
    l_dlg.exec();
}

void Main::setProgress ( const double p_prgs )
{
    const int l_val = ( int ) ( p_prgs * 100 );
    m_prgStatusbar->setValue ( l_val );
}

Main::~Main()
{
    delete m_ui;
}

#ifdef HAVE_KDE
#include "main-window.moc"
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
