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

#ifndef CONTENTWIZARD_SOURCESELECTION_HPP
#define CONTENTWIZARD_SOURCESELECTION_HPP

#include <QMap>
#include <QWidget>
#include <QWizardPage>

namespace Ui
{
class SourceSelectionPage;
}

namespace SpeechControl
{
class AbstractContentSource;
namespace Windows
{
namespace Wizards
{
namespace Pages
{

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

}
}
}
}
#endif // CONTENTWIZARD_SOURCESELECTION_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
