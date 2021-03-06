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
#include <QDateTime>
#include <QProgressBar>
#include <QMessageBox>

#include <lib/audiosource/device.hpp>
#include <lib/audio/devicemanager.hpp>

#include "core.hpp"
#include "sessions/session.hpp"
#include "sessions/phrase.hpp"
#include "sessions/content.hpp"
#include "sessions/corpus.hpp"
#include "training-dialog.hpp"
#include "select-microphone-dialog.hpp"
#include "ui_training-dialog.h"

#define ICON_RECORD "media-record"
#define ICON_PAUSE "media-playback-pause"
#define ICON_STOP "media-playback-stop"
#define ICON_UNDO "edit-undo"
#define ICON_CLEAR "view-clear"
#define ICON_NEXT "go-next"

using namespace SpeechControl;
using namespace SpeechControl::Audio;
using SpeechControl::Windows::TrainingDialog;

TrainingDialog::TrainingDialog (QWidget* p_parent) :
    QDialog (p_parent),
    m_ui (new Ui::Training),
    m_mic (0),
    recorder(0),
    m_session (0),
    m_currentPosition (0), m_initialPosition (0),
    m_data()
{
    m_ui->setupUi (this);
    m_ui->pushButtonProgress->setIcon (QIcon::fromTheme (ICON_RECORD));
    m_ui->pushButtonReset->setIcon (QIcon::fromTheme (ICON_CLEAR));
    m_ui->pushButtonUndo->setIcon (QIcon::fromTheme (ICON_UNDO));
    m_ui->pushButtonNext->setIcon (QIcon::fromTheme (ICON_NEXT));

    recorder = new FileRecorder;

    /// @bug stopCollecting method seems to be used in redundant way (e.g. in the open() method).
    stopCollecting();
}

TrainingDialog::~TrainingDialog()
{
    delete m_ui;
}

DeviceAudioSource* TrainingDialog::deviceSource() const
{
    return m_mic;
}

void TrainingDialog::setDevice (DeviceAudioSource* p_device)
{
    m_mic = p_device;
    connect (m_mic, SIGNAL (begun()), this, SLOT (onMicStartedListening()));
    connect (m_mic, SIGNAL (ended()), this, SLOT (onMicStoppedListening()));
    connect (m_mic, SIGNAL (bufferObtained (QByteArray)), this, SLOT (on_mic_BufferObtained (QByteArray)));
}

/// @todo Write data to a stream representing the current phrase's audio.
void TrainingDialog::on_mic_BufferObtained (QByteArray p_buffer)
{
    m_data.append (p_buffer);
    qDebug() << "[TrainingDialog::on_mic_BufferObtained()] Added to internal buffer:" << p_buffer.toUInt();
}

void TrainingDialog::onMicStartedListening()
{
    m_ui->lblRecording->setPixmap (QIcon::fromTheme ("audio-volume-high").pixmap (32, 32));
    m_data.clear();
}

/// @todo [audio-recording] This is to be removed because new class will handle saving audio to files.
void TrainingDialog::onMicStoppedListening()
{
    if (session()) {
        m_ui->lblRecording->setPixmap (QIcon::fromTheme ("audio-volume-muted").pixmap (32, 32));
        QFile* file = currentPhrase()->audioFile();
        file->open (QIODevice::WriteOnly | QIODevice::Truncate);

        if (file->write (m_data) == -1) {
            qDebug() << "[TrainingDialog::onMicStoppedListening()] Error saving audio to disk: " << file->errorString();
        }

        file->close();
    }
}


