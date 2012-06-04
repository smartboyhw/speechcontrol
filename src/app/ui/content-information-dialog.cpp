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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 15:05:31 PM
 */

#include "core.hpp"
#include <lib/sphinx/content.hpp>
#include "content-information-dialog.hpp"
#include "ui_content-information-dialog.h"

using SpeechControl::Core;
using SpeechControl::Windows::ContentInformationDialog;

ContentInformationDialog::ContentInformationDialog (Content* p_content) :
    QDialog (QApplication::activeModalWidget()),
    m_ui (new Ui::ContentInformationDialog),
    m_content (p_content)
{
    m_ui->setupUi (this);
    this->setLayout (m_ui->verticalLayout);
    updateUi();

    connect (m_ui->btnGoLeft, SIGNAL (clicked()), this, SLOT (goToPreviousPage()));
    connect (m_ui->btnGoRight, SIGNAL (clicked()), this, SLOT (goToNextPage()));
}

void ContentInformationDialog::updateUi()
{
    m_ui->lblTitle->setText (tr ("<p><span style=\"font-size:16pt;\">%1</span></p>"
                                 "<p>by <span style=\"font-weight:600;\">%2</span></p>")
                             .arg (m_content->title(), m_content->author()));
    goToPage (0);
}

void ContentInformationDialog::goToNextPage()
{
    goToPage (m_indx + 1);
}

void ContentInformationDialog::goToPreviousPage()
{
    goToPage (m_indx - 1);
}

void ContentInformationDialog::goToPage (const int p_index)
{
    if (p_index < 0 || (uint) p_index > m_content->pageCount()) {
        return;
    }

    m_indx = p_index;
    m_ui->txtEdit->setPlainText (m_content->pageAt (p_index).simplified().trimmed());
    m_ui->lblPageCounter->setText (tr ("Page %1 of %2").arg (p_index + 1).arg (m_content->pageCount()));

    const bool isAtLast = m_indx == m_content->pageCount() - 1;
    const bool isAtFirst  = m_indx == 0;

    m_ui->btnGoLeft->setEnabled (!isAtFirst);
    m_ui->btnGoRight->setEnabled (!isAtLast);
}

ContentInformationDialog::~ContentInformationDialog()
{
    delete m_ui;
}

#include "ui/content-information-dialog.moc"
// kate: indent-mode cstyle; replace-tabs on;
