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

#include <lib/acousticmodel.hpp>
#include <noisedictionary.hpp>

#include <QDir>
#include <QDebug>
#include "modelselection.hpp"
#include "ui_adaptwizard-modelselection.h"

using namespace SpeechControl;
using SpeechControl::Wizards::Pages::ModelSelection;

ModelSelection::ModelSelection (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::ModelSelection), m_model (0)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    initializePage();
}

AcousticModel* ModelSelection::model()
{
    return m_model;
}

void ModelSelection::initializePage()
{
    ui->comboBoxModel->clear();

    AcousticModelList models = AcousticModel::allModels();
    bool hasOnlyUserLocal = false;
    bool hasOnlySystemLocal = false;

    Q_FOREACH (const AcousticModel * model, models) {
        const bool isUserLocal = model->path().contains (QDir::homePath());

        if (!model->path().endsWith ("/.")) {
            const QString name = QString ("(") +
                                 QString (isUserLocal ? "user-specific" : "system-wide") +
                                 QString (") ") +
                                 QDir (model->path()).dirName();

            qDebug() << model->path();
            ui->comboBoxModel->addItem (name, model->path());
        }

        if (isUserLocal && !hasOnlyUserLocal && !hasOnlySystemLocal)
            hasOnlyUserLocal = true;

        if (!isUserLocal && !hasOnlySystemLocal && !hasOnlyUserLocal)
            hasOnlySystemLocal = true;
    }

    if (hasOnlySystemLocal) {
        ui->labelStatus->setText (tr ("Select a model that'd be used for the base of adaption.<br/>"
                                      "<i>You only have system distributed models on your computer.</i>"
                                     ));
    }
    else if (hasOnlyUserLocal) {
        ui->labelStatus->setText (tr ("Select a model that'd be used for the base of adaption.<br/>"
                                      "<i>You only have models that you've created on your computer.</i>"
                                     ));
    }
}

void ModelSelection::cleanupPage()
{
    ui->comboBoxModel->clear();
}


bool ModelSelection::isComplete() const
{
    return m_model && m_model->isValid();
}

void ModelSelection::on_comboBoxModel_currentIndexChanged (const int index)
{
    QString path = ui->comboBoxModel->itemData (index).toString();
    m_model = new AcousticModel (path);
    const bool hasNoiseDictionary = m_model->noiseDictionary() != 0 && m_model->noiseDictionary()->isValid();
    ui->checkBoxNoiseDictionary->setChecked(hasNoiseDictionary);
}

ModelSelection::~ModelSelection()
{
    delete ui;
}

#include "modelselection.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
