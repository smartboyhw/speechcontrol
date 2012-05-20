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
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 13:32:06 PM
 */

#include <QDir>
#include <QDebug>

#include "sessions/session.hpp"
#include "sessions/content.hpp"

#include "sessionselection.hpp"
#include "ui_adaptwizard-sessionselection.h"

SPCHCNTRL_USE_NAMESPACE
SPCHCNTRL_UI_WIZARD_PAGES_USE_NAMESPACE

SessionSelection::SessionSelection (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::SessionSelection)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
}

void SessionSelection::setSessions (SessionList p_sessionList)
{
    m_listSession = p_sessionList;
}

void SessionSelection::initializePage()
{
    SessionList sessions = Session::allSessions();
    QListWidget* widget = ui->listWidgetSession;

    Q_FOREACH (Session * session, sessions) {
        if (session->isCompleted()) {
            const QString name = session->name();
            QListWidgetItem* item = new QListWidgetItem (name, widget);
            item->setData (Qt::UserRole, session->id());
            item->setSelected (m_listSession.contains (session));
        }
    }
}

void SessionSelection::cleanupPage()
{
    ui->listWidgetSession->clear();
    ui->lineEditUniqueWordCount->clear();
}

bool SessionSelection::validatePage()
{
    if (!m_listSession.isEmpty())
        this->setSubTitle ("<b><font color=red>Please select at least one session for adapting.</font></b>");

    return !m_listSession.isEmpty();
}

void SessionSelection::on_listWidgetSession_itemSelectionChanged ()
{
    QList<QListWidgetItem*> items = ui->listWidgetSession->selectedItems();
    m_listSession.clear();

    if (items.isEmpty()) {
        ui->lineEditUniqueWordCount->setText ("no sessions selected");
    }
    else {
        uint count = 0;
        Q_FOREACH (QListWidgetItem * item, items) {
            const QString id = item->data (Qt::UserRole).toString();
            const Session* session = Session::obtain (id);

            if (session->content()) {
                count += session->content()->uniqueWords();
            }

            m_listSession << Session::obtain (id);
        }
        ui->lineEditUniqueWordCount->setText (QString::number (count));
    }
}

SessionList SessionSelection::sessions() const
{
    return m_listSession;
}

void SessionSelection::setSession (Session* p_session)
{
    if (!m_listSession.contains (p_session))
        m_listSession.append (p_session);
}

SessionSelection::~SessionSelection()
{
    delete ui;
}

#include "ui/wizards/adapt/sessionselection.moc"
// kate: indent-mode cstyle; replace-tabs on;
