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


#include <sentence.hpp>
#include <corpus.hpp>
#include <QMessageBox>
#include <QListWidget>

#include "ui_manager-session.h"

#include "sessions/session.hpp"
#include "windows/about-dialog.hpp"
#include "windows/main-window.hpp"
#include "books-manager.hpp"
#include "session-manager.hpp"
#include "core.hpp"


using namespace SpeechControl;
using SpeechControl::Windows::Managers::SessionManager;
using SpeechControl::Windows::Managers::BooksManager;

SessionManager::SessionManager ( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::SessionManager ),
    m_session ( 0 ) {
    m_ui->setupUi ( this );
    updateList();
}

SessionManager::~SessionManager() {
    delete m_ui;
}

void SessionManager::updateList() {
    m_ui->listSession->clear();

    SessionList l_lst = Session::allSessions();
    Q_FOREACH ( const Session* l_sessionItr, l_lst ) {
        QListWidgetItem* l_item = new QListWidgetItem ( m_ui->listSession );
        l_item->setData ( Qt::UserRole,l_sessionItr->uuid().toString() );
        l_item->setText ( tr ( "%1 - %2%" ).arg ( l_sessionItr->name() ).arg ( 30 ) );
        l_item->setIcon ( ( l_sessionItr->isCompleted() ) ? QIcon::fromTheme ( "task-complete" ) : QIcon::fromTheme ( "task-ongoing" ) );
        m_ui->listSession->addItem ( l_item );

        if ( m_session && m_session->uuid() == l_sessionItr->uuid() ) {
            l_item->setSelected ( true );
        }
    }

    if ( !m_session ) {
        m_ui->listSession->setCurrentRow ( 0 );
    }
}

Session* SessionManager::session() const {
    return m_session;
}

/// @todo Implement a means of selecting @see Session objects from the manager.
Session* SessionManager::pickSession() {
    SessionManager* l_win = new SessionManager;

    if ( Session::allSessions().empty() ) {
        l_win->on_btnCreate_clicked();
        return l_win->session();
    } else {
        if ( l_win->exec() == QDialog::Accepted ) {
            return l_win->session();
        }
    }

    return 0;
}

void SessionManager::on_btnCancel_clicked() {
    this->reject();
}

void SessionManager::on_btnOk_clicked() {
    if ( m_session->isCompleted() ) {
        if ( QMessageBox::Yes == QMessageBox::question ( this,
                tr ( "Continue Training?" ),
                tr ( "This session has already been completed, do you want to create a new session based on the content of this session?" ),
                QMessageBox::Yes,QMessageBox::No ) ) {
            m_session = Session::create ( m_session->content() );
            this->accept();
        }
    } else {
        this->accept();
    }
}

void SessionManager::on_btnCreate_clicked() {
    Content* l_content = BooksManager::doSelectContent();

    if ( l_content ) {
        Session* l_session = Session::create ( l_content );
        if ( l_session ) {
            m_session = l_session;
            Core::mainWindow()->updateContent();
        }
    }

    updateList();
}

void SessionManager::on_listSession_itemSelectionChanged() {
    m_session = 0;
    const QListWidgetItem* l_item = m_ui->listSession->currentItem();

    if ( l_item ) {
        m_session = Session::obtain ( QUuid ( l_item->data ( Qt::UserRole ).toString() ) );
    }
}

#include "session-manager.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
