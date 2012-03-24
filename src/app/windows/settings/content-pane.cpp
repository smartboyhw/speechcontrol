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

#include "content-pane.hpp"
#include "sessions/content.hpp"
#include "windows/content-information-dialog.hpp"
#include "windows/contents-wizard.hpp"
#include "ui_settingspane-content.h"

#include <QListWidget>
#include <QMessageBox>
#include <QErrorMessage>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

ContentSettingsPane::ContentSettingsPane () :
    ui (new Ui::ContentSettingsPane)
{
    qDebug() << "[ContentSettingsPane::{constructor}] Building content settings pane...";
    ui->setupUi (this);
    updateUi();
    qDebug() << "[ContentSettingsPane::{constructor}] Built content settings pane.";
}

ContentSettingsPane::~ContentSettingsPane()
{
    delete ui;
}

void ContentSettingsPane::show()
{
    updateUi();
    QFrame::show();
}

void ContentSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString ContentSettingsPane::title() const
{
    return "Content";
}

QString ContentSettingsPane::id() const
{
    return "cntns";
}

QPixmap ContentSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("text-plain").pixmap (32, 32);
}

void ContentSettingsPane::updateUi()
{
    QListWidget* widget = ui->lstContent;
    ContentList lst = Content::allContents();

    widget->clear();

    if (!lst.empty()) {
        Q_FOREACH (const Content * cnt, lst) {
            const QString lbl = cnt->title();
            QListWidgetItem* item = new QListWidgetItem (widget);
            item->setData (Qt::UserRole, cnt->id().toString());
            widget->addItem (item);

            if (lbl.isEmpty()) {
                item->setText (tr ("Unnamed [%1 word(s)]").arg (lbl).arg (cnt->words()));
            }
            else {
                item->setText (tr ("%1 [%2 word(s)]").arg (lbl).arg (cnt->words()));
            }
        }
    }
}

void ContentSettingsPane::on_btnDelete_clicked()
{
    QListWidget* widget = ui->lstContent;
    bool doAll = false;
    const bool multiple = widget->selectedItems().size() >= 2;

    if (multiple) {
        QMessageBox* msg = new QMessageBox (this);
        msg->setIcon (QMessageBox::Question);
        msg->setText (tr ("Do you want to delete %1 content(s)?").arg (widget->selectedItems().size()));
        msg->setInformativeText (tr ("All of the Sessions created from these Contents will be preserved."));
        msg->setWindowTitle (tr ("Delete Multiple Contents"));
        msg->setStandardButtons (QMessageBox::No | QMessageBox::Yes);
        msg->setDefaultButton (QMessageBox::NoButton);
        doAll = (msg->exec() == QMessageBox::Yes);
    }


    if (!widget->selectedItems().empty()) {
        Q_FOREACH (QListWidgetItem * itm, widget->selectedItems()) {
            Content* cntn = Content::obtain (itm->data (Qt::UserRole).toString());

            if ( (multiple && doAll) ||
                    QMessageBox::Yes == QMessageBox::question (this,
                            tr ("Confirm Content Delete"),
                            tr ("Are you sure you want to delete this book '%1' by '%2'?\nAny session connected to the book will become invalid and untrainable.").arg (cntn->title()).arg (cntn->author()),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No)) {
                cntn->erase();
                widget->removeItemWidget (itm);
            }
        }

        updateUi();
    }
}

void ContentSettingsPane::on_btnAdd_clicked()
{
    Wizards::ContentWizard* l_wiz = new Wizards::ContentWizard (this);

    if (l_wiz->exec() == QDialog::Accepted) {
        updateUi();
    }
}

void ContentSettingsPane::on_btnInfo_clicked()
{
    QListWidget* l_widg = ui->lstContent;

    if (!l_widg->selectedItems().empty()) {
        Q_FOREACH (QListWidgetItem * l_itm, l_widg->selectedItems()) {
            Content* l_cntn = Content::obtain (l_itm->data (Qt::UserRole).toString());
            ContentInformationDialog l_dialog (l_cntn);
            l_dialog.exec();
        }
    }
}

#include "content-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