void TrainingDialog::startTraining (Session* session, DeviceAudioSource* device)
{
    if (!device) {
        device = Windows::MicrophoneSelectionDialog::select();

        if (!device) {
            QMessageBox::critical (0, tr ("Microphone Error"), tr ("<h2>Microphone Error</h2>SpeechControl is unable to get a proper handle on your microphone. Please check that all required peripherals are connected."));
            return;
        }
    }

    if (session->isValid() && !session->isCompleted()) {
        TrainingDialog* dialog = new TrainingDialog;

        dialog->setSession (session);
        dialog->setDevice (device);
        DeviceManager::chooseDevice(device->deviceName());
        dialog->open();
    }
    else {
        /// @bug This is ambiguous due to the branching statement above (testing for two possible cases).
        QMessageBox::information (0 , tr ("Session Completed"), tr ("<h2>Session Completed</h2>Session <b>%1</b> has been completed already.").arg (session->name()));
    }
}

void TrainingDialog::startCollecting()
{
    if (!m_session->firstIncompletePhrase()) {
        reject();
        hide();
        QMessageBox::information (this,
                                  tr ("Session Completed"),
                                  tr ("No text is available for this session."));
    }
    else {
        m_initialPosition = m_currentPosition = m_session->firstIncompletePhrase()->index();
        m_ui->pushButtonNext->setEnabled (true);
        m_ui->pushButtonReset->setEnabled (false);
        m_ui->pushButtonProgress->setIcon (QIcon::fromTheme (ICON_PAUSE));
        m_ui->pushButtonProgress->setText (tr ("Pause"));
        m_ui->labelText->setEnabled (true);
        m_ui->labelText->setText (tr ("<i>Rendering...</i>"));
        qDebug() << "[TrainingDialog::startCollecting()] Cur: " << m_currentPosition << m_initialPosition;
        navigateToPart (m_currentPosition);
    }
}

void TrainingDialog::stopCollecting()
{
    if (recorder->isActive())
        recorder->stop();

    m_ui->pushButtonNext->setEnabled (false);
    m_ui->pushButtonUndo->setEnabled (false);
    m_ui->pushButtonReset->setEnabled (false);
    m_ui->labelText->setEnabled (false);
    m_ui->labelText->setText (tr ("<i>training paused</i>"));
    m_ui->pushButtonProgress->setIcon (QIcon::fromTheme (ICON_RECORD));
    m_ui->pushButtonProgress->setText (tr ("Resume"));
}

void TrainingDialog::setSession (Session* p_session)
{
    if (p_session) {
        m_session = p_session;
        this->setWindowTitle (tr ("Training '%1' (0%) - SpeechControl").arg (m_session->name()));
        updateProgress (m_session->assessProgress());
        connect (m_session, SIGNAL (progressChanged (double)), this, SLOT (updateProgress (double)));
    }
}

Session* TrainingDialog::session() const
{
    return m_session;
}

/// @todo Should we add a 'save progress' confirmation dialog here?
void TrainingDialog::on_pushButtonClose_clicked()
{
    stopCollecting();
    reject();
}

void TrainingDialog::on_pushButtonProgress_toggled (const bool& checked)
{
    m_ui->labelText->setEnabled (!checked);
    !checked ? stopCollecting() : startCollecting();
}

void TrainingDialog::updateProgress (const double p_progress)
{
    const int progress = (int) (p_progress * 100) + ( (0.5 / (double) m_session->corpus()->phrases().length()) * 100);
    this->setWindowTitle (tr ("Training '%2' (%1%) - SpeechControl").arg (progress).arg (m_session->content()->title()));
    m_ui->groupBoxTitle->setTitle (QString ("%1 - %2%").arg (m_session->name()).arg (QString::number (progress)));
    m_ui->progressBar->setValue (progress);
}

void TrainingDialog::open()
{
    stopCollecting();
    m_ui->pushButtonProgress->setText (tr ("Start"));
    m_ui->labelText->setText (tr ("<i>click <em>start</b> to begin training.</i>"));
    QDialog::open();
}

Phrase* TrainingDialog::currentPhrase()
{
    return m_session->corpus()->phraseAt (m_currentPosition);
}

Phrase* TrainingDialog::initialPhrase()
{
    return m_session->corpus()->phraseAt (m_initialPosition);
}

