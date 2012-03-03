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

#include "sessions/session.hpp"

#include "contents-wizard.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/contents/selection.hpp"
#include "wizards/contents/urlselect.hpp"
#include "wizards/contents/wikiselect.hpp"
#include "wizards/contents/customsource.hpp"
#include "core.hpp"

#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QVariant>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

ContentWizard::ContentWizard ( QWidget *parent ) :
    WizardBase ( parent ), m_src ( 0 ) {
    QIcon l_icon = QIcon::fromTheme ( "text-plain" );
    setPixmap ( QWizard::LogoPixmap,l_icon.pixmap ( 32,32,QIcon::Active,QIcon::On ) );
    setWindowTitle ( tr ( "Book Addition Wizard - SpeechControl" ) );
    setPage ( ContentWizard::IntroductionPage,
              ( new Wizards::Pages::IntroductionPage ( "This wizard allows you to add a new book into SpeechControl's collection." ) ) );
    setPage ( ContentWizard::AdditionSelectionPage,
              ( new Wizards::Pages::AdditionSelectionPage ( this ) ) );
    setPage ( ContentWizard::UriSelectionPage,
              ( new Wizards::Pages::UrlSelectionPage ( this ) ) );
    setPage ( ContentWizard::WikiSourcePage,
              ( new Wizards::Pages::WikiSourcePage ( this ) ) );
    setPage ( ContentWizard::CustomSelectionPage,
              ( new Wizards::Pages::CustomSourcePage ( this ) ) );
    setPage ( ContentWizard::ConclusionPage,
              ( new Wizards::Pages::ConclusionPage ( "You've successfully added a book into SpeechControl." ) ) );
}

void ContentWizard::setSource ( AbstractContentSource* p_src ) {
    if ( p_src != 0 ) {
        m_src = new AbstractContentSource ( *p_src );
        qDebug() << "Got source" << m_src->id();
    }
}

/// @todo Add a page showing
int ContentWizard::nextId() const {
    switch ( currentId() ) {
    case IntroductionPage:
        return AdditionSelectionPage;
        break;

    case AdditionSelectionPage: {
        m_src = 0;
        if ( field ( "selection.wiki" ).toBool() ) {
            return WikiSourcePage;
        } else if ( field ( "selection.url" ).toBool() ) {
            return UriSelectionPage;
        } else if ( field ( "selection.custom" ).toBool() ) {
            return CustomSelectionPage;
        }
    }
    break;

    case WikiSourcePage:
    case UriSelectionPage:
    case CustomSelectionPage:
        return ConclusionPage;
        break;

    case ConclusionPage: {
        Q_ASSERT ( m_src != 0 );
        Content* l_cntn = m_src->generate();
        if ( l_cntn == 0 ) {
            QMessageBox::warning ( Core::mainWindow(),
                                   "Failure Creating Content",
                                   "There was an issue creating your content; thus resulting in a failure.",
                                   QMessageBox::Ok
                                 );
            return AdditionSelectionPage;
        } else {
            return -1;
        }
    }
    break;
    }

    return QWizard::nextId();
}

ContentWizard::~ContentWizard() {
}

#include "contents-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
