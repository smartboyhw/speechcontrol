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

PluginsSettingsPane::PluginsSettingsPane ( QWidget *parent ) :
    QFrame ( parent ),
    ui ( new Ui::PluginsSettingsPane ) {
    ui->setupUi ( this );
    updateList();
}

PluginsSettingsPane::~PluginsSettingsPane() {
    delete ui;
}

void PluginsSettingsPane::updateList() {
    QListWidget* l_lstWidg = ui->lstPlugins;
    l_lstWidg->clear();

    PluginList l_plgnLst = Factory::availablePlugins().values();

    Q_FOREACH(AbstractPlugin* l_plgn, l_plgnLst){
        QListWidgetItem* l_itm = new QListWidgetItem(l_plgn->name(),l_lstWidg);
        l_lstWidg->addItem(l_itm);
    }
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

const QString PluginsSettingsPane::title() const {
    return "Plugins";
}

const QString PluginsSettingsPane::id() const {
    return "plgn";
}

void PluginsSettingsPane::on_btnInfo_clicked() {

}

#ifdef HAVE_KDE
#include "plugins-pane.moc"
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
