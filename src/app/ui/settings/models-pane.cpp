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

#include "models-pane.hpp"
#include "ui_settingspane-model.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

AcousticModelSettingsPane::AcousticModelSettingsPane() :
    m_ui (new Ui::AcousticModelSettingsPane)
{
    qDebug() << "[AcousticModelSettingsPane::{constructor}] Building acoustic model settings pane...";
    m_ui->setupUi (this);
    updateUi();
    qDebug() << "[AcousticModelSettingsPane::{constructor}] Built acoustic model settings pane.";
}

AcousticModelSettingsPane::~AcousticModelSettingsPane()
{
    delete m_ui;
}

void AcousticModelSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString AcousticModelSettingsPane::title() const
{
    return "Acoustic Models";
}

QString AcousticModelSettingsPane::id() const
{
    return "cstc-mdls";
}

QPixmap AcousticModelSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void AcousticModelSettingsPane::restoreDefaults()
{
}

void AcousticModelSettingsPane::updateUi()
{
}

#include "models-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
