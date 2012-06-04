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
 * @date 05/20/12 14:26:19 PM
 */

#include "source-text.hpp"
#include "ui/contents-wizard.hpp"
#include <lib/sphinx/content.hpp>
#include "ui_contentwizard-source-text.h"

#include <QDebug>
#include <QTextStream>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

TextContentSourceWidget::TextContentSourceWidget (QWidget* parent) :
    QWidget (parent),
    m_ui (new Ui::TextContentSourceWidget)
{
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);
    setProperty ("id", "txt");
    setProperty ("title", "Text-Based");
}

void TextContentSourceWidget::on_btnOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName (this, "Select SpeechControl Content",
                     QDir::homePath());

    if (!QFile::exists (path)) {
        QMessageBox::critical (this, tr ("Invalid Content Source"), tr ("The content source specified is invalid.\nPlease select another."));
        return;
    }
    else {
        m_ui->lineEditPath->setText (path);
        updateView();
    }
}

void TextContentSourceWidget::updateView()
{
    TextContentSource* src = new TextContentSource;
    src->setUrl (QUrl::fromUserInput (m_ui->lineEditPath->text()));

    if (src->isValid()) {
        m_ui->lblInfo->setText (tr ("<h2>%1</h2>by <b>%2</b>").arg (src->title()).arg (src->author()));
        ( (SpeechControl::Windows::ContentWizard*) this->window())->setSource (src);
    }
    else {
        m_ui->lblInfo->setText (tr ("<h2>Invalid Content</h2>"));
        ( (SpeechControl::Windows::ContentWizard*) this->window())->setSource (0);
    }
}

TextContentSourceWidget::~TextContentSourceWidget()
{
    delete m_ui;
}

#include "ui/wizards/contents/source-text.moc"
// kate: indent-mode cstyle; replace-tabs on;
