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

#include <QDebug>

#include <app/core.hpp>
#include <app/indicator.hpp>
#include <app/sessions/session.hpp>
#include <app/desktopcontrol/agent.hpp>

#include "microphone-pane.hpp"
#include <abstractaudiosource.hpp>
#include "ui_settingspane-microphone.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

MicrophoneSettingsPane::MicrophoneSettingsPane () :
    ui (new Ui::MicrophoneSettingsPane)
{
    qDebug() << "[MicrophoneSettingsPane::{constructor}] Building microphone settings pane...";
    ui->setupUi (this);
    connect (ui->comboBoxDevices, SIGNAL (currentIndexChanged (QString)), this, SLOT (on_comboBoxDevices_currentIndexChanged (QString)));
    connect (ui->horizontalSliderVolume, SIGNAL (valueChanged (int)), this, SLOT (on_horizontialSliderVolume_valueChanged (int)));
    updateUi();
    qDebug() << "[MicrophoneSettingsPane::{constructor}] Built microphone settings pane.";
}

MicrophoneSettingsPane::~MicrophoneSettingsPane()
{
    delete ui;
}

void MicrophoneSettingsPane::changeEvent (QEvent* e)
{
    QWidget::changeEvent (e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}

QString MicrophoneSettingsPane::title() const
{
    return "Microphones";
}

QString MicrophoneSettingsPane::id() const
{
    return "mcrphns";
}

QPixmap MicrophoneSettingsPane::pixmap() const
{
    return QIcon::fromTheme ("audio-input-microphone").pixmap (32, 32);
}

void MicrophoneSettingsPane::restoreDefaults()
{
}

void MicrophoneSettingsPane::updateUi()
{
    ui->comboBoxDevices->clear();

    AbstractAudioSourceList devices = DeviceAudioSource::allDevices();
    QString defaultMic = Core::configuration ("Microphone/Default").toString();
    Q_FOREACH (const AbstractAudioSource * device, devices) {
        const DeviceAudioSource* mic = (DeviceAudioSource*) device;
        ui->comboBoxDevices->addItem (mic->deviceName());
        ui->comboBoxDevices->setItemIcon (ui->comboBoxDevices->findText (mic->deviceName()), QIcon::fromTheme ("audio-input-microphone"));
    }

    if (!defaultMic.isNull())
        ui->comboBoxDevices->setCurrentIndex (ui->comboBoxDevices->findText (defaultMic));

    ui->horizontalSliderVolume->setValue (devices.first()->volume());
}

void MicrophoneSettingsPane::on_comboBoxDevices_currentIndexChanged (const QString p_device)
{
    Core::setConfiguration ("Microphone/Default", p_device);
}

void MicrophoneSettingsPane::on_horizontialSliderVolume_valueChanged (const int p_value)
{
    QString curVal = ui->comboBoxDevices->currentText();
    DeviceAudioSource* mic = DeviceAudioSource::obtain(curVal);
    mic->setVolume ( (double) (p_value / 1000));
    delete mic;
}

void MicrophoneSettingsPane::on_checkBoxMute_toggled (const bool p_checked)
{
    QString curVal = ui->comboBoxDevices->currentText();
    DeviceAudioSource* mic = DeviceAudioSource::obtain(curVal);
    mic->setMuted(p_checked);
    delete mic;
}

#include "microphone-pane.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
