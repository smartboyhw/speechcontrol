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
#include <QListWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QLabel>
#include <QSettings>

#include "factory.hpp"
#include "plugins-pane.hpp"
#include "ui/plugin-info-dialog.hpp"
#include "ui_settingspane-plugins.h"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;
using namespace SpeechControl::Windows;

PluginsSettingsPane::PluginsSettingsPane () :
    ui (new Ui::PluginsSettingsPane)
{
    qDebug() << "[PluginsSettingsPane::{constructor}] Building plugins settings pane...";
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    updateUi();
    connect (ui->lstPlugins, SIGNAL (itemClicked (QListWidgetItem*)), this, SLOT (on_lstPlugins_itemActivated (QListWidgetItem*)));
    connect (ui->checkBoxEnabled, SIGNAL (toggled (bool)), this, SLOT (on_checkBoxEnabled_toggled (bool)));
    connect (ui->lstPlugins, SIGNAL (itemSelectionChanged()), this, SLOT (on_lstPlugins_itemSelectedChanged()));
    qDebug() << "[PluginsSettingsPane::{constructor}] Built plugins settings pane.";
}

PluginsSettingsPane::~PluginsSettingsPane()
{
    delete ui;
}

/// @todo Disable an item if it's not supported to be loaded.
void PluginsSettingsPane::updateUi()
{
    QListWidget* list = ui->lstPlugins;
    list->clear();

    QStringList plgnLst = Factory::availablePlugins().keys();
    qDebug() << "[PluginsSettingsPane::updateUi()" << plgnLst.length() << "plug-ins installed." << plgnLst;

    Q_FOREACH (QString id, plgnLst) {
        GenericPlugin* plgn = new GenericPlugin (id);
        QListWidgetItem* item = new QListWidgetItem (plgn->name(), list);

        if (plgn->isLoaded())
            item->setIcon (Factory::plugin (plgn->id())->pixmap());
        else
            item->setIcon (QIcon::fromTheme ("dialog-error"));

        item->font().setBold (plgn->isLoaded());
        item->setData (Qt::UserRole, plgn->id());

        qDebug() << "[PluginsSettingsPane::updateUi()" << plgn->id() << plgn->name() << "enabled?" << plgn->isEnabled();
    }

    ui->lstPlugins->setCurrentItem (ui->lstPlugins->item (0));
    on_lstPlugins_itemActivated (ui->lstPlugins->item (0));
}

void PluginsSettingsPane::on_lstPlugins_itemSelectedChanged()
{
    ui->btnInfo->setEnabled (!ui->lstPlugins->selectedItems().isEmpty());
}

void PluginsSettingsPane::on_lstPlugins_itemActivated (QListWidgetItem* p_item)
{
    if (p_item) {
        GenericPlugin* plgn = new GenericPlugin (p_item->data (Qt::UserRole).toString());
        ui->checkBoxEnabled->setChecked (plgn->isEnabled());
        ui->checkBoxAutoStart->setEnabled (plgn->isEnabled());
        ui->checkBoxAutoStart->setChecked (Factory::doesLoadOnStart (plgn->id()));
        ui->btnLoadPlugin->setText (plgn->isLoaded() ? "Unload" : "Load");
        ui->btnLoadPlugin->setEnabled (true);
        ui->btnInfo->setEnabled (true);
    }
    else {
        ui->btnLoadPlugin->setEnabled (false);
        ui->checkBoxAutoStart->setChecked (false);
        ui->checkBoxAutoStart->setEnabled (false);
        ui->checkBoxEnabled->setChecked (false);
        ui->checkBoxEnabled->setEnabled (false);
        ui->btnInfo->setEnabled (false);
    }
}

void PluginsSettingsPane::on_btnLoadPlugin_clicked()
{
    QListWidgetItem* item = ui->lstPlugins->currentItem();
    const QString id (item->data (Qt::UserRole).toString());

    if (!Factory::isPluginLoaded (id) && Factory::loadPlugin (id)) {
        item->setIcon (Factory::plugin (id)->pixmap());
    }
    else {
        Factory::unloadPlugin (id);
        item->setIcon (QIcon::fromTheme ("dialog-error"));
    }

    on_lstPlugins_itemActivated (item);
}

void PluginsSettingsPane::on_checkBoxAutoStart_toggled (const bool p_checked)
{
    QListWidgetItem* item = ui->lstPlugins->currentItem();
    GenericPlugin* plgn = new GenericPlugin (item->data (Qt::UserRole).toString());
    Factory::setLoadOnStart (plgn->id(), p_checked);
}

void PluginsSettingsPane::on_checkBoxEnabled_toggled (const bool p_checked)
{
    QListWidgetItem* item = ui->lstPlugins->currentItem();
    GenericPlugin* plgn = new GenericPlugin (item->data (Qt::UserRole).toString());
    ui->checkBoxAutoStart->setEnabled (plgn->isEnabled());
    Factory::pluginSettings (plgn->id())->setValue ("Plugin/Enabled", p_checked);
    qDebug() << "[PluginsSettingsPane::on_table_cellClicked()]" << plgn->name() << "is now enabled?" << plgn->isEnabled() << p_checked;
}

QString PluginsSettingsPane::title() const
{
    return tr ("Plug-ins");
}

QString PluginsSettingsPane::id() const
{
    return "plgn";
}

QPixmap PluginsSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("list-add").pixmap (32, 32);
}

void PluginsSettingsPane::restoreDefaults()
{
    // herp derp.
}

void PluginsSettingsPane::on_btnInfo_clicked()
{
    QListWidgetItem* item = ui->lstPlugins->currentItem();

    if (item) {
        GenericPlugin* plgn = new GenericPlugin (item->data (Qt::UserRole).toString());
        PluginInfoDialog window (plgn, this);
        window.exec();
    }
}

void PluginsSettingsPane::changeEvent (QEvent* e)
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

#include "plugins-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
