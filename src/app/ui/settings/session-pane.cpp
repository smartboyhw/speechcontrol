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

#include "session-pane.hpp"
#include "sessions/session.hpp"
#include "ui/session-information-dialog.hpp"
#include "ui_settingspane-sessions.h"

#include <QMenu>
#include <QDebug>
#include <QListWidget>
#include <QMessageBox>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

SessionSettingsPane::SessionSettingsPane() :
    m_ui (new Ui::SessionSettingsPane)
{
    qDebug() << "[SessionSettingsPane::{constructor}] Building session settings pane...";
    m_ui->setupUi (this);
    updateUi();
    qDebug() << "[SessionSettingsPane::{constructor}] Built session settings pane.";

    QList<QAction*> l_modifyActions;
    QMenu* l_modifyMenu = new QMenu (m_ui->btnModify);

    l_modifyActions << m_ui->actionDelete
                    << m_ui->actionCopy;

    l_modifyMenu->addActions (l_modifyActions);
    m_ui->btnModify->setMenu (l_modifyMenu);
}

SessionSettingsPane::~SessionSettingsPane()
{
    delete m_ui;
}

void SessionSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString SpeechControl::Windows::SessionSettingsPane::title() const
{
    return "Sessions";
}

QString SpeechControl::Windows::SessionSettingsPane::id() const
{
    return "sssns";
}

void SessionSettingsPane::updateUi()
{
    QListWidget* widget = m_ui->listWidgetSession;
    widget->clear();

    SessionList l_lst = Session::allSessions();
    Q_FOREACH (const Session * sssn, l_lst) {
        QListWidgetItem* item = new QListWidgetItem (widget);
        QString sssnId = sssn->id();
        item->setData (Qt::UserRole, sssnId);
        widget->addItem (item);

        if (sssn->content()) {
            const QString lbl = sssn->name();

            if (lbl.isEmpty()) {
                item->setText (sssnId);
            }
            else {
                item->setText (lbl);
            }
        }
        else {
            item->setText (tr ("* missing book data *"));
        }

        widget->clearSelection();
        widget->setCurrentItem (item);
    }
}

QPixmap SessionSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void SessionSettingsPane::restoreDefaults()
{

}

void SpeechControl::Windows::SessionSettingsPane::on_btnInfo_clicked()
{
    QListWidget* l_widget = m_ui->listWidgetSession;
    Q_FOREACH (QListWidgetItem * l_itm, l_widget->selectedItems()) {
        Session* l_ss = Session::obtain (l_itm->data (Qt::UserRole).toString());
        SessionInformationDialog l_dialog (l_ss);
        l_dialog.setParent (this);
        l_dialog.show();
    }
}

void SpeechControl::Windows::SessionSettingsPane::on_actionDelete_triggered()
{
    QListWidget* l_widget = m_ui->listWidgetSession;
    bool l_doAll = false;
    const bool l_multiple = l_widget->selectedItems().size() >= 2;

    if (l_multiple) {
        QMessageBox* l_msg = new QMessageBox (this);
        l_msg->setIcon (QMessageBox::Question);
        l_msg->setText (tr ("Do you want to delete %1 sessions?").arg (l_widget->selectedItems().size()));
        l_msg->setInformativeText (tr ("All of the training progress for these sessions will be destroyed. "
                                       "However, the successfully formed acoustic models <b>will be</b> preserved."));
        l_msg->setWindowTitle (tr ("Delete Multiple Sessions"));
        l_msg->setStandardButtons (QMessageBox::No | QMessageBox::Yes);
        l_msg->setDefaultButton (QMessageBox::NoButton);
        l_doAll = (l_msg->exec() == QMessageBox::Yes);
    }

    QList<QListWidgetItem*> l_itms = l_widget->selectedItems();
    foreach (QListWidgetItem * l_itm, l_itms) {
        qDebug() << "[SessionSettingsPane::on_actionDelete_triggered()]" << l_itm->data (Qt::UserRole);
        const QString l_uuid = l_itm->data (Qt::UserRole).toString();
        qDebug() << "[SessionSettingsPane::on_actionDelete_triggered()]" << l_uuid;
        Session* l_ss = Session::obtain (l_uuid);

        if ( (l_multiple && l_doAll) ||
                QMessageBox::Yes == QMessageBox::question (this,
                        tr ("Confirm Session Deletion"),
                        tr ("Are you sure you want to <b>wipe all</b> of this session's data?"),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::No)) {
            l_ss->erase();
            l_widget->removeItemWidget (l_itm);
            l_itms.removeAll (l_itm);
        }
    }

    updateUi();
}

void SpeechControl::Windows::SessionSettingsPane::on_actionCopy_triggered()
{
    QListWidget* l_widget = m_ui->listWidgetSession;

    QList<QListWidgetItem*> l_itms = l_widget->selectedItems();
    foreach (QListWidgetItem * l_itm, l_itms) {
        qDebug() << "[SessionSettingsPane::on_actionCopy_triggered()]" << l_itm->data (Qt::UserRole);
        const QString id = l_itm->data (Qt::UserRole).toString();
        Session* ss = Session::obtain (id);
        ss->clone();
    }

    updateUi();
}

#include "ui/session-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
