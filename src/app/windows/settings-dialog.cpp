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

Settings::Settings ( QWidget *m_prnt ) :
    QDialog ( m_prnt ),
    m_ui ( new Ui::SettingsDialog ) {
    s_inst = this;
    m_ui->setupUi ( this );
}

/// @todo Add it to the list of options.
void Settings::addPanel ( QWidget* p_panelWidget ) {
    const QString l_paneTitle = p_panelWidget->property ( "Title" ).toString();
    const QString l_paneID = p_panelWidget->property ( "ID" ).toString();
    QListWidgetItem* l_itm = new QListWidgetItem ( l_paneTitle,instance()->m_ui->lstNavigation );

    instance()->m_panes.insert ( l_paneID,p_panelWidget );
    l_itm->setData ( Qt::UserRole,l_paneID );
    p_panelWidget->setParent ( instance()->m_ui->frmPageContainer );
    p_panelWidget->hide();
}

void Settings::switchToPanel ( const QString& p_panelID ) {
    QWidget* l_currentPane = instance()->m_panes.value ( p_panelID );
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;

    if ( l_currentPane != 0 ) {
        l_currentPane->show();
        l_lstNavi->setCurrentItem ( instance()->findPanelItem ( p_panelID ) );
    } else {
        Core::mainWindow()->setStatusMessage ( tr ( "Invalid settings panel ID '%1'" ).arg ( p_panelID ) );
        instance()->m_panes.value ( "gnrl" )->show();
        l_lstNavi->setCurrentItem ( instance()->findPanelItem ( "gnrl" ) );
    }

    if ( !instance()->isVisible() ) {
        instance()->open();
    }
}

QListWidgetItem* Settings::findPanelItem ( const QString& p_panelID ) {
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;
    for ( uint i = 0; i < ( uint ) l_lstNavi->children().length(); i++ ) {
        QListWidgetItem* l_itm = l_lstNavi->item ( i );
        if ( l_itm->data ( Qt::UserRole ).toString() == p_panelID ) {
            return l_itm;
        }
    }

    return 0;
}

Settings* Settings::instance() {
    if ( s_inst == 0 ) {
        s_inst = new Settings;
        GeneralSettingsPane* l_generalPane = new GeneralSettingsPane;
        PluginsSettingsPane* l_pluginsPane = new PluginsSettingsPane;
        ContentSettingsPane* l_booksPane = new ContentSettingsPane;
        VoxforgeSettingsPane* l_voxforgePane = new VoxforgeSettingsPane;
        SessionSettingsPane* l_sessionPane = new SessionSettingsPane;
        DesktopControlSettingsPane* l_dsktpCntrlPane = new DesktopControlSettingsPane;
        DictationSettingsPane* l_dctnPane = new DictationSettingsPane;

        addPanel ( l_generalPane );
        addPanel ( l_pluginsPane );
        addPanel ( l_booksPane );
        addPanel ( l_sessionPane );
        addPanel ( l_dctnPane );
        addPanel ( l_dsktpCntrlPane );
        addPanel ( l_voxforgePane );

        l_generalPane->show();
    }

    return s_inst;
}

/// @todo Remove this from the list of options.
void Settings::removePanel ( const QString& p_panelID ) {
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

Settings::~Settings() {
    delete m_ui;
}

void SpeechControl::Windows::Settings::on_buttonBox_accepted() {
    this->close();
}

#include "settings-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
