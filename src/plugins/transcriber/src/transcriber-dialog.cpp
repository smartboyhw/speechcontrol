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

#include "sphinx.hpp"
#include "transcriber-dialog.hpp"
#include "ui_transcriber-dialog.h"

using namespace SpeechControl::Plugins::Transcriber;
using namespace SpeechControl::Windows;

TranscriberDialog::TranscriberDialog (QWidget* parent) :
    QDialog (parent),
    m_ui (new Ui::TranscriberDialog),
    m_sphnx (0)
{
    m_ui->setupUi (this);
    m_sphnx = new Sphinx;
    connect (m_sphnx, SIGNAL (finished (QString)), this, SLOT (outputValue (QString)));
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
    QFile* l_file = new QFile (m_ui->lineEditPath->text());

    if (!l_file->exists()) {
        m_ui->lineEditPath->clear();
        qDebug() << "File doesn't exist.";
        return;
    }

    while (m_sphnx->isRunning())
        m_sphnx->stop();

    m_sphnx->prepareForFile (l_file->fileName());

    if (m_sphnx->start()) {
        m_ui->textBrowserTranscription->setText (tr ("Transcribing..."));
        QString l_text, l_uttid;
        m_sphnx->formPartialResult (l_text, l_uttid);
        qDebug() << l_text << l_uttid;
    }
    else {
        m_ui->textBrowserTranscription->setText (tr ("Initialization of transcribing service failed."));
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
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
