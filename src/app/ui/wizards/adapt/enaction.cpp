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

#include "enaction.hpp"
#include "modelselection.hpp"
#include "sessionselection.hpp"
#include "ui/adapt-wizard.hpp"
#include "sessions/adaptionutility.hpp"
#include "ui_adaptwizard-enaction.h"

using namespace SpeechControl;
using namespace SpeechControl::Wizards::Pages;

Enaction::Enaction (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::Enaction), m_utility (0)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    setCommitPage (true);
    initializePage();
}

void Enaction::initalizePage()
{
    cleanupPage();
}

void Enaction::cleanupPage()
{
    ui->progressBarStatus->setFormat (QString::null);
    ui->progressBarStatus->setValue (0);
    ui->lblStatus->setText (QString::null);
    ui->btnAdapt->setEnabled(true);
}

void Enaction::on_btnAdapt_clicked()
{
    ui->btnAdapt->setEnabled (false);

    Session* session = 0;
    AcousticModel* model = 0;

    SessionSelection* sessionPage = (SessionSelection*) wizard()->page (Wizards::AdaptWizard::SessionSelectionPage);
    ModelSelection* modelPage = (ModelSelection*) wizard()->page (Wizards::AdaptWizard::ModelSelectionPage);

    session = sessionPage->session();
    model = modelPage->model();

    m_utility = new AdaptationUtility (session, model);
    connect (m_utility, SIGNAL (phaseStarted (Phases)), this, SLOT (on_mUtility_phaseStarted (Phases)));
    connect (m_utility, SIGNAL (phaseEnded (Phases)), this, SLOT (on_mUtility_phaseEnded (Phases)));
    m_utility->adapt();
}

void Enaction::on_mUtility_phaseEnded (const Phases& p_phase)
{
    ui->progressBarStatus->setFormat ("Ended phase " + m_utility->obtainPhaseText(p_phase) + "(" + QString::number((int) p_phase) + ")");
}

void Enaction::on_mUtility_phaseStarted (const Phases& p_phase)
{
    ui->progressBarStatus->setFormat ("Started phase " + m_utility->obtainPhaseText(p_phase) + "(" + QString::number((int) p_phase) + ")");
    ui->progressBarStatus->setValue((int) p_phase);

    if (p_phase == Phases::PhaseCompleteAdaption)
        this->wizard()->next();
}

bool Enaction::isComplete() const
{
    return m_utility && m_utility->currentPhase() == Phases::PhaseCompleteAdaption;
}

Enaction::~Enaction()
{
    delete ui;
}

#include "enaction.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;


