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
 * @date 05/20/12 14:30:54 PM
 */

#ifndef CONTENTWIZARD_SOURCESELECTION_HPP
#define CONTENTWIZARD_SOURCESELECTION_HPP

#include <QMap>
#include <QWidget>
#include <QWizardPage>
#include <ui/wizards/base.hpp>

SPCHCNTRL_DEFINE_UIC(SourceSelectionPage)
SPCHCNTRL_FORWARD_DECLARE_CLASS(AbstractContentSource);
namespace SpeechControl { namespace Windows { namespace Pages {

/**
 * @brief ...
 **/
class SourceSelectionPage : public QWizardPage
{
    Q_OBJECT

public:
    /**
     * @brief ...
     *
     * @param parent ... Defaults to 0.
     **/
    explicit SourceSelectionPage (QWidget* parent = 0);
    /**
     * @brief ...
     *
     * @param p_dummy ... Defaults to 0.
     * @return void
     **/
    static void registerSourceWidget (QWidget* p_widget);
    /**
     * @brief ...
     *
     **/
    ~SourceSelectionPage();

    virtual bool validatePage();

private slots:
    void on_comboBoxSource_currentIndexChanged (const int& p_index);

private:
    void updateUi();
    void addDefaultSources();
    Ui::SourceSelectionPage* m_ui;
    AbstractContentSource* m_src;
    static QMap<QString, QWidget*> s_lst;
};

} } }
#endif // CONTENTWIZARD_SOURCESELECTION_HPP
// kate: indent-mode cstyle; replace-tabs on;
