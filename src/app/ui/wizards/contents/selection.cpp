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
 * @date 05/20/12 14:31:01 PM
 */

#include "selection.hpp"
#include "source-text.hpp"
#include "sessions/content.hpp"
#include "ui/contents-wizard.hpp"
#include "ui_contentwizard-sourceselect.h"

SPCHCNTRL_USE_NAMESPACE
SPCHCNTRL_UI_WIZARD_PAGES_USE_NAMESPACE

QMap<QString, QWidget*> SourceSelectionPage::s_lst;

SourceSelectionPage::SourceSelectionPage (QWidget* parent) :
    QWizardPage (parent),
    m_ui (new Ui::SourceSelectionPage)
{
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);

    addDefaultSources();
    updateUi();
}

void SourceSelectionPage::updateUi()
{
    Q_FOREACH (QWidget * src, s_lst) {
        m_ui->comboBoxSource->addItem (src->property ("title").toString(), src->property ("id"));
    }
}

void SourceSelectionPage::addDefaultSources()
{
    registerSourceWidget (new TextContentSourceWidget);
}

void SourceSelectionPage::on_comboBoxSource_currentIndexChanged (const int& p_index)
{
    QVariant vrnt = m_ui->comboBoxSource->itemData (p_index);

    Q_FOREACH (QWidget * page, s_lst.values()) {
        page->hide();
    }

    if (vrnt.isValid()) {
        QWidget* wid = s_lst.value (vrnt.toString());
        wid->setParent (m_ui->frameContainer);
        wid->updateGeometry();
        wid->show();
    }
}

bool SourceSelectionPage::validatePage()
{
    AbstractContentSource* src = ( (SpeechControl::Windows::ContentWizard*) window())->source();

    if (!src)
        setSubTitle ("<b><font color='red'>Please select a source to generate content from.</font></b>");

    return ! (src == 0);
}

void SourceSelectionPage::registerSourceWidget (QWidget* p_widget)
{
    s_lst.insert (p_widget->property ("id").toString(), p_widget);
}

SourceSelectionPage::~SourceSelectionPage()
{
    delete m_ui;
}

#include "ui/wizards/contents/selection.moc"
// kate: indent-mode cstyle; replace-tabs on;
