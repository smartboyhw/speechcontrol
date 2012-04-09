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

#include "results.hpp"
#include "ui_adaptwizard-results.h"

using namespace SpeechControl;
using SpeechControl::Windows::Wizards::Pages::Results;

Results::Results (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::Results)
{
    ui->setupUi (this);
    updateUi();
}

void Results::updateUi()
{
    setLayout (ui->gridLayoutResults);
    ui->tabVerbose->setLayout (ui->verticalLayoutVerbose);
    ui->tabOverview->setLayout (ui->gridLayoutOverview);
}

void Results::cleanupPage()
{
}

void Results::initializePage()
{
    const bool accuracyRatingPassed = wizard()->property ("accuracy-rating") == "passed";
    ui->lblSessionCount->setText (tr ("Trained %1 Session(s)").arg (wizard()->property ("trained-session-count").toString()));
    ui->tabVerbose->setEnabled (accuracyRatingPassed);
    ui->tabWidget->setTabEnabled (1, accuracyRatingPassed);

    if (accuracyRatingPassed) {
        QString accuracyReport = tr ("Using %1 words, the accuracy of model <b>%2</b> %3 %4%.")
                                 .arg (wizard()->property ("total-word-count").toString())
                                 .arg (wizard()->property ("base-acoustic-model").toString())
                                 .arg (wizard()->property ("accuracy-value").toInt() > 0 ? "increased" : "decreased")
                                 .arg (wizard()->property ("accuracy-value").toString());
        ui->lblAdaptionReport->setText (accuracyReport);
        ui->lblAccuracy->setText (QString ("%1%").arg (wizard()->property ("accuracy-value").toString()));
        ui->lblAccuracyText->setPixmap (wizard()->property ("accuracy-value").toInt() > 0 ? QIcon::fromTheme ("arrow-up").pixmap (48, 48) : QIcon::fromTheme ("arrow-down").pixmap (48, 48));
    }
    else {
        ui->lblAdaptionReport->setText (tr ("<font color='red'>Adaption failed due to a complication with generating an accuracy report.</font>"));
        ui->lblAccuracy->setPixmap (QIcon::fromTheme ("dialog-error").pixmap (48, 48));
        ui->lblAccuracyText->setText ("N/A");
    }
}

Results::~Results()
{
    delete ui;
}

#include "ui/wizards/adapt/results.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
