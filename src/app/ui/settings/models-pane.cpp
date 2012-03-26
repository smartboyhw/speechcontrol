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

#include "models-pane.hpp"
#include <lib/acousticmodel.hpp>
#include <lib/languagemodel.hpp>
#include "ui_settingspane-model.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

ModelSettingsPane::ModelSettingsPane() :
    m_ui (new Ui::ModelSettingsPane)
{
    qDebug() << "[ModelSettingsPane::{constructor}] Building models settings pane...";
    m_ui->setupUi (this);
    this->setLayout(m_ui->gridLayout);
    m_ui->tabLanguage->setLayout(m_ui->gridLayoutLanguage);
    m_ui->tabAcoustic->setLayout(m_ui->gridLayoutAcoustic);
    updateUi();
    qDebug() << "[ModelSettingsPane::{constructor}] Built models settings pane.";
}

ModelSettingsPane::~ModelSettingsPane()
{
    delete m_ui;
}

void ModelSettingsPane::changeEvent (QEvent* e)
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

QString ModelSettingsPane::title() const
{
    return "Models";
}

QString ModelSettingsPane::id() const
{
    return "mdls";
}

QPixmap ModelSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void ModelSettingsPane::restoreDefaults()
{
}

void ModelSettingsPane::updateUi()
{
    updateAcousticTab();
    updateLanguageTab();
}

void ModelSettingsPane::updateAcousticTab()
{
    QListWidget* widget = m_ui->listWidgetAcoustic;
    AcousticModelList models = AcousticModel::allModels();
    widget->clear();

    Q_FOREACH(const AcousticModel* model, models){
        QListWidgetItem* item = new QListWidgetItem(widget);
        item->setText(model->name());
        widget->addItem(item);
    }
}

void ModelSettingsPane::updateLanguageTab()
{
    QListWidget* widget = m_ui->listWidgetLanguage;
    LanguageModelList models = LanguageModel::allModels();
    widget->clear();

    Q_FOREACH(const LanguageModel* model, models){
        QListWidgetItem* item = new QListWidgetItem(widget);
        item->setText(model->name());
        widget->addItem(item);
    }
}

#include "models-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
