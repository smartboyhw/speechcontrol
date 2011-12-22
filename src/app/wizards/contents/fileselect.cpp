/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 Jacky Alcine <jacky.alcine@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "fileselect.hpp"
#include "ui_cw_fileselectionpage.h"

#include <QFileDialog>

using SpeechControl::Wizards::Pages::FileSelectionPage;

FileSelectionPage::FileSelectionPage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::FileSelectionPage)
{
    m_ui->setupUi(this);
}

FileSelectionPage::~FileSelectionPage()
{
    delete m_ui;
}

void SpeechControl::Wizards::Pages::FileSelectionPage::on_toolButton_clicked()
{
    const QString l_filePath = QFileDialog::getOpenFileName(this, tr("Open Text File"), QDir::homePath(), tr("Text Files (*.txt)"));

    if (!l_filePath.isNull()){
        QFile l_file(l_filePath);
        m_ui->plainTextEdit->setPlainText(l_file.readAll());
        m_ui->lineEdit->setText(l_filePath);
    }
}