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

#include "app/ui/contents-wizard.hpp"
#include "app/sessions/content.hpp"
#include "app/sessions/wikicontentsource.hpp"
#include "ui_contentwizard-source-wiki.h"
#include "source-wiki.hpp"

#include <QDebug>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

using SpeechControl::Content;
using SpeechControl::WikiContentSource;
using SpeechControl::WikiContentSourceWidget;

WikiContentSourceWidget::WikiContentSourceWidget (QWidget* parent) :
    QWidget (parent),
    m_ui (new Ui::WikiContentSourceWidget)
{
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);
    setProperty ("id", "wiki");
    setProperty ("title", "Wikipedia");
}

void WikiContentSourceWidget::updateView()
{
    WikiContentSource* src = new WikiContentSource;
}

WikiContentSourceWidget::~WikiContentSourceWidget()
{
    delete m_ui;
}

#include "ui/source-wiki.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
