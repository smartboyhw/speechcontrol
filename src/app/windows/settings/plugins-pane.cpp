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

#include "factory.hpp"
#include "plugins-pane.hpp"
#include "ui_settingspane-plugins.h"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;
using namespace SpeechControl::Windows;

PluginsSettingsPane::PluginsSettingsPane () :
    ui (new Ui::PluginsSettingsPane)
{
    qDebug() << "[PluginsSettingsPane::{constructor}] Building plugins settings pane...";
    ui->setupUi (this);
    updateUi();
    qDebug() << "[PluginsSettingsPane::{constructor}] Built plugins settings pane.";
}

PluginsSettingsPane::~PluginsSettingsPane()
{
    delete ui;
}

void PluginsSettingsPane::updateUi()
{
    QTableWidget* l_view = ui->lstPlugins;
    l_view->clear();

    PluginList l_plgnLst = Factory::availablePlugins().values();
    qDebug() << "[PluginsSettingsPane::updateUi()" << l_plgnLst.length() << "plug-ins installed.";
    l_view->setHorizontalHeaderLabels(QStringList() << "Enabled" << "Name" << "Version");
    l_view->setRowCount(l_plgnLst.count());
    l_view->setColumnWidth(0,30);
    l_view->setColumnCount(3);
    int index = 0;

    Q_FOREACH (AbstractPlugin * l_plgn, l_plgnLst) {
        QTableWidgetItem* l_autoStart = new QTableWidgetItem;
        QTableWidgetItem* l_pluginTitle = new QTableWidgetItem;
        QTableWidgetItem* l_pluginVersion = new QTableWidgetItem;

        l_autoStart->setFlags (Qt::ItemIsUserCheckable);
        l_autoStart->setCheckState (l_plgn->isEnabled() ? Qt::Checked : Qt::Unchecked);
        l_pluginTitle->setText(l_plgn->name());
        l_pluginTitle->setToolTip(l_plgn->description());
        l_pluginVersion->setText(QString::number(l_plgn->version()));

        l_view->setItem(index,0,l_autoStart);
        l_view->setItem(index,1,l_pluginTitle);
        l_view->setItem(index,2,l_pluginVersion);
        index++;
    }
}

QString PluginsSettingsPane::title() const
{
    return tr("Plug-ins");
}

QString PluginsSettingsPane::id() const
{
    return "plgn";
}

bool PluginsSettingsPane::containsText (const QString& p_query) const
{

}

QPixmap PluginsSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("list-add").pixmap (32, 32);
}

void PluginsSettingsPane::resetPanel()
{

}

void PluginsSettingsPane::restoreDefaults()
{

}

void PluginsSettingsPane::on_btnInfo_clicked()
{

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
