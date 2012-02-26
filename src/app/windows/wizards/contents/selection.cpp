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

#include <QDebug>

#include "selection.hpp"
#include "ui_contentwizard-selectiontype.h"

using SpeechControl::Wizards::Pages::AdditionSelectionPage;

AdditionSelectionPage::AdditionSelectionPage ( QWidget *parent ) :
    QWizardPage ( parent ),
    m_ui ( new Ui::AdditionSelectionPage ) {
    m_ui->setupUi ( this );
    registerField ( "selection.wiki",m_ui->radioButtonWiki );
    registerField ( "selection.url",m_ui->radioButtonUrl );
    registerField ( "selection.custom",m_ui->radioButtonCustom );

    connect ( m_ui->radioButtonWiki,SIGNAL ( clicked ( bool ) ),this,SLOT ( updateDescription() ) );
    connect ( m_ui->radioButtonUrl,SIGNAL ( clicked ( bool ) ),this,SLOT ( updateDescription() ) );
    connect ( m_ui->radioButtonCustom,SIGNAL ( clicked ( bool ) ),this,SLOT ( updateDescription() ) );

    updateDescription();
}

void AdditionSelectionPage::updateDescription() {
    if ( m_ui->radioButtonUrl->isChecked() )
        m_ui->lblDescription->setText ( m_ui->radioButtonUrl->whatsThis() );
    else if ( m_ui->radioButtonWiki->isChecked() )
        m_ui->lblDescription->setText ( m_ui->radioButtonWiki->whatsThis() );
    else if ( m_ui->radioButtonCustom->isChecked() )
        m_ui->lblDescription->setText ( m_ui->radioButtonCustom->whatsThis() );
}

AdditionSelectionPage::~AdditionSelectionPage() {
    delete m_ui;
}

#include "selection.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
