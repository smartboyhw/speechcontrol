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
#include "session.hpp"
#include "windows/session-information-dialog.hpp"
#include "ui_settingspane-sessions.h"

#include <QMenu>
#include <QDebug>
#include <QListWidget>
#include <QMessageBox>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

SessionSettingsPane::SessionSettingsPane ( QWidget *parent ) :
    QFrame ( parent ),
    m_ui ( new Ui::SessionSettingsPane ) {
    m_ui->setupUi ( this );
    updateList();

    QList<QAction*> l_modifyActions;
    QList<QAction*> l_archiveActions;
    QMenu* l_modifyMenu = new QMenu ( m_ui->btnModify );
    QMenu* l_archiveMenu = new QMenu ( m_ui->btnArchive );

    l_modifyActions << m_ui->actionDelete
                    << m_ui->actionCopy;

    l_archiveActions << m_ui->actionBackup
                     << m_ui->actionRestoreBackup;

    l_modifyMenu->addActions ( l_modifyActions );
    l_archiveMenu->addActions ( l_archiveActions );

    m_ui->btnModify->setMenu ( l_modifyMenu );
    m_ui->btnArchive->setMenu ( l_archiveMenu );
}

void SessionSettingsPane::show() {
    updateList();
    QFrame::show();
}

SessionSettingsPane::~SessionSettingsPane() {
    delete m_ui;
}

void SessionSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

const QString SpeechControl::Windows::SessionSettingsPane::title() const {
    return "Sessions";
}

const QString SpeechControl::Windows::SessionSettingsPane::id() const {
    return "sssns";
}

void SessionSettingsPane::updateList() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    l_widget->clear();

    SessionList l_lst = Session::allSessions();
    Q_FOREACH ( const Session* l_sssn, l_lst ) {
        QListWidgetItem* l_item = new QListWidgetItem ( l_widget );
        l_item->setData ( Qt::UserRole,l_sssn->uuid().toString() );
        l_widget->addItem ( l_item );

        if ( l_sssn->content() ) {
            const QString l_lbl = l_sssn->content()->title();

            if ( l_lbl.isEmpty() ) {
                QString l_sssnUuid = l_sssn->uuid().toString();
                l_sssnUuid.chop ( l_sssnUuid.lastIndexOf ( "-" ) );
                l_item->setText ( l_sssnUuid );
            } else
                l_item->setText ( l_lbl );
        } else
            l_item->setText ( "* missing book data *" );

        l_widget->clearSelection();
        l_widget->setCurrentItem ( l_item );
    }


}

void SpeechControl::Windows::SessionSettingsPane::on_btnInfo_clicked() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    Q_FOREACH ( QListWidgetItem* l_itm, l_widget->selectedItems() ) {
        Session* l_ss = Session::obtain ( l_itm->data ( Qt::UserRole ).toString() );
        SessionInformationDialog l_dialog ( l_ss );
        l_dialog.setParent ( this );
        l_dialog.show();
    }
}

void SpeechControl::Windows::SessionSettingsPane::on_actionDelete_triggered() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    bool l_doAll = false;
    const bool l_multiple = l_widget->selectedItems().size() >= 2;

    if ( l_multiple ) {
        QMessageBox* l_msg = new QMessageBox ( this );
        l_msg->setIcon ( QMessageBox::Question );
        l_msg->setText ( tr ( "Do you want to delete %1 sessions?" ).arg ( l_widget->selectedItems().size() ) );
        l_msg->setInformativeText ( "All of the training progress for these sessions will be destroyed. "
                                    "However, the successfully formed acoustic models <b>will be</b> preserved." );
        l_msg->setWindowTitle ( "Delete Multiple Sessions" );
        l_msg->setStandardButtons ( QMessageBox::No | QMessageBox::Yes );
        l_msg->setDefaultButton ( QMessageBox::NoButton );
        l_doAll = ( l_msg->exec() == QMessageBox::Yes );
    }

    QList<QListWidgetItem*> l_itms = l_widget->selectedItems();
    foreach ( QListWidgetItem* l_itm, l_itms ) {
        qDebug() << l_itm->data ( Qt::UserRole );
        const QString l_uuid = l_itm->data ( Qt::UserRole ).toString();
        qDebug() << l_uuid;
        Session* l_ss = Session::obtain ( l_uuid );
        if ( ( l_multiple && l_doAll ) ||
                QMessageBox::Yes == QMessageBox::question ( this,"Confirm Session Deletion",
                        "Are you sure you want to <b>wipe all</b> of this session's data?",
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::No ) ) {
            l_ss->erase();
            l_widget->removeItemWidget ( l_itm );
            l_itms.removeAll ( l_itm );
        }
    }

    updateList();
}

/// @todo Add support for multiple session selection.
/// @todo Implement a means of just clicking once for all to be affected by this action.
void SpeechControl::Windows::SessionSettingsPane::on_actionCopy_triggered() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    QListWidgetItem* l_itm = l_widget->selectedItems().first();
    Session* l_ss = Session::obtain ( l_itm->data ( Qt::UserRole ).toString() );
    Session* l_newSs = l_ss->clone();
    if ( l_newSs )
        updateList();
}

/// @todo Add support for multiple session selection.
/// @todo Implement a means of just clicking once for all to be affected by this action.
void SpeechControl::Windows::SessionSettingsPane::on_actionBackup_triggered() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    QListWidgetItem* l_itm = l_widget->selectedItems().first();
    Session* l_ss = Session::obtain ( l_itm->data ( Qt::UserRole ).toString() );
    Session::Backup* l_bckpSs = l_ss->createBackup();
    if ( l_bckpSs )
        updateList();
}

/// @todo Add support for multiple session selection.
/// @todo Implement a means of just clicking once for all to be affected by this action.
void SpeechControl::Windows::SessionSettingsPane::on_actionRestoreBackup_triggered() {
    QListWidget* l_widget = m_ui->listWidgetSession;
    QListWidgetItem* l_itm = l_widget->selectedItems().first();
    Session* l_ss = Session::obtain ( l_itm->data ( Qt::UserRole ).toString() );

#ifdef RESTORESESSIONWIZARD_HPP
    Wizards::RestoreSessionWizard* l_wiz = new Wizards::RestoreSessionWizard ( this,l_ss );

    if ( l_wiz->exec() == QWizard::Accepted )
        updateList();
#endif //RESTORESESSIONWIZARD_HPP
}

#include "session-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
