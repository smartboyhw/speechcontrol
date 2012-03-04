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

#include "urlselect.hpp"
#include "app/sessions/content.hpp"
#include "app/windows/contents-wizard.hpp"
#include "ui_contentwizard-urlselection.h"

#include <QDebug>
#include <QTextStream>
#include <QUrl>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

using SpeechControl::Wizards::ContentWizard;
using SpeechControl::Wizards::Pages::UrlSelectionPage;

UrlSelectionPage::UrlSelectionPage ( QWidget *parent ) :
    QWizardPage ( parent ),
    m_ui ( new Ui::UrlSelectionPage ) {
    m_ui->setupUi ( this );
}

UrlSelectionPage::~UrlSelectionPage() {
    delete m_ui;
}

void UrlSelectionPage::on_lineEdit_textChanged ( const QString p_path ) {
    const QUrl l_url = QUrl::fromUserInput ( p_path );
    if ( l_url.isValid() ) {
        updateContentData ( l_url );
        m_previousValue = p_path;
    } else {
        m_ui->lineEdit->setText ( m_previousValue );
    }
}

void UrlSelectionPage::on_toolButton_clicked() {
    const QString l_path = QFileDialog::getOpenFileName ( this, tr ( "Open Text File" ), QDir::homePath(), tr ( "SpeechControl Books (*.spch)" ) );
    const QUrl l_fileUrl ( l_path );
    qDebug() << l_fileUrl << l_path;

    if ( l_fileUrl.isValid() ) {
        TextContentSource l_txtSrc;
        if ( l_txtSrc.setUrl ( l_fileUrl ) ) {
            m_ui->lineEditAuthor->setText ( l_txtSrc.author() );
            m_ui->lineEditTitle->setText ( l_txtSrc.title() );
            m_ui->plainTextEdit->setPlainText ( l_txtSrc.text() );
            m_ui->lineEdit->setText ( l_fileUrl.toString() );

            ContentWizard* l_wiz = dynamic_cast<ContentWizard*> ( wizard() );
            l_wiz->setSource ( &l_txtSrc );
        }
    } else {
        if ( QMessageBox::Ok == QMessageBox::warning ( this,
                tr ( "Unable to Load Content" ),
                tr ( "The file you've chosen is either in the wrong format or isn't valid.\nPlease try another file." ),QMessageBox::Ok,QMessageBox::Cancel ) ) {
            on_toolButton_clicked();
        }
    }
}

void UrlSelectionPage::updateContentData ( const QUrl p_fileUrl ) {
    if ( p_fileUrl.isValid() ) {
        TextContentSource l_txtSrc;
        if ( l_txtSrc.setUrl ( p_fileUrl ) ) {
            m_ui->lineEditAuthor->setText ( l_txtSrc.author() );
            m_ui->lineEditTitle->setText ( l_txtSrc.title() );
            m_ui->plainTextEdit->setPlainText ( l_txtSrc.text() );
            m_ui->lineEdit->setText ( p_fileUrl.toString() );
        }
    }
}

#include "urlselect.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
