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
 * @date 05/22/12 23:20:05 PM
 */

#include "training-pane.hpp"
#include "session-pane.hpp"
#include "content-pane.hpp"
#include "microphone-pane.hpp"
#include "models-pane.hpp"
#include "ui_settingspane-training.h"

SPCHCNTRL_UI_USE_NAMESPACE

TrainingSettingsPane::TrainingSettingsPane() :
    ui (new Ui::TrainingSettingsPane)
{
    qDebug() << "[TrainingSettingsPane::{constructor}] Building training settings pane...";
    ui->setupUi (this);
    addPane (new SessionSettingsPane);
    addPane (new ContentSettingsPane);
    addPane (new ModelSettingsPane);
    //addPane (new LanguageModelSettingsPane);
    //addPane (new DictionarySettingsPane);
    addPane (new MicrophoneSettingsPane);
    updateUi();
    qDebug() << "[TrainingSettingsPane::{constructor}] Built training settings pane.";
}

TrainingSettingsPane::~TrainingSettingsPane()
{
    delete ui;
}

void TrainingSettingsPane::changeEvent (QEvent* e)
{
    QFrame::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString SpeechControl::Windows::TrainingSettingsPane::title() const
{
    return "Training";
}

QString SpeechControl::Windows::TrainingSettingsPane::id() const
{
    return "trnng";
}

QPixmap TrainingSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("configure").pixmap (32, 32);
}

void TrainingSettingsPane::restoreDefaults()
{

}

void TrainingSettingsPane::updateUi()
{

}

#include "ui/settings/training-pane.moc"
// kate: indent-mode cstyle; replace-tabs on;
