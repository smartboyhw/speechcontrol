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

#include <QTableWidget>

#include "core.hpp"
#include "sessions/session.hpp"
#include "desktopcontrol/agent.hpp"
#include "desktopcontrol/command.hpp"
#include "desktopcontrol-pane.hpp"
#include "ui_settingspane-desktopcontrol.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::DesktopControl;

DesktopControlSettingsPane::DesktopControlSettingsPane ( QWidget *parent ) :
    QFrame ( parent ),
    m_ui ( new Ui::DesktopControlSettingsPane ) {
    m_ui->setupUi ( this );
    updateContent();
}

DesktopControlSettingsPane::~DesktopControlSettingsPane() {
    delete m_ui;
}

void DesktopControlSettingsPane::changeEvent ( QEvent *e ) {
    QFrame::changeEvent ( e );
    switch ( e->type() ) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi ( this );
        break;
    default:
        break;
    }
}

const QString DesktopControlSettingsPane::title() const {
    return "Desktop Control";
}

const QString DesktopControlSettingsPane::id() const {
    return "dsktpcntrl";
}

void DesktopControlSettingsPane::updateContent() {
    m_ui->checkBoxEnable->setChecked(DesktopControl::Agent::instance()->isEnabled());
    AbstractCategory* l_glbl = DesktopControl::AbstractCategory::global();
    CommandList l_cmds = l_glbl->commands();
    QTableWidget* l_widg = m_ui->tableWidget;
    l_widg->clear();
    l_widg->setHorizontalHeaderLabels ( QStringList() << tr("Statement") << tr("Command") );
    l_widg->setRowCount ( 0 );
    l_widg->setColumnCount ( 2 );

    Q_FOREACH ( AbstractCommand* l_cmd, l_cmds ) {
        l_widg->setRowCount ( l_cmd->statements().count() + l_widg->rowCount() );
        int l_count = 0;
        Q_FOREACH ( const QString l_statement, l_cmd->statements() ) {
            const int l_row = l_widg->rowCount() - l_cmd->statements().count() - l_count;
            QTableWidgetItem* l_itemStatement = new QTableWidgetItem;
            QTableWidgetItem* l_commandStatement = new QTableWidgetItem;

            l_itemStatement->setText ( l_statement );
            l_commandStatement->setText ( l_cmd->id() );

            l_widg->setItem ( l_row, 0, l_itemStatement );
            l_widg->setItem ( l_row, 1, l_commandStatement );
            l_count--;
        }
    }
}

void DesktopControlSettingsPane::on_checkBoxEnable_toggled ( bool p_checked ) {
    Core::setConfiguration ( "DesktopControl/Enabled",p_checked );
    DesktopControl::Agent::instance()->setState ( ( ( p_checked ) ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled ) );
}

#include "desktopcontrol-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
