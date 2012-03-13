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
    QTableWidget* view = ui->lstPlugins;
    view->clear();

    QList<QUuid> plgnLst = Factory::availablePlugins().keys();
    qDebug() << "[PluginsSettingsPane::updateUi()" << plgnLst.length() << "plug-ins installed.";
    view->setHorizontalHeaderItem(0,(new QTableWidgetItem("Enabled")));
    view->setHorizontalHeaderItem(1,(new QTableWidgetItem("Name")));
    view->setHorizontalHeaderItem(2,(new QTableWidgetItem("Version")));
    view->setColumnCount(3);
    view->setRowCount(plgnLst.count());
    int index = 0;

    Q_FOREACH (QUuid uuid, plgnLst) {
        GenericPlugin* plgn = new GenericPlugin (uuid);
        QCheckBox* checkBox = new QCheckBox (this);
        QLabel* title = new QLabel (plgn->name(), this);
        QLabel* version = new QLabel (QString::number (plgn->version()), this);

        checkBox->setChecked (plgn->isEnabled());
        checkBox->setGeometry(checkBox->geometry().x(),checkBox->geometry().y(),30,30);

        view->setCellWidget (index, 0, checkBox);
        view->setCellWidget (index, 1, title);
        view->setCellWidget (index, 2, version);

        index++;
    }
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
