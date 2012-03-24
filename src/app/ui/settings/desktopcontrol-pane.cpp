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
#include <QLabel>

#include "core.hpp"
#include "desktopcontrol/agent.hpp"
#include "desktopcontrol/command.hpp"
#include "desktopcontrol-pane.hpp"
#include "uimain-window.hpp"
#include "dictation/agent.hpp"
#include "ui_settingspane-desktopcontrol.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::DesktopControl;

DesktopControlSettingsPane::DesktopControlSettingsPane () :
    m_ui (new Ui::DesktopControlSettingsPane)
{
    qDebug() << "[DesktopControlSettingsPane::{constructor}] Building desktop control settings pane...";
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);
    updateUi();
    qDebug() << "[DesktopControlSettingsPane::{constructor}] Built desktop control settings pane.";
}

DesktopControlSettingsPane::~DesktopControlSettingsPane()
{
    delete m_ui;
}

void DesktopControlSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString DesktopControlSettingsPane::title() const
{
    return "Desktop Control";
}

QString DesktopControlSettingsPane::id() const
{
    return "dsktpcntrl";
}

QPixmap DesktopControlSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("audio-headset").pixmap (32, 32);
}

void DesktopControlSettingsPane::restoreDefaults()
{
    Core::setConfiguration ("DesktopControl/Enabled", false);
    Core::setConfiguration ("DesktopControl/AutoStart", false);
    updateUi();
}

void DesktopControlSettingsPane::updateUi()
{
    m_ui->checkBoxEnable->setChecked (!Dictation::Agent::instance()->isEnabled() && DesktopControl::Agent::instance()->isEnabled());
    m_ui->checkBoxEnable->setEnabled (!Dictation::Agent::instance()->isEnabled());

    QTableWidget* widget = m_ui->tableWidget;
    widget->clear();

    widget->setEnabled (DesktopControl::Agent::instance()->isEnabled());
    AbstractCategory* glbl = DesktopControl::AbstractCategory::global();
    CommandList cmds = glbl->commands();
    widget->setHorizontalHeaderLabels (QStringList() << tr ("Statement") << tr ("Command"));
    widget->setRowCount (0);
    widget->setColumnCount (2);

    Q_FOREACH (AbstractCommand * cmd, cmds) {
        widget->setRowCount (cmd->statements().count() + widget->rowCount());
        int count = 0;
        Q_FOREACH (const QString statement, cmd->statements()) {
            const int row = widget->rowCount() - cmd->statements().count() - count;
            QLabel* command = new QLabel (statement, widget);
            QLabel* category = new QLabel (cmd->id(), widget);

            command->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Preferred);

            widget->setCellWidget (row, 0, command);
            widget->setCellWidget (row, 1, category);
            count--;
        }
    }
}

void DesktopControlSettingsPane::on_checkBoxEnable_toggled (bool p_checked)
{
    Core::setConfiguration ("DesktopControl/Enabled", p_checked);

    if (!p_checked && DesktopControl::Agent::instance()->isActive())
        DesktopControl::Agent::instance()->stop();

    Core::mainWindow()->updateWindow();
    updateUi();
}

void DesktopControlSettingsPane::on_checkBoxEnableStartup_toggled (bool p_checked)
{
    Core::setConfiguration ("DesktopControl/AutoStart", p_checked);
}

#include "desktopcontrol-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
