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

#include "factory.hpp"
#include "plugins-pane.hpp"
#include "ui_settingspane-plugins.h"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;
using namespace SpeechControl::Windows;

PluginsSettingsPane::PluginsSettingsPane ( ) :
    ui ( new Ui::PluginsSettingsPane ) {
        qDebug() << "[PluginsSettingsPane::{constructor}] Building plugins settings pane...";
        ui->setupUi ( this );
        updateUi();
        qDebug() << "[PluginsSettingsPane::{constructor}] Built plugins settings pane.";
    }

PluginsSettingsPane::~PluginsSettingsPane() {
    delete ui;
}

void PluginsSettingsPane::updateUi() {
    QListWidget* l_lstWidg = ui->lstPlugins;
    l_lstWidg->clear();

    PluginList l_plgnLst = Factory::availablePlugins().values();

    Q_FOREACH ( AbstractPlugin* l_plgn, l_plgnLst ) {
        QListWidgetItem* l_itm = new QListWidgetItem ( l_plgn->name(),l_lstWidg );
        l_lstWidg->addItem ( l_itm );
    }
}

QString PluginsSettingsPane::title() const {
    return "Plugins";
}

QString PluginsSettingsPane::id() const {
    return "plgn";
}

bool PluginsSettingsPane::containsText ( const QString& p_query ) const {

}

QPixmap PluginsSettingsPane::pixmap() const {
    return QIcon::fromTheme ( "configure" ).pixmap ( 32,32 );
}

void PluginsSettingsPane::resetPanel() {

}

void PluginsSettingsPane::restoreDefaults() {

}

void PluginsSettingsPane::on_btnInfo_clicked() {

}

void PluginsSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

#include "plugins-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
