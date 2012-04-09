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

#include <acousticmodel.hpp>

#include "core.hpp"
#include "services/dictation/service.hpp"
#include "services/desktopcontrol/service.hpp"
#include "services/desktopcontrol/command.hpp"
#include "ui/main-window.hpp"
#include "ui_settingspane-desktopcontrol.h"

#include "desktopcontrol-pane.hpp"

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
    m_ui->checkBoxEnable->setChecked (!Dictation::Service::instance()->isEnabled() && DesktopControl::Service::instance()->isEnabled());
    m_ui->checkBoxEnable->setEnabled (!Dictation::Service::instance()->isEnabled());
    AcousticModelList models = AcousticModel::allModels();

    QTableWidget* widget = m_ui->tableWidget;
    widget->clear();

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

    m_ui->comboBoxAcousticModel->clear();

    Q_FOREACH (AcousticModel * model, models) {
        m_ui->comboBoxAcousticModel->addItem (model->name(), model->path());
    }

    m_ui->comboBoxAcousticModel->setCurrentIndex (m_ui->comboBoxAcousticModel->findData (Core::configuration ("DesktopControl/AcousticModel")));
}

void DesktopControlSettingsPane::on_checkBoxEnable_toggled (bool p_checked)
{
    Core::setConfiguration ("DesktopControl/Enabled", p_checked);

    if (!p_checked && DesktopControl::Service::instance()->isActive())
        DesktopControl::Service::instance()->stop();

    Core::mainWindow()->updateUi();
    updateUi();
}

void DesktopControlSettingsPane::on_checkBoxEnableStartup_toggled (bool p_checked)
{
    Core::setConfiguration ("DesktopControl/AutoStart", p_checked);
}

void DesktopControlSettingsPane::on_deftAcousticModel_textEdited (const QString& text)
{
    if (QDir (text).exists()) {
        AcousticModel newModel (text);
        DesktopControl::Service::instance()->setDefaultAcousticModel (newModel);
        DesktopControl::Service::instance()->setAcousticModel (newModel);
    }
}

#include "ui/settings/desktopcontrol-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
