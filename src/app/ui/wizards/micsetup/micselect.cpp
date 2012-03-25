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

// libspchcntrl includes
#include <lib/abstractaudiosource.hpp>

#include "micselect.hpp"
#include "app/ui/micsetup-wizard.hpp"
#include "ui_micwizard-select.h"

using namespace SpeechControl;
using namespace SpeechControl::Wizards::Pages;
using SpeechControl::Wizards::Pages::MicrophoneSelection;

/// @todo The loudness of the content spoken should begin detection here.
MicrophoneSelection::MicrophoneSelection (QWidget* parent) :
    QWizardPage (parent), ui (new Ui::MicrophoneSelection),
    m_mic (DeviceAudioSource::defaultDevice())
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
void SpeechControl::Wizards::Pages::MicrophoneSelection::initializePage()
{
    AbstractAudioSourceList l_allMics = DeviceAudioSource::allDevices();

    if (!l_allMics.empty()) {
        Q_FOREACH (AbstractAudioSource * src, l_allMics) {
            DeviceAudioSource* deviceSrc = (DeviceAudioSource*) src;
            ui->comboBoxMicrophones->addItem (QIcon::fromTheme ("audio-input-microphone"), deviceSrc->humanName(), deviceSrc->deviceName());
        }
    }

    on_comboBoxMicrophones_activated(ui->comboBoxMicrophones->currentIndex());
}

bool SpeechControl::Wizards::Pages::MicrophoneSelection::validatePage()
{
    if (m_mic) {
        wizard()->setProperty ("mic-id", m_mic->deviceName());
    }

    return ui->progressBarFeedback->isEnabled();
}

void SpeechControl::Wizards::Pages::MicrophoneSelection::cleanupPage()
{
    m_mic->stop();
    ui->comboBoxMicrophones->clear();
    ui->progressBarFeedback->setValue (0);
    ui->progressBarFeedback->setFormat ("inactive");
}

bool SpeechControl::Wizards::Pages::MicrophoneSelection::isComplete()
{
    return m_complete;
}

/// @todo Set the device to be detected for volume detection here.
/// @todo Set this page's value to this field.
void SpeechControl::Wizards::Pages::MicrophoneSelection::on_comboBoxMicrophones_activated (int index)
{
    if (m_mic) {
        m_mic->stop();
    }

    m_mic = 0;

    const QString deviceName = ui->comboBoxMicrophones->itemData (index).toString();

    m_mic = DeviceAudioSource::obtain (deviceName);

    m_mic->start();
    connect (m_mic, SIGNAL (bufferObtained (QByteArray)), this, SLOT (on_mic_bufferObtained (QByteArray)));
}

void MicrophoneSelection::on_mic_bufferObtained (QByteArray p_buffer)
{
    quint16 max = pow (2, 16) - 1;
    quint16 val = p_buffer.at (0);
    double progress = (double) val / (double) max;
    ui->progressBarFeedback->setValue (progress * 100);
}

#include "micselect.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