void TrainingDialog::navigateToPart (const uint& p_index)
{
    m_currentPosition = p_index;
    m_ui->labelText->setText (currentPhrase()->text());
    qDebug() << m_currentPosition << m_initialPosition;
    m_ui->pushButtonReset->setEnabled (! (currentPhrase() == initialPhrase()));
    m_ui->pushButtonUndo->setEnabled (m_ui->pushButtonReset->isEnabled());
    updateProgress (m_session->assessProgress());
    recorder->setFile(currentPhrase()->filePath());
    recorder->start();
}

void TrainingDialog::navigateNextPart()
{
    const Phrase* nextPhrase = m_session->firstIncompletePhrase();

    if (currentPhraseCompleted() && nextPhrase) {
        navigateToPart (nextPhrase->index());
    }
}

void TrainingDialog::navigatePreviousPart()
{
    const Phrase* prevPhrase = m_session->lastIncompletePhrase();

    if (currentPhraseCompleted() && prevPhrase) {
        navigateToPart (prevPhrase->index());
    }
}

bool TrainingDialog::currentPhraseCompleted()
{
    qDebug() << "[TrainingDialog::currentPhraseCompleted()] Is phrase completed? " << currentPhrase()->recorded();
    return currentPhrase()->recorded();
}

/// @todo This should clear all of the progress made since the start of training WHEN this dialog opened.
/// @todo Implement a means of tracking history.
void SpeechControl::Windows::TrainingDialog::on_pushButtonReset_clicked()
{
    if (QMessageBox::No == QMessageBox::question (this,
            tr ("Undo Current Progress"),
            tr ("Are you sure you want to reset your training progress back to the state it was initially?"))) {
        return;
    }

    // Wipe out the work up to the initial point.
    for (uint i = currentPhrase()->index(); i >= initialPhrase()->index(); i--) {
        Phrase* sntc = m_session->corpus()->phraseAt (i);
        qDebug() << "[TrainingDialog::on_pushButtonReset_clicked()] Wiping phrase" << sntc->text();
        Q_FOREACH (Phrase * phrs, m_session->corpus()->phrases()) {
            phrs->audioFile()->remove();
        }
    }

    // Now, revert and jump to the place that training when this dialog opened began at.
    m_currentPosition = m_initialPosition;
    navigateToPart (m_currentPosition);
}

/// @todo This should undo progress at a decrementing interval until it hits the point of where the dialog opened.
/// @todo Prevent going back further than what the history index recommends.
void SpeechControl::Windows::TrainingDialog::on_pushButtonUndo_clicked()
{
    int pos = 0;
    // Wipe out this audio.
    currentPhrase()->audioFile()->remove();

    // Rewind to that part.
    navigateToPart (pos);
}

/// @todo This method here should handle the act of recording audio.
void SpeechControl::Windows::TrainingDialog::on_pushButtonNext_clicked()
{
    qDebug() << "[TrainingDialog::onPushButtonNext_clicked()] Is recording? " << "BLAH BLAH";

    if (recorder->isActive())
        recorder->stop();

    if (!m_session->firstIncompletePhrase()) {
        updateProgress (1.0);
        QMessageBox* msgComplete = new QMessageBox (this);
        msgComplete->setWindowTitle (tr ("Session Complete - SpeechControl"));
        msgComplete->setText (tr ("You've successfully completed the recording part of this session!"));
        msgComplete->setInformativeText (tr ("With the session completed, you can now queue it for either adaption or generation of a language model."));
        msgComplete->setIcon (QMessageBox::Information);
        msgComplete->setIconPixmap (QIcon::fromTheme ("task-complete").pixmap (64, 64));
        msgComplete->open();
        close();
        return;
    }
    else {
        navigateNextPart();
    }
}

#include "ui/training-dialog.moc"
// kate: indent-mode cstyle; replace-tabs on; 
