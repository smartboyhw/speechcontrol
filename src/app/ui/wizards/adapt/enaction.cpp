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
 * @date 05/19/12 21:05:36 PM
 */

#include <QDebug>
#include <QMessageBox>
#include <QTemporaryFile>

#include "modelselection.hpp"
#include "sessionselection.hpp"
#include "ui/adapt-wizard.hpp"
#include "sessions/accuracymeter.hpp"
#include "sessions/adaptionutility.hpp"
#include "sessions/adaptionutility.hpp"
#include "ui_adaptwizard-enaction.h"
#include "enaction.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows::Pages;

Enaction::Enaction (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::Enaction), m_utility (0), m_model (0), m_meter (0)
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
//    ui->progressBarStatus->setFormat (QString::null);
    ui->progressBarOverall->setFormat (QString::null);
//    ui->progressBarStatus->setValue (0);
    ui->progressBarOverall->setValue (0);
    ui->lblStatus->setText (QString::null);
    ui->btnAdapt->setEnabled (true);
}

void Enaction::on_btnAdapt_clicked()
{
    ui->btnAdapt->setEnabled (false);

    SessionSelection* sessionPage = (SessionSelection*) wizard()->page (Windows::AdaptWizard::SessionSelectionPage);
    ModelSelection* modelPage = (ModelSelection*) wizard()->page (Windows::AdaptWizard::ModelSelectionPage);

    m_sessions = sessionPage->sessions();
    m_model = modelPage->model();

    ui->progressBarOverall->setRange (0, m_sessions.length() * 2);
    ui->progressBarOverall->setValue (0);

    wizard()->setProperty ("trained-session-count", m_sessions.count());

    invokeAdaption (m_sessions.first());
}

void Enaction::invokeAdaption (Session* p_session)
{
    m_utility = new AdaptationUtility (p_session, m_model);
    connect (m_utility, SIGNAL (phaseStarted (AdaptationUtility::Phase)),
             this, SLOT (updateProgress (AdaptationUtility::Phase)));
    connect (m_utility, SIGNAL (phaseEnded (Phases)), this, SLOT (on_mUtility_phaseEnded (Phases)));
    connect (m_utility, SIGNAL (endedAdapting()), this, SLOT (on_mUtility_endedAdapting()));
    connect (m_utility, SIGNAL (startedAdapting()), this, SLOT (on_mUtility_startedAdapting()));
    m_utility->adapt();
    m_model = m_utility->resultingModel();
}

void Enaction::on_mUtility_endedAdapting()
{
    ui->progressBarOverall->setFormat ("%p%: Adapted using session '" + m_utility->session()->name() + "'");
    ui->progressBarOverall->setValue (ui->progressBarOverall->value() + 1);

    if (m_meter) {
        m_meter->deleteLater();
    }

    m_meter = new AccuracyMeter (m_utility->resultingModel());
    connect (m_meter, SIGNAL (assessmentCompleted ()), this, SLOT (on_mMeter_assessmentCompleted ()));
    m_meter->setSession (m_utility->session());
    m_meter->doAssessment (m_utility->hypothesis()->fileName());
}

void Enaction::on_mMeter_assessmentCompleted ()
{
    switch (m_meter->status()) {
    case AccuracyMeter::Successful: {
        QVariant accuracyData = wizard()->property ("accuracy-report");
        QVariantList dataSets;

        if (accuracyData.isValid()) {
            dataSets = accuracyData.toList();
        }

        dataSets << m_meter->data();
        wizard()->setProperty ("accuracy-report", dataSets);

        if (m_utility->session() != m_sessions.last())
            invokeAdaption (m_sessions.at (m_sessions.indexOf (m_utility->session()) + 1));

        qDebug() << "[Enaction::on_mMeter_assessmentCompleted()]" << m_meter->data();
        wizard()->setProperty ("accuracy-rating", "passed");
    }
    break;

    case AccuracyMeter::Error : {
        QMessageBox::critical (this, tr ("Failed to Determine Accuracy"),
                               tr ("SpeechControl was unable to determine the accuracy "
                                   "of the adapted acoustic model.\n\n<b>Error message</b>: %1\nOutput:\n<pre>%2</pre>"
                                  ).arg (m_meter->data() ["message"].toString()).arg (m_meter->data() ["output"].toString()));
        wizard()->setProperty ("accuracy-rating", "failed");
        wizard()->next();
    }
    break;

    default:
        break;
    }
}

void Enaction::on_mUtility_startedAdapting()
{
    ui->progressBarOverall->setFormat ("%p%: Adapting using session '" + m_utility->session()->name() + "'");
    ui->progressBarOverall->setValue (ui->progressBarOverall->value() + 1);
}

void Enaction::on_mUtility_phaseEnded (const Phases& p_phase)
{
//    ui->progressBarStatus->setFormat ("Ended phase " + m_utility->obtainPhaseText (p_phase) + "...");
    emit completeChanged();
}

void Enaction::updateProgress (AdaptationUtility::Phase phase)
{
//    ui->progressBarStatus->setFormat ("Started phase " + m_utility->obtainPhaseText (phase) + "...");
    ui->statusInfo->setText("State: " + m_utility->obtainPhaseText (phase));
    ui->progressBarOverall->setFormat ("%p%: Adapting using session '" + m_utility->session()->name() + "'");
    ui->progressBarOverall->setValue (ui->progressBarOverall->value() + 1);
}

bool Enaction::isComplete() const
{
    return m_utility && m_utility->currentPhase() == AdaptationUtility::CompleteAdaption && m_utility->session() == m_sessions.last();
}

Enaction::~Enaction()
{
    delete ui;
}

#include "ui/wizards/adapt/enaction.moc"
// kate: indent-mode cstyle; replace-tabs on;
