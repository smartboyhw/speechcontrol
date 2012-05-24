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

#include <QFileDialog>
#include <QMessageBox>

#include <lib/audiosource/abstract.hpp>
#include <lib/audiosource/stream.hpp>
#include <lib/sphinx/audiosource.hpp>

#include "plugin.hpp"
#include "transcriber-dialog.hpp"
#include "ui_transcriber-dialog.h"

using namespace SpeechControl;
using namespace SpeechControl::Plugins::Transcriber;
using namespace SpeechControl::Windows;

TranscriberDialog::TranscriberDialog (QWidget* parent) :
    QDialog (parent),
    m_ui (new Ui::TranscriberDialog),
    m_streamSrc (0), m_audioSrcSphnx (0), m_strm (0)
{
    m_ui->setupUi (this);
}

/// @todo Fix this to have an approved list of audio files that can read by GStreamer.
void TranscriberDialog::on_btnOpen_clicked()
{
    const QString l_filePath = QFileDialog::getOpenFileName (this, tr ("Obtain Audio File for Transcribing"),
                               QDir::homePath());

    QMessageBox::information (this, l_filePath , l_filePath);
    m_ui->lineEditPath->setText (l_filePath);
}

void TranscriberDialog::on_btnTranscribe_clicked()
{
    QFile* file = new QFile (m_ui->lineEditPath->text());

    if (!file->exists()) {
        m_ui->lineEditPath->clear();
        qDebug() << "[TranscriberDialog::on_btnTranscribe_clicked()] File doesn't exist.";
        return;
    }

    if (!file->open (QIODevice::ReadOnly)) {
        qDebug() << "[TranscriberDialog::on_btnTranscribe_clicked()] File can't open.";
        return;
    }

    m_strm = new QDataStream (file);
    m_streamSrc = new StreamAudioSource (m_strm);
    m_audioSrcSphnx = new AudioSourceSphinx (m_streamSrc, this);

    connect (m_audioSrcSphnx, SIGNAL (finished (QString)), this, SLOT (outputValue (QString)));

    if (!m_audioSrcSphnx->start()) {
        qDebug() << "[TranscriberDialog::on_btnTranscribe_clicked()] Transcriber's Sphinx failed to start.";
    }
}

void TranscriberDialog::outputValue (const QString& p_value)
{
    if (!p_value.isEmpty())
        m_ui->textBrowserTranscription->setText (p_value);
    else
        m_ui->textBrowserTranscription->setText (tr ("Unable to transcribe any text in %1.").arg (m_ui->lineEditPath->text()));
}

TranscriberDialog::~TranscriberDialog()
{
    delete m_ui;
}

#include "transcriber-dialog.moc"
// kate: indent-mode cstyle; replace-tabs on; 
