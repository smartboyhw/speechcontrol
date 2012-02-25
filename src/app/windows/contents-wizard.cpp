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


#include "session.hpp"

#include "contents-wizard.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/contents/selection.hpp"
#include "wizards/contents/fileselect.hpp"
#include "wizards/contents/wikiselect.hpp"

#include <QIcon>
#include <QDebug>
#include <QVariant>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

ContentWizard::ContentWizard ( QWidget *parent ) :
    WizardBase ( parent ) {
    QIcon l_icon = QIcon::fromTheme ( "text-plain" );
    setPixmap ( QWizard::LogoPixmap,l_icon.pixmap ( 32,32,QIcon::Active,QIcon::On ) );
    setWindowTitle ( tr ( "Book Addition Wizard - SpeechControl" ) );
    setPage ( ContentWizard::IntroductionPage,
              ( new Wizards::Pages::IntroductionPage ( "This wizard allows you to add a new book into SpeechControl's collection." ) ) );
    setPage ( ContentWizard::AdditionSelectionPage,
              ( new Wizards::Pages::AdditionSelectionPage ( this ) ) );
    setPage ( ContentWizard::FileSelectionPage,
              ( new Wizards::Pages::FileSelectionPage ( this ) ) );
    setPage ( ContentWizard::WikiSourcePage,
              ( new Wizards::Pages::WikiSourcePage ( this ) ) );
    setPage ( ContentWizard::ConclusionPage,
              ( new Wizards::Pages::ConclusionPage ( "You've successfully added a book into SpeechControl." ) ) );
}

int ContentWizard::nextId() const {
    switch ( currentId() ) {
    case IntroductionPage:
        return AdditionSelectionPage;
        break;

    case AdditionSelectionPage:
        if ( field ( "selection.wiki" ).toBool() )
            return WikiSourcePage;
        else if ( field ( "selection.file" ).toBool() )
            return FileSelectionPage;
        break;

    case WikiSourcePage:
        return ConclusionPage;
        break;

    case FileSelectionPage:
        return ConclusionPage;
        break;

    case ConclusionPage:
        return -1;
        break;
    }

    return QWizard::nextId();
}

void ContentWizard::accept() {
    qDebug() << currentId() << nextId();

    if ( currentId() == -1 || currentId() == ConclusionPage ) {
        Content* l_cntn = 0;
        if ( field ( "selection.file" ).toBool() ) {
            qDebug() << field ( "file.content" ).toString();
            l_cntn = Content::create ( field ( "file.author" ).toString(),
                                       field ( "file.title" ).toString(),
                                       field ( "file.content" ).toString() );
        }

        if ( l_cntn ) {
            qDebug() << l_cntn->title() << "created." << l_cntn->words() << "words found.";
            WizardBase::accept();
        } else {
            qDebug() << "Content not created.";
            WizardBase::reject();
        }
    }
}

ContentWizard::~ContentWizard() {
}

#include "contents-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
