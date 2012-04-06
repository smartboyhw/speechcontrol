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

#include "core.hpp"
#include "sessions/content.hpp"
#include <sessions/session.hpp>
#include "contents-wizard.hpp"
#include "ui/main-window.hpp"
#include "content-manager.hpp"
#include "ui_manager-content.h"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

ContentManager::ContentManager (QWidget* parent) :
    QDialog (parent),
    ui (new Ui::ContentManager),
    m_content (0)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    ui->groupBox->setLayout (ui->gridLayout_2);
    updateList();
}

ContentManager::~ContentManager()
{
    delete ui;
}

void ContentManager::updateList()
{
    ContentList lst = Content::allContents();
    ui->lstContent->clear();

    if (lst.empty()) {
        ui->lblTitle->setText (tr ("No Content"));
    }
    else {
        ui->lblTitle->setText (tr ("No Selection"));
    }

    if (!lst.empty()) {
        Q_FOREACH (const Content * l_cnt, lst) {
            QListWidgetItem* l_item = new QListWidgetItem (l_cnt->title(), ui->lstContent);
            l_item->setData (Qt::UserRole, l_cnt->id());
            ui->lstContent->addItem (l_item);

            if (m_content && m_content->id() == l_cnt->id()) {
                l_item->setSelected (true);
            }
        }

        if (!m_content) {
            ui->lstContent->setCurrentRow (0);
        }
    }
}

void ContentManager::on_btnSelect_clicked()
{
    QListWidgetItem* item = ui->lstContent->currentItem();

    if (item) {
        m_content = Content::obtain (item->data (Qt::UserRole).toString());
        accept();
    }
    else {
        m_content = 0;
        reject();
    }
}

Content* ContentManager::pickContent()
{
    ContentManager* wiz = new ContentManager;

    if (Content::allContents().empty()) {
        wiz->on_btnAdd_clicked();
        return wiz->m_content;
    }
    else {
        if (wiz->exec() == QDialog::Accepted) {
            return wiz->m_content;
        }
    }

    return 0;
}

/// @todo Invoke the Content addition wizard here.
void ContentManager::on_btnAdd_clicked()
{
    ContentWizard* wiz = new ContentWizard;

    if (wiz->exec() == QDialog::Accepted) {
        Core::mainWindow()->updateUi();
        updateList();
    }
}

void ContentManager::on_btnCancel_clicked()
{
    m_content = 0;
    reject();
}

void ContentManager::on_lstContent_itemSelectionChanged()
{
    const QListWidgetItem* item = ui->lstContent->currentItem();

    ui->btnSelect->setEnabled (! (item == 0));

    if (item) {
        const QString id = item->data (Qt::UserRole).toString();
        const Content* cnt = Content::obtain (id);
        ui->lblTitle->setText (cnt->title());
        ui->lblAuthor->setText (cnt->author());
        ui->lblWordCount->setText (tr ("%1 word(s)").arg (QString::number (cnt->words())));
        uint sessionCount = 0;

        SessionList lst = Session::allSessions();
        Q_FOREACH(const Session* itm, lst){
            if (itm->content()->id() == cnt->id())
                sessionCount ++;
        }

        ui->lblSessionCount->setText(tr("Used by %1 session(s)").arg(sessionCount));

    }
    else {
        ui->lblTitle->setText (tr ("No Selection"));
        ui->lblAuthor->clear();
        ui->lblWordCount->clear();
        ui->btnSelect->setEnabled (false);
    }
}

#include "ui/content-manager.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
