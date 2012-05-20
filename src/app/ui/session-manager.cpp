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

#include <QListWidget>
#include <QMessageBox>
#include <QListWidgetItem>

#include "core.hpp"
#include "sessions/session.hpp"
#include "sessions/phrase.hpp"
#include "sessions/corpus.hpp"
#include "session-information-dialog.hpp"
#include "ui/about-dialog.hpp"
#include "ui_manager-session.h"
#include "content-manager.hpp"
#include "session-manager.hpp"

using namespace SpeechControl;
using SpeechControl::Windows::SessionInformationDialog;
using SpeechControl::Windows::Managers::SessionManager;
using SpeechControl::Windows::Managers::ContentManager;

SessionManager::SessionManager (QWidget* parent) :
    QDialog (parent),
    m_ui (new Ui::SessionManager),
    m_session (0)
{
    m_ui->setupUi (this);
    updateList();
}

SessionManager::~SessionManager()
{
    delete m_ui;
}

void SessionManager::updateList()
{
    m_ui->listSession->clear();

    SessionList lst = Session::allSessions();
    Q_FOREACH (const Session * sessionItr, lst) {
        QListWidgetItem* item = new QListWidgetItem (m_ui->listSession);
        item->setData (Qt::UserRole, sessionItr->id());
        item->setText (tr ("%1 - %2%").arg (sessionItr->name()).arg ( (int) (sessionItr->assessProgress() * 100.0)));
        item->setIcon ( (sessionItr->isCompleted()) ? QIcon::fromTheme ("task-complete") : QIcon::fromTheme ("task-ongoing"));
        m_ui->listSession->addItem (item);

        if (m_session && m_session->id() == sessionItr->id()) {
            item->setSelected (true);
        }
    }

    if (!m_session) {
        m_ui->listSession->setCurrentRow (0);
    }

    m_ui->btnSelect->setEnabled(m_session != 0);
}

Session* SessionManager::session() const
{
    return m_session;
}

/// @todo Implement a means of selecting @see Session objects from the manager.
Session* SessionManager::pickSession()
{
    SessionManager* win = new SessionManager;

    if (Session::allSessions().empty()) {
        win->on_btnCreate_clicked();
        return win->session();
    }
    else {
        if (win->exec() == QDialog::Accepted) {
            return win->session();
        }
    }

    return 0;
}

void SessionManager::on_btnCancel_clicked()
{
    this->reject();
}

void SessionManager::on_btnSelect_clicked()
{
    if (m_session->isCompleted()) {
        if (QMessageBox::Yes == QMessageBox::question (this,
                tr ("Continue Training?"),
                tr ("This session has already been completed, do you want to create a new session based on the content of this session?"),
                QMessageBox::Yes, QMessageBox::No)) {
            m_session = Session::create (m_session->content());
            this->accept();
        }
    }
    else {
        this->accept();
    }
}

void SessionManager::on_btnCreate_clicked()
{
    Content* content = ContentManager::pickContent();

    if (content) {
        Session* session = Session::create (content);

        if (session) {
            m_session = session;
        }
    }

    updateList();
}

void SessionManager::on_listSession_itemDoubleClicked (QListWidgetItem* p_item)
{
    if (p_item) {
        Session* session = Session::obtain (p_item->data (Qt::UserRole).toString());

        if (session) {
            SessionInformationDialog dialog (session);
            dialog.exec();
            updateList();
        }
    }
}

void SessionManager::on_listSession_itemSelectionChanged()
{
    const QListWidgetItem* item = m_ui->listSession->currentItem();

    if (item) {
        Session* session = Session::obtain (item->data (Qt::UserRole).toString());
        m_ui->btnSelect->setEnabled(session != 0);

        if (session) {
            const int progress = (int) (session->assessProgress() * 100.0);

            if (progress == 100)
                m_ui->progressBar->setFormat (tr ("Completed"));
            else
                m_ui->progressBar->setFormat (tr ("%p% complete"));

            m_ui->progressBar->setValue (progress);

            m_session = session;
        }
        else {
            m_ui->progressBar->setFormat (tr ("no session selected"));
            m_ui->progressBar->setValue (0.0);
        }
    }
}

#include "ui/session-manager.moc"
// kate: indent-mode cstyle; replace-tabs on; 
