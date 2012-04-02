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
    this->setLayout(ui->gridLayout);
    updateList();
}

ContentManager::~ContentManager()
{
    delete ui;
}

void ContentManager::updateList()
{
    ContentList l_lst = Content::allContents();
    ui->lstContent->clear();

    if (l_lst.empty()) {
        ui->lblTitle->setText (tr ("No Content"));
    }
    else {
        ui->lblTitle->setText (tr ("No Selection"));
    }

    if (!l_lst.empty()) {
        Q_FOREACH (const Content * l_cnt, l_lst) {
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
    QListWidgetItem* l_item = ui->lstContent->currentItem();

    if (l_item) {
        m_content = Content::obtain (l_item->data (Qt::UserRole).toString());
        accept();
    }
    else {
        m_content = 0;
        reject();
    }
}

Content* ContentManager::pickContent()
{
    ContentManager* l_wiz = new ContentManager;

    if (Content::allContents().empty()) {
        l_wiz->on_btnAdd_clicked();
        return l_wiz->m_content;
    }
    else {
        if (l_wiz->exec() == QDialog::Accepted) {
            return l_wiz->m_content;
        }
    }

    return 0;
}

/// @todo Invoke the Content addition wizard here.
void ContentManager::on_btnAdd_clicked()
{
    ContentWizard* l_wiz = new ContentWizard;

    if (l_wiz->exec() == QDialog::Accepted) {
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
    const QListWidgetItem* l_item = ui->lstContent->currentItem();

    if (l_item) {
        const QString id = l_item->data (Qt::UserRole).toString();
        const Content* cnt = Content::obtain (id);
        ui->lblTitle->setText (cnt->title());
        ui->lblWordCount->setText (QString::number (cnt->words()));
        ui->btnSelect->setEnabled (true);
    }
    else {
        ui->lblTitle->setText (tr ("No Selection"));
        ui->lblWordCount->setText (0);
        ui->btnSelect->setEnabled (false);
    }
}

#include "content-manager.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
