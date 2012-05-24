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
 * @date 05/20/12 13:21:37 PM
 */

#ifndef ADAPT_MODELSELECTION_HPP
#define ADAPT_MODELSELECTION_HPP

#include <QWizardPage>
#include <ui/wizards/base.hpp>

class QComboBox;

SPCHCNTRL_DEFINE_UIC(ModelSelection)
SPCHCNTRL_FORWARD_DECLARE_CLASS(AcousticModel)
SPCHCNTRL_UI_WIZARD_PAGES_BEGIN_NAMESPACE

/**
 * @brief A QWizardPage that allows a user to select a base model for adaption.
 *
 * The ModelSelection wizard page, typically found in AdaptWizard, is used to
 * permit the user a choice in the base model for adaption.
 *
 * @todo Permit the user to overwrite the old model with the new data.
 * @todo Allow the user to name the new model if they'd like to.
 * @todo Allow the user to use another model's noise dictionary.
 * @todo Allow 'quick configurations options'.
 *
 * @section qco Quick Configuration Options
 *
 * Quick configuration options would allow users to mix and match components of
 * acoustic models to potentially sharpen the accuracy process. This allows for
 * specialized acoustic models that users could switch to for specific use cases.
 **/
class ModelSelection : public QWizardPage
{
    Q_OBJECT

public:
    explicit ModelSelection (QWidget* parent = 0);
    AcousticModel* model();
    ~ModelSelection();
    virtual void initializePage();
    virtual void cleanupPage();
    virtual bool isComplete() const;

private slots:
    void on_comboBoxModel_currentIndexChanged (const int index);

private:
    void fillModelsForComboBox (QComboBox* comboBoxModel);
    Ui::ModelSelection* ui;
    AcousticModel* m_model;
};

SPCHCNTRL_UI_WIZARD_PAGES_END_NAMESPACE

#endif // ADAPT_MODELSELECTION_HPP
// kate: indent-mode cstyle; replace-tabs on;
