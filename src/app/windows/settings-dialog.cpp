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
#include <QMessageBox>
#include <QListWidgetItem>

#include "core.hpp"
#include "windows/main-window.hpp"
#include "settings-dialog.hpp"
#include "settings/general-pane.hpp"
#include "settings/plugins-pane.hpp"
#include "settings/content-pane.hpp"
#include "settings/session-pane.hpp"
#include "settings/voxforge-pane.hpp"
#include "settings/desktopcontrol-pane.hpp"
#include "settings/dictation-pane.hpp"
#include "ui_settings-dialog.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

Settings* Settings::s_inst = 0;

Settings::Settings() : QDialog() {
    s_inst =this;
    buildWindow();
}

Settings::Settings ( QWidget *m_prnt ) :
    QDialog ( m_prnt ) {
    s_inst =this;
    buildWindow();
}

void Settings::buildWindow() {
    qDebug() << "[Settings::buildWindow()] Building settings window.";
    m_ui = new Ui::SettingsDialog;
    m_ui->setupUi ( this );

    addPane ( new GeneralSettingsPane );
    addPane ( new PluginsSettingsPane );
    addPane ( new DesktopControlSettingsPane );
    addPane ( new DictationSettingsPane );
    addPane ( new ContentSettingsPane );
    addPane ( new SessionSettingsPane );
    addPane ( new VoxforgeSettingsPane );
    qDebug() << "[Settings::buildWindow()] Built settings window.";
}


Settings::Settings ( const Settings& p_other ) : QDialog() {

}

/// @todo Add it to the list of options.
void Settings::addPane ( AbstractSettingsPane* p_pane ) {
    QListWidget* listWidget = instance()->m_ui->lstNavigation;
    QListWidgetItem* l_itm = new QListWidgetItem ( p_pane->title(), listWidget );
    instance()->m_panes.insert ( p_pane->id(), p_pane );
    l_itm->setData ( Qt::UserRole,p_pane->id() );
    l_itm->setIcon ( p_pane->pixmap() );
    p_pane->setParent ( instance()->m_ui->frmPageContainer );
    p_pane->hide();
}

void Settings::displayPane ( const QString& p_panelID ) {
    QWidget* l_currentPane = instance()->m_panes.value ( p_panelID );
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;

    if ( l_currentPane != 0 ) {
        l_currentPane->show();
        l_lstNavi->setCurrentItem ( instance()->findPaneItem ( p_panelID ) );
    } else {
        Core::mainWindow()->setStatusMessage ( tr ( "Invalid settings panel ID '%1'" ).arg ( p_panelID ) );
        instance()->m_panes.value ( "gnrl" )->show();
        l_lstNavi->setCurrentItem ( instance()->findPaneItem ( "gnrl" ) );
    }

    if ( !instance()->isVisible() ) {
        instance()->open();
    }
}

QListWidgetItem* Settings::findPaneItem ( const QString& p_panelID ) {
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;
    for ( uint i = 0; i < ( uint ) l_lstNavi->children().length(); i++ ) {
        QListWidgetItem* l_itm = l_lstNavi->item ( i );
        if ( l_itm->data ( Qt::UserRole ).toString() == p_panelID ) {
            return l_itm;
        }
    }

    return 0;
}

/// @todo Remove this from the list of options.
void Settings::removePane ( const QString& p_panelID ) {
    instance()->m_panes.remove ( p_panelID );
}

void Settings::on_lstNavigation_itemSelectionChanged() {
    QWidget* l_container = instance()->m_ui->frmPageContainer;
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;

    if ( !l_lstNavi->selectedItems().empty() ) {
        QListWidgetItem* l_itm = l_lstNavi->selectedItems().first();
        const QString l_id = l_itm->data ( Qt::UserRole ).toString();

        QWidget* l_pane = m_panes[l_id];
        Q_FOREACH ( QObject* l_subPane, l_container->children() ) {
            ( ( QWidget* ) l_subPane )->hide();
        }

        l_pane->setParent ( l_container );
        l_pane->show();
    }
}

void Settings::on_buttonBox_clicked ( QAbstractButton* p_button ) {
    QDialogButtonBox::StandardButton buttonState = m_ui->buttonBox->standardButton ( p_button );

    switch ( buttonState ) {
    case QDialogButtonBox::Ok:
        break;

    case QDialogButtonBox::Help:
        break;

    case QDialogButtonBox::Reset:
        /// @todo Add functionality to reset options to the state they were before opening the dialog.
        break;

    case QDialogButtonBox::RestoreDefaults:
        /// @todo Add functionality to set the values of the properties all back to default.
        break;
    }
}
Settings::~Settings() {
    delete m_ui;
}

AbstractSettingsPane::AbstractSettingsPane ( ) {
}

void AbstractSettingsPane::addPane ( AbstractSettingsPane* p_subPane ) {
    m_panes.insert ( p_subPane->id(),p_subPane );
}

bool AbstractSettingsPane::hasPane ( const QString& p_paneID ) const {
    return m_panes.contains ( p_paneID );
}

/// @todo Ensure that the accompanying QListWidgetItem is removed as well.
void AbstractSettingsPane::removePane ( AbstractSettingsPane* p_subPane ) {
    m_panes.remove ( p_subPane->id() );
    p_subPane->deleteLater();
}

void AbstractSettingsPane::removePane ( const QString& p_subPaneID ) {
    removePane ( m_panes.value ( p_subPaneID ) );
}

AbstractSettingsPane::~AbstractSettingsPane() {

}
#include "settings-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
