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
 * @date 05/22/12 23:25:00 PM
 */

#include <QDebug>

#include <lib/audiosource/device.hpp>

#include "core.hpp"
#include "microphone-pane.hpp"
#include "ui_settingspane-microphone.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

MicrophoneSettingsPane::MicrophoneSettingsPane () :
    ui (new Ui::MicrophoneSettingsPane)
{
    qDebug() << "[MicrophoneSettingsPane::{constructor}] Building microphone settings pane...";
    ui->setupUi (this);
    connect (ui->comboBoxDevices, SIGNAL (currentIndexChanged (int)), this, SLOT (on_comboBoxDevices_currentIndexChanged (int)));
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

    AudioSourceList devices = DeviceAudioSource::allDevices();
    qDebug() << "[MicrophoneSettingsPane::updateUi()]" << Core::configuration ("Microphone/Default");
    Q_FOREACH (const AbstractAudioSource * device, devices) {
        const DeviceAudioSource* mic = (DeviceAudioSource*) device;
        ui->comboBoxDevices->addItem (mic->humanName());
        const int index = ui->comboBoxDevices->findText (mic->humanName());
        qDebug() << "[MicrophoneSettingsPane::updateUi()] Building #" << index;
        ui->comboBoxDevices->setItemIcon (index, QIcon::fromTheme ("audio-input-microphone"));
        ui->comboBoxDevices->setItemData (index, mic->deviceName());
    }

    QString defaultMic = Core::configuration ("Microphone/Default").toString();
    qDebug() << defaultMic.isNull() << defaultMic.isEmpty() << Core::configuration ("Microphone/Default").isNull() << Core::configuration ("Microphone/Default").isValid();

    if (!defaultMic.isNull() && !defaultMic.isEmpty())
        ui->comboBoxDevices->setCurrentIndex (ui->comboBoxDevices->findData (DeviceAudioSource::obtain (defaultMic)->deviceName()));
    else
        ui->comboBoxDevices->setCurrentIndex (0);
}

void MicrophoneSettingsPane::on_comboBoxDevices_currentIndexChanged (const int p_index)
{
    const QString selectedDevice = ui->comboBoxDevices->itemData (p_index).toString();
    qDebug() << "MicrophoneSettingsPane::on_comboBoxDevices_currentIndexChanged()] Selected device: " << selectedDevice;
    Core::setConfiguration ("Microphone/Default", selectedDevice);
    QString curVal = ui->comboBoxDevices->itemData (p_index).toString();
    DeviceAudioSource* mic = DeviceAudioSource::obtain (curVal);
    ui->horizontalSliderVolume->setValue (mic->volume() * 100);
    ui->checkBoxMute->setChecked (mic->isMuted());
    on_checkBoxMute_toggled (mic->isMuted());
}

void MicrophoneSettingsPane::on_horizontialSliderVolume_valueChanged (const int p_value)
{
    QString curVal = ui->comboBoxDevices->currentText();
    DeviceAudioSource* mic = DeviceAudioSource::obtain (curVal);
    mic->setVolume ( (double) (p_value / 100.0));
}

void MicrophoneSettingsPane::on_checkBoxMute_toggled (const bool p_checked)
{
    QString curVal = ui->comboBoxDevices->currentText();
    DeviceAudioSource* mic = DeviceAudioSource::obtain (curVal);
    mic->setMuted (p_checked);
    ui->horizontalSliderVolume->setEnabled (!p_checked);
    ui->horizontalSliderVolume->setValue ( (p_checked ? 0 : mic->volume() * 100));
}

#include "ui/settings/microphone-pane.moc"
// kate: indent-mode cstyle; replace-tabs on;
