/**
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

#include "ui_main.h"
#include "core.hpp"
#include "main.hpp"
#include "training.hpp"
#include "settings.hpp"
#include "panelicon.hpp"
#include "desktopcontrol/agent.hpp"
#include "dictation/agent.hpp"
#include "managers/books.hpp"
#include "managers/session.hpp"
#include "sessions/session.hpp"
#include "wizards/quickstart/wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

/// @todo Add icons to the QActions.
Main::Main() : m_ui(new Ui::MainWindow), m_prgTraining(0) {
   m_ui->setupUi(this);
   m_ui->retranslateUi(this);
   m_prgTraining = new QProgressBar(this);
   PanelIcon::instance()->setIcon(this->windowIcon());

   this->restoreGeometry(Core::instance()->getConfig("MainWindow/Geometry").toByteArray());
   this->restoreState(Core::instance()->getConfig("MainWindow/State").toByteArray());

   m_ui->statusBar->showMessage("Ready.");
   m_ui->statusBar->addPermanentWidget(m_prgTraining);

   connect(DesktopControl::Agent::instance(),SIGNAL(stateChanged(bool)),this,SLOT(desktopControlToggled(bool)));
   connect(Dictation::Agent::instance(),SIGNAL(stateChanged(bool)),this,SLOT(dictationToggled(bool)));

   m_ui->btnDsktpCntrl->setChecked(DesktopControl::Agent::isActive());
   m_ui->btnDctn->setChecked(Dictation::Agent::isActive());
   on_btnDsktpCntrl_toggled(DesktopControl::Agent::isActive());
   on_btnDctn_toggled(DesktopControl::Agent::isActive());
   m_prgTraining->setValue(3);
   m_prgTraining->setMaximum(10);
   updateContent();
}

Main::~Main() {
    delete m_ui;
}

/// @todo Instead of this constant ticking, use signals to update this code.
void Main::updateContent() {
    m_ui->lcdSession->display(Session::allSessions().count());
    m_ui->lcdBooks->display(Content::allContents().count());
}

void Main::on_actionOptionsDesktopControl_triggered()
{
    Settings::switchToPanel("dsktp-cntrl");
}


void Main::on_actionOptions_triggered()
{
    Settings::switchToPanel("gnrl");
}

/// @todo Invoke the process of adapting a model. If anything, have a special window for such a process.
void Main::on_actionAdaptModels_triggered()
{
    m_ui->statusBar->showMessage("This feature hasn't been implemented yet.");
}

void Main::on_actionStartTraining_triggered()
{
    Session* l_session = SessionManager::pickSession();
    if (l_session){
        Training::startTraining(l_session);
        m_ui->statusBar->showMessage("Training session \"" + l_session->content()->title() + "\"...");
    }
}

/// @todo Invoke the process of starting up desktop control.
void SpeechControl::Windows::Main::on_actionStartDesktopControl_triggered()
{
    m_ui->statusBar->showMessage("This feature hasn't been implemented yet.");
    DesktopControl::Agent::start();
}

void SpeechControl::Windows::Main::on_btnDsktpCntrl_toggled(bool checked)
{
    QPushButton* l_btn = m_ui->btnDsktpCntrl;
    if (checked)
        l_btn->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    else
        l_btn->setStyleSheet(QString::null);
}

void SpeechControl::Windows::Main::on_btnDctn_toggled(bool checked)
{
    QPushButton* l_btn = m_ui->btnDctn;
    if (checked)
        l_btn->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    else
        l_btn->setStyleSheet(QString::null);
}

/// @todo Allow configuration option to show specific notifcations to prevent noise.
void SpeechControl::Windows::Main::desktopControlToggled(const bool p_val)
{
    QPushButton* l_btn = m_ui->btnDsktpCntrl;
    l_btn->setChecked(p_val);

    m_ui->statusBar->showMessage((( p_val == true ) ? "Activating desktop control..." : "Deactivating desktop control...") ,5);
    //PanelIcon::instance()->showMessage("Desktop Control State Changed",m_ui->statusBar->currentMessage());
}

/// @todo Allow configuration option to show specific notifcations to prevent noise.
void SpeechControl::Windows::Main::dictationToggled(const bool p_val)
{
    QPushButton* l_btn = m_ui->btnDctn;
    l_btn->setChecked(p_val);

    m_ui->statusBar->showMessage((( p_val == true ) ? "Activating dictation..." : "Deactivating dictation..." ) ,5);
    //PanelIcon::instance()->showMessage("Dictation State Changed",m_ui->statusBar->currentMessage());
}

void SpeechControl::Windows::Main::on_btnDsktpCntrl_clicked()
{
    if (DesktopControl::Agent::isActive())
        DesktopControl::Agent::stop();
    else
        DesktopControl::Agent::start();
}

void SpeechControl::Windows::Main::on_btnDctn_clicked()
{
    if (Dictation::Agent::isActive())
        Dictation::Agent::stop();
    else
        Dictation::Agent::start();
}
