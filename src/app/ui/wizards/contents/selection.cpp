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

#include "selection.hpp"
#include "source-text.hpp"
#include "source-wiki.hpp"
#include "sessions/content.hpp"
#include "ui/contents-wizard.hpp"
#include "ui_contentwizard-sourceselect.h"

using namespace SpeechControl;
using SpeechControl::Windows::Wizards::Pages::SourceSelectionPage;

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
    Q_FOREACH (QWidget * l_src, s_lst) {
        m_ui->comboBoxSource->addItem (l_src->property ("title").toString(), l_src->property ("id"));
    }
}

void SourceSelectionPage::addDefaultSources()
{
    registerSourceWidget (new TextContentSourceWidget);
    registerSourceWidget (new WikiContentSourceWidget);
}

void SourceSelectionPage::on_comboBoxSource_currentIndexChanged (const int& p_index)
{
    QVariant l_vrnt = m_ui->comboBoxSource->itemData (p_index);

    Q_FOREACH (QWidget * page, s_lst.values()) {
        page->hide();
    }

    if (l_vrnt.isValid()) {
        QWidget* l_wid = s_lst.value (l_vrnt.toString());
        l_wid->setParent (m_ui->frameContainer);
        l_wid->show();
    }
}

bool SourceSelectionPage::validatePage()
{
    AbstractContentSource* src = ( (SpeechControl::Windows::Wizards::ContentWizard*) window())->source();

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
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
