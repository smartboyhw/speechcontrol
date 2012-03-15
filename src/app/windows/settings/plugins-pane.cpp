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
#include <windows/plugin-info-dialog.hpp>
#include "ui_settingspane-plugins.h"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;
using namespace SpeechControl::Windows;

PluginsSettingsPane::PluginsSettingsPane () :
    ui (new Ui::PluginsSettingsPane)
{
    qDebug() << "[PluginsSettingsPane::{constructor}] Building plugins settings pane...";
    ui->setupUi (this);
    this->setLayout(ui->gridLayout);
    updateUi();
    connect (ui->lstPlugins, SIGNAL (itemClicked (QListWidgetItem*)), this, SLOT (on_lstPlugins_itemActivated (QListWidgetItem*)));
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

    QList<QUuid> plgnLst = Factory::availablePlugins().keys();
    qDebug() << "[PluginsSettingsPane::updateUi()" << plgnLst.length() << "plug-ins installed.";

    Q_FOREACH (QUuid uuid, plgnLst) {
        GenericPlugin* plgn = new GenericPlugin (uuid);
        QListWidgetItem* item = new QListWidgetItem (plgn->name(), list);
        item->setFlags (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        item->setHidden(plgn->isSupported());

        if (plgn->isLoaded())
            item->setIcon(Factory::plugin(plgn->uuid())->pixmap());
        else
            item->setIcon(QIcon::fromTheme("dialog-error"));

        item->font().setBold(plgn->isLoaded());
        item->setCheckState (plgn->isEnabled() ? Qt::Checked : Qt::Unchecked);
        item->setData (Qt::UserRole, plgn->uuid().toString());

        qDebug() << "[PluginsSettingsPane::updateUi()" << plgn->name() << "enabled?" << plgn->isEnabled();
    }

    ui->lstPlugins->setCurrentItem (ui->lstPlugins->item (0));
}

void PluginsSettingsPane::on_lstPlugins_itemSelectedChanged()
{
    ui->btnInfo->setEnabled (!ui->lstPlugins->selectedItems().isEmpty());
}

void PluginsSettingsPane::on_lstPlugins_itemActivated (QListWidgetItem* p_item)
{
    GenericPlugin* plgn = new GenericPlugin (p_item->data (Qt::UserRole).toString());
    const bool isChecked = p_item->checkState() == Qt::Checked;
    p_item->setCheckState (isChecked ? Qt::Unchecked : Qt::Checked);
    Factory::pluginConfiguration (plgn->uuid())->setValue ("Plugin/Enabled", isChecked);
    qDebug() << "[PluginsSettingsPane::on_table_cellClicked()]" << plgn->name() << "is now enabled?" << plgn->isEnabled();
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
