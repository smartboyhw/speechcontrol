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
 * @date 05/19/12 21:04:58 PM
 */

#include <QIcon>
#include <QMessageBox>

#include <lib/acousticmodel.hpp>

#include "core.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/adapt/modelselection.hpp"
#include "wizards/adapt/sessionselection.hpp"
#include "wizards/adapt/enaction.hpp"
#include "wizards/adapt/results.hpp"
#include "adapt-wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Windows::Pages;

AdaptWizard::AdaptWizard (QWidget* parent) :
    AbstractWizard (parent)
{
    // http://wiki.thesii.org/SpeechControl/UserGuides/AdaptAcousticModels
    QIcon icon = QIcon::fromTheme ("configure");
    setPixmap (QWizard::LogoPixmap, icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setWindowTitle (tr ("Adaption Wizard - SpeechControl"));
    setPage (AdaptWizard::IntroductionPage,
             new Windows::Pages::IntroductionPage (tr ("This wizard will help you adapt acoustic models for optimal performance in SpeechControl."
                                          " If this is the first time you've attempted to adapt an acoustic model, please "
                                          "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels\">http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels</a> "
                                          "for more detailed information about how adaption and adapting acoustic models enhance SpeechControl's performance."
                                             )));
    setPage (AdaptWizard::ModelSelectionPage, new ModelSelection);
    setPage (AdaptWizard::SessionSelectionPage, new SessionSelection);
    setPage (AdaptWizard::EnactionPage, new Pages::Enaction);
    setPage (AdaptWizard::ResultsPage, new Pages::Results);
    setPage (AdaptWizard::ConclusionPage,
             new Windows::Pages::ConclusionPage (tr ("You've successfully adapted your acoustic model to be used with SpeechControl. Click <b>Finish</b> to finalize your results and commit them to SpeechControl."
                                            " If this is the first time you've attempted to adapt an acoustic model, please "
                                            "visit <a href=\"http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels\">http://wiki.thesii.org/SpeechControl/UserGuides/AdaptingAcousticModels</a> "
                                            "for more detailed information about how adaption and adapting acoustic models enhance SpeechControl's performance."
                                           )));
}

int AdaptWizard::nextId() const
{
    switch (currentId()) {
    case AdaptWizard::IntroductionPage: {
        return AdaptWizard::ModelSelectionPage;
    }
    break;

    case AdaptWizard::ModelSelectionPage: {
        Pages::ModelSelection* page = (Pages::ModelSelection*) currentPage();

        if (page->model() && page->model()->isValid()) {
            return AdaptWizard::SessionSelectionPage;
        }
        else {
            QMessageBox::warning (topLevelWidget(), tr ("Invalid Model"),
                                  tr ("<h2>Invalid Model</h2>"
                                      "The model you've selected is invalid. Please select another."));
            return AdaptWizard::ModelSelectionPage;
        }
    }
    break;

    case AdaptWizard::SessionSelectionPage: {
        return AdaptWizard::EnactionPage;
    }
    break;

    case AdaptWizard::EnactionPage: {
        return AdaptWizard::ResultsPage;
    }
    break;

    case AdaptWizard::ResultsPage: {
        return AdaptWizard::ConclusionPage;
    }
    break;

    case AdaptWizard::ConclusionPage: {
        // just return, willis!
    } break;
    }

    return -1;
}

void AdaptWizard::accept()
{
    this->QDialog::accept();
}

void AdaptWizard::setSession (Session* p_session)
{
    ( (Pages::SessionSelection*) this->page (AdaptWizard::SessionSelectionPage))->setSession (p_session);
}

AdaptWizard::~AdaptWizard()
{

}

#include "ui/adapt-wizard.moc"
// kate: indent-mode cstyle; replace-tabs on;
