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
// libspchcntrl includes
#include <lib/audiosource/device.hpp>

#include "micselect.hpp"
#include "ui/micsetup-wizard.hpp"
#include "ui_micwizard-select.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows::Wizards::Pages;
using SpeechControl::Windows::Wizards::Pages::MicrophoneSelection;

/// @todo The loudness of the content spoken should begin detection here.
MicrophoneSelection::MicrophoneSelection (QWidget* parent) :
    QWizardPage (parent), ui (new Ui::MicrophoneSelection),
    m_mic (0)
{
    ui->setupUi (this);
    this->setLayout (ui->verticalLayout);
    this->registerField ("mic", ui->comboBoxMicrophones, "currentIndex", SIGNAL (currentIndexChanged (QString)));
}

MicrophoneSelection::~MicrophoneSelection()
{
    delete ui;
}

/// @todo Fill the combo box with all of the mics.
void SpeechControl::Windows::Wizards::Pages::MicrophoneSelection::initializePage()
{
    AudioSourceList l_allMics = DeviceAudioSource::allDevices();

    if (!l_allMics.empty()) {
        Q_FOREACH (AbstractAudioSource * src, l_allMics) {
            DeviceAudioSource* deviceSrc = (DeviceAudioSource*) src;
            ui->comboBoxMicrophones->addItem (QIcon::fromTheme ("audio-input-microphone"), deviceSrc->humanName(), deviceSrc->deviceName());
        }
    }

    on_comboBoxMicrophones_activated (ui->comboBoxMicrophones->currentIndex());
}

bool SpeechControl::Windows::Wizards::Pages::MicrophoneSelection::validatePage()
{
    device()->stop();

    if (device()) {
        wizard()->setProperty ("mic-id", device()->deviceName());
    }

    return ui->progressBarFeedback->isEnabled();
}

void SpeechControl::Windows::Wizards::Pages::MicrophoneSelection::cleanupPage()
{
    device()->stop();
    ui->comboBoxMicrophones->clear();
    ui->progressBarFeedback->setValue (0);
    ui->progressBarFeedback->setFormat ("inactive");
}

bool SpeechControl::Windows::Wizards::Pages::MicrophoneSelection::isComplete()
{
    return device() != 0;
}

/// @todo Set the device to be detected for volume detection here.
/// @todo Set this page's value to this field.
void SpeechControl::Windows::Wizards::Pages::MicrophoneSelection::on_comboBoxMicrophones_activated (int index)
{
    if (device()) {
        device()->stop();
    }

    setDevice (0);

    const QString deviceName = ui->comboBoxMicrophones->itemData (index).toString();

    setDevice (DeviceAudioSource::obtain (deviceName));

    connect (device(), SIGNAL (bufferObtained (QByteArray)), this, SLOT (on_mic_bufferObtained (QByteArray)));
    device()->start();
}

void MicrophoneSelection::on_mic_bufferObtained (QByteArray p_buffer)
{
    quint16 max = pow (2, 16) - 1;
    quint16 val = p_buffer.toUInt();
    double progress = (double) val / (double) max;
    qDebug() << "[MicrophoneSelection::on_mic_bufferObtained()] Buffer value: "
             << p_buffer.toUInt();
    ui->progressBarFeedback->setValue (progress * 100);
}

DeviceAudioSource* MicrophoneSelection::device()
{
    return m_mic;
}

void MicrophoneSelection::setDevice (DeviceAudioSource* p_device)
{
    m_mic = p_device;

    if (p_device) {
        wizard()->setField ("mic", m_mic->deviceName());
    }
}

#include "ui/wizards/micsetup/micselect.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
