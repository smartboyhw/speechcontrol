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
#include <ui/adapt-wizard.hpp>
#include <core.hpp>
#include <acousticmodel.hpp>
#include <languagemodel.hpp>
#include "ui_settingspane-model.h"
#include <QMessageBox>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

ModelSettingsPane::ModelSettingsPane() :
    m_ui (new Ui::ModelSettingsPane)
{
    qDebug() << "[ModelSettingsPane::{constructor}] Building models settings pane...";
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);
    m_ui->tabLanguage->setLayout (m_ui->gridLayoutLanguage);
    m_ui->tabAcoustic->setLayout (m_ui->gridLayoutAcoustic);
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

    const uint index = m_ui->comboBoxAcousticModelView->currentIndex();
    const bool showAll = index == 0;
    const bool showUser = index == 1;
    const bool showSystem = index == 2;

    Q_FOREACH (const AcousticModel * model, models) {
        if (model->isSystem() && ! (showSystem || showAll))
            continue;
        else if (model->isUser() && ! (showUser || showAll))
            continue;

        QListWidgetItem* item = new QListWidgetItem (widget);
        item->setText (model->name());
        item->setData (Qt::UserRole, model->path());
        widget->addItem (item);
    }
}

void ModelSettingsPane::updateLanguageTab()
{
    QListWidget* widget = m_ui->listWidgetLanguage;
    LanguageModelList models = LanguageModel::allModels();
    widget->clear();

    const uint index = m_ui->comboBoxLanguageModelView->currentIndex();
    const bool showAll = index == 0;
    const bool showUser = index == 1;
    const bool showSystem = index == 2;

    Q_FOREACH (const LanguageModel * model, models) {
        if (model->isSystem() && ! (showSystem || showAll))
            continue;
        else if (model->isUser() && ! (showUser || showAll))
            continue;

        QListWidgetItem* item = new QListWidgetItem (widget);
        item->setText (model->name());
        item->setData (Qt::UserRole, model->path());
        widget->addItem (item);
    }
}

void ModelSettingsPane::on_btnAdaptAcoustic_clicked()
{
    Wizards::AdaptWizard* wiz = new Wizards::AdaptWizard (this->window());
    wiz->exec();
}

void ModelSettingsPane::on_btnAdaptDelete_clicked()
{
    QListWidgetItem* item = m_ui->listWidgetAcoustic->currentItem();

    if (item) {
        QString path = item->data (Qt::UserRole).toString();
        AcousticModel* model = new AcousticModel (path);

        if (model->isSystem()) {
            QMessageBox::warning (this, tr ("Cannot Remove Model"),
                                  tr ("SpeechControl is unable to delete system models.")
                                 );
            return;
        }

        if (QMessageBox::Yes == QMessageBox::question (this, tr ("Confirm Acoustic Model Deletion"),
                tr ("Are you sure you want to delete %1? There's no un-doing this action.").arg (model->name())
                , QMessageBox::Yes | QMessageBox::No)) {
            model->erase();
        }
    }
}

void ModelSettingsPane::on_btnLanguageDelete_clicked()
{
    QListWidgetItem* item = m_ui->listWidgetAcoustic->currentItem();

    if (item) {
        QString path = item->data (Qt::UserRole).toString();
        LanguageModel* model = LanguageModel::fromDirectory (path);

        if (model->isSystem()) {
            QMessageBox::warning (this, tr ("Cannot Remove Model"),
                                  tr ("SpeechControl is unable to delete system models.")
                                 );
            return;
        }

        if (QMessageBox::Yes == QMessageBox::question (this, tr ("Confirm Acoustic Model Deletion"),
                tr ("Are you sure you want to delete %1? There's no un-doing this action.").arg (model->name())
                , QMessageBox::Yes | QMessageBox::No)) {
            model->erase();
        }
    }
}

void ModelSettingsPane::on_comboBoxAcousticModelView_currentIndexChanged ()
{
    updateAcousticTab();
}

void ModelSettingsPane::on_comboBoxLanguageModelView_currentIndexChanged ()
{
    updateLanguageTab();
}

void ModelSettingsPane::on_checkBoxDefaultAcousticModel_clicked (const bool p_checked)
{
    if (p_checked){
        QString currentModel = m_ui->listWidgetAcoustic->currentItem()->data(Qt::UserRole).toString();
        Core::setConfiguration("Model/Acoustic",currentModel);
    } else {
        Core::setConfiguration("Model/Acoustic",QString());
    }

    qDebug() << "[ModelSettingsPane::on_checkBoxDefaultAcousticModel_clicked()]" << Core::configuration("Model/Acoustic") << p_checked;
}

void ModelSettingsPane::on_checkBoxDefaultLanguageModel_clicked (const bool p_checked)
{
    if (p_checked){
        QString currentModel = m_ui->listWidgetLanguage->currentItem()->data(Qt::UserRole).toString();
        Core::setConfiguration("Model/Language",currentModel);
    } else {
        Core::setConfiguration("Model/Language",QString());
    }

    qDebug() << "[ModelSettingsPane::on_checkBoxDefaultLanguageModel_clicked()]" << Core::configuration("Model/Language") << p_checked;
}

void ModelSettingsPane::on_listWidgetAcoustic_currentRowChanged (const int p_index)
{
    QString defaultModel = Core::configuration ("Model/Acoustic").toString();
    QString currentModel = m_ui->listWidgetAcoustic->item(p_index)->data(Qt::UserRole).toString();
    m_ui->checkBoxDefaultAcousticModel->setChecked(defaultModel == currentModel);
    qDebug() << "[ModelSettingsPane::on_listWidgetAcoustic_currentRowChanged()]" << defaultModel << currentModel;
}

void ModelSettingsPane::on_listWidgetLanguage_currentRowChanged (const int p_index)
{
    QString defaultModel = Core::configuration ("Model/Language").toString();
    QString currentModel = m_ui->listWidgetLanguage->item(p_index)->data(Qt::UserRole).toString();
    m_ui->checkBoxDefaultLanguageModel->setChecked(defaultModel == currentModel);
    qDebug() << "[ModelSettingsPane::on_listWidgetLanguage_currentRowChanged()]" << defaultModel << currentModel;
}

#include "ui/models-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
