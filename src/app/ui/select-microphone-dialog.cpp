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

#include <QIcon>

#include <lib/abstractaudiosource.hpp>

#include "app/core.hpp"
#include "ui_select-microphone-dialog.h"

#include "select-microphone-dialog.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

MicrophoneSelectionDialog::MicrophoneSelectionDialog (QWidget* parent) :
    QDialog (parent), ui (new Ui::MicrophoneSelectionDialog), m_mic (0)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    updateUi();
}

void MicrophoneSelectionDialog::updateUi()
{
    AbstractAudioSourceList devices = DeviceAudioSource::allDevices();
    ui->comboBoxDevices->clear();

    Q_FOREACH (AbstractAudioSource * abstractDevice, devices) {
        DeviceAudioSource* device = (DeviceAudioSource*) abstractDevice;
        ui->comboBoxDevices->addItem (QIcon::fromTheme ("audio-input-microphone"), device->humanName(), device->deviceName());
    }

    const QString defaultDevice = Core::configuration ("Microphone/Default").toString();

    if (!defaultDevice.isNull() && !defaultDevice.isEmpty()) {
        ui->comboBoxDevices->setCurrentIndex (ui->comboBoxDevices->findData (defaultDevice));
    }

    m_mic = DeviceAudioSource::obtain (defaultDevice);
}

void MicrophoneSelectionDialog::on_comboBoxDevices_currentIndexChanged (const int index)
{
    const QString itemDevice = ui->comboBoxDevices->itemData (index).toString();
    const QString defaultDevice = Core::configuration ("Microphone/Default").toString();

    if (!defaultDevice.isEmpty() && !defaultDevice.isNull()) {
        ui->checkBoxUseDefault->setChecked (itemDevice == defaultDevice);
        m_mic = DeviceAudioSource::obtain (defaultDevice);
    }

    qDebug() << "[MicrophoneSelectionDialog::on_comboBoxDevices_currentIndexChanged()]" << itemDevice << defaultDevice;
}

void MicrophoneSelectionDialog::on_checkBoxUseDefault_toggled (const bool checked)
{
    const QString itemDevice = ui->comboBoxDevices->itemData (ui->comboBoxDevices->currentIndex()).toString();

    if (checked)
        Core::setConfiguration ("Microphone/Default", itemDevice);
    else
        Core::setConfiguration ("Microphone/Default", QVariant());
}

DeviceAudioSource* MicrophoneSelectionDialog::select()
{
    if (Core::configuration ("Microphone/UseDefault").toBool())
        return DeviceAudioSource::obtain (Core::configuration ("Microphone/Default").toString());
    else {
        MicrophoneSelectionDialog dialog;

        if (dialog.exec() == QDialog::Accepted)
            return dialog.m_mic;
    }

    return 0;
}

void MicrophoneSelectionDialog::accept()
{
    this->QDialog::accept();
}

MicrophoneSelectionDialog::~MicrophoneSelectionDialog()
{

}

#include "select-microphone-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
