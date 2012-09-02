/***
 *  This file is part of the SpeechControl project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 19:54:35 PM
 */

#include <QLabel>
#include <QTableWidget>

#include <lib/acousticmodel.hpp>
#include <core.hpp>

#include "global.hpp"
#include "service.hpp"
#include "command.hpp"
#include "settings-pane.hpp"
#include "ui_settingspane-desktopcontrol.h"

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
    m_ui->checkBoxEnable->setChecked (Service::instance()->isEnabled());
    AcousticModelList models = AcousticModel::allModels();

    QTableWidget* widget = m_ui->tableWidget;
    widget->clear();

    AbstractCategory* glbl = AbstractCategory::global();
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

void DesktopControlSettingsPane::on_checkBoxEnable_toggled (bool checked)
{
    Core::setConfiguration ("DesktopControl/Enabled", checked);

    if (!checked && Service::instance()->isActive())
        Service::instance()->stop();

    updateUi();
}

void DesktopControlSettingsPane::on_checkBoxEnableStart_toggled (bool checked)
{
    Core::setConfiguration ("DesktopControl/AutoStart", checked);
}

void DesktopControlSettingsPane::on_comboBoxAcousticModel_currentIndexChanged(int idx)
{
    QString path = m_ui->comboBoxAcousticModel->itemData(idx).toString();
    if (QDir(path).exists()) {
        AcousticModel newModel(path);
        Service::instance()->setDefaultAcousticModel(newModel);
        Service::instance()->setAcousticModel(newModel);
    }
}

#include "settings-pane.moc"
// kate: indent-mode cstyle; replace-tabs on;
