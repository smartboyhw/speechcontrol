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
    QTableWidget* table = ui->lstPlugins;
    table->clear();

    QList<QUuid> plgnLst = Factory::availablePlugins().keys();
    qDebug() << "[PluginsSettingsPane::updateUi()" << plgnLst.length() << "plug-ins installed.";
    table->setColumnCount(3);
    table->setRowCount(plgnLst.count());
    table->setHorizontalHeaderItem(0,(new QTableWidgetItem("[-]")));
    table->setHorizontalHeaderItem(1,(new QTableWidgetItem(tr("Name"))));
    table->setHorizontalHeaderItem(2,(new QTableWidgetItem(tr("Version"))));
    table->setColumnWidth(1,30);
    table->setColumnWidth(2,50);
    int index = 0;

    Q_FOREACH (QUuid uuid, plgnLst) {
        GenericPlugin* plgn = new GenericPlugin (uuid);
        QCheckBox* checkBox = new QCheckBox (this);
        QLabel* title = new QLabel (plgn->name(), this);
        QLabel* version = new QLabel (QString::number (plgn->version()), this);

        table->setCellWidget (index, 0, checkBox);
        table->setCellWidget (index, 1, title);
        table->setCellWidget (index, 2, version);

        checkBox->setChecked (plgn->isEnabled());
        checkBox->setGeometry(checkBox->geometry().x(),checkBox->geometry().y(),30,30);
        title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        if (!plgn->isSupported()){
            checkBox->setEnabled(false);
            title->setEnabled(false);
            version->setEnabled(false);
        }

        qDebug() << "[PluginsSettingsPane::updateUi()" << plgn->name() << "enabled?" << plgn->isEnabled();
        index++;
    }

    table->resizeColumnToContents(1);
}

QString PluginsSettingsPane::title() const
{
    return tr ("Plug-ins");
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
