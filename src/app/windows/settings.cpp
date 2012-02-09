/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QMessageBox>
#include <QListWidgetItem>

#include "settings.hpp"
#include "settings/general.hpp"
#include "ui_settingsDialog.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

Settings* Settings::s_inst = 0;

Settings::Settings(QWidget *m_prnt) :
    QDialog(m_prnt),
    m_ui(new Ui::SettingsDialog)
{
    s_inst = this;
    m_ui->setupUi(this);
}

/// @todo Add it to the list of options.
void Settings::addPanel(QWidget* p_pane)
{
    const QString l_paneTitle = p_pane->property("Title").toString();
    const QString l_paneID = p_pane->property("ID").toString();
    QListWidgetItem* l_itm = new QListWidgetItem(l_paneTitle,instance()->m_ui->lstNavigation);

    instance()->m_panes.insert(l_paneID,p_pane);
    l_itm->setData(Qt::UserRole,l_paneID);
    p_pane->setParent(instance()->m_ui->frmPageContainer);
    p_pane->hide();
}

/// @todo Add the initial panes here.
Settings* Settings::instance()
{
    if (s_inst == 0){
        s_inst = new Settings;
        GeneralSettingsPane* l_generalPane = new GeneralSettingsPane;
        addPanel(l_generalPane);
    }

    return s_inst;
}

/// @todo Remove this from the list of options.
void Settings::removePanel(const QString& p_paneID)
{
    instance()->m_panes.remove(p_paneID);
}

void Settings::on_lstNavigation_itemSelectionChanged()
{
    QWidget* l_container = instance()->m_ui->frmPageContainer;
    QListWidget* l_lstNavi = instance()->m_ui->lstNavigation;

    if (!l_lstNavi->selectedItems().empty()){
        QListWidgetItem* l_itm = l_lstNavi->selectedItems().first();
        const QString l_id = l_itm->data(Qt::UserRole).toString();

        QWidget* l_pane = m_panes[l_id];
        Q_FOREACH(QObject* l_subPane, l_container->children()){
            ((QWidget*) l_subPane)->hide();
        }

        l_pane->setParent(l_container);
        l_pane->show();
    }
}

Settings::~Settings()
{
    delete m_ui;
}

void SpeechControl::Windows::Settings::on_buttonBox_rejected()
{
    this->close();
}

void SpeechControl::Windows::Settings::on_buttonBox_accepted()
{
    this->close();
}
