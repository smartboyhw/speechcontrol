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
    setWindowTitle ( tr ( "Content Addition Wizard - SpeechControl" ) );
    setPage ( ContentWizard::IntroductionPage,
              ( new Wizards::Pages::IntroductionPage ( tr ( "This wizard allows you to add a new book into SpeechControl's collection." ) ) ) );
    setPage ( ContentWizard::SourceSelectionPage,
              ( new Wizards::Pages::SourceSelectionPage ) );
    setPage ( ContentWizard::ConclusionPage,
              ( new Wizards::Pages::ConclusionPage ( tr ( "You've successfully added a book into SpeechControl." ) ) ) );
}

void ContentWizard::setSource ( AbstractContentSource* p_src ) {
    Q_ASSERT ( p_src != 0 );
    m_src = new AbstractContentSource ( *p_src );
    qDebug() << "Got source" << m_src->id();
}

int ContentWizard::nextId() const {
    switch ( currentId() ) {
    case IntroductionPage:
        return SourceSelectionPage;
        break;

    case SourceSelectionPage:
        return ConclusionPage;
        break;

    case ConclusionPage: {
        Q_ASSERT ( m_src != 0 );
        Content* l_cntn = m_src->generate();
        if ( l_cntn == 0 ) {
            QMessageBox::warning ( 0,
                                   tr ( "Failure Creating Content" ),
                                   tr ( "There was an issue creating your content; thus resulting in a failure." ),
                                   QMessageBox::Ok
                                 );
            return SourceSelectionPage;
        }

        return -1;
    }
    break;
    }

    return QWizard::nextId();
}

ContentWizard::~ContentWizard() {
}

#include "contents-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
