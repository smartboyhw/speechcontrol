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

#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/AudioOutput>

#include "core.hpp"
#include "sessions/session.hpp"
#include "sessions/corpus.hpp"
#include "sessions/phrase.hpp"
#include "training-dialog.hpp"
#include "ui_session-information-dialog.h"
#include "content-information-dialog.hpp"

#include "session-information-dialog.hpp"

using SpeechControl::Core;
using SpeechControl::Windows::TrainingDialog;
using SpeechControl::Windows::SessionInformationDialog;
using SpeechControl::Windows::ContentInformationDialog;

SessionInformationDialog::SessionInformationDialog (Session* p_session) : QDialog (0),
    m_ui (new Ui::SessionInformationDialog), m_session (p_session)
{
    m_ui->setupUi (this);
    connect (m_session, SIGNAL (progressChanged (double)), this, SLOT (updateProgress (double)));
    updateUi();
}

void SessionInformationDialog::updateUi()
{
    int sharedSessionCount = 0;

    Q_FOREACH (const Session * session, Session::allSessions()) {
        if (session->content() == m_session->content() && session->id() != m_session->id())
            sharedSessionCount += 1;
    }

    const int progress = (int) (m_session->assessProgress() * 100.0);
    m_ui->lblContentInfo->setText (tr ("Shared with %1 other session(s).")
                                   .arg (sharedSessionCount)
                                  );

    if (progress > 0) {
        m_ui->progressBarCompletion->setFormat (tr ("%p% complete"));
        m_ui->progressBarCompletion->setValue (progress);
    }
    else {
        m_ui->progressBarCompletion->setFormat (tr ("no training progress"));
        m_ui->progressBarCompletion->setValue (0);
    }

    m_ui->lblTitle->setText (m_session->name());
    m_ui->lineEditNickname->setText ( (m_session->name().isEmpty()) ? QString::null : m_session->name());
    m_ui->lblPhraseText->clear();
    m_ui->horizontalSliderPhrase->setRange (0, m_session->corpus()->phrases().count() - 1);
    m_ui->horizontalSliderPhrase->setValue (0);
}

void SessionInformationDialog::on_btnOpenContent_clicked()
{
    ContentInformationDialog dialog (m_session->content());
    dialog.setParent (this);
    dialog.exec();
}

void SessionInformationDialog::on_btnTrainSession_clicked()
{
    TrainingDialog::startTraining (m_session);
}

void SessionInformationDialog::on_lineEditNickname_textChanged (const QString& p_newNickname)
{
    if (p_newNickname.isEmpty() || p_newNickname.isNull()) {
        return;
    }

    m_session->setName (p_newNickname);
    updateUi();
}

void SessionInformationDialog::updateProgress (const double p_progress)
{
    m_ui->progressBarCompletion->setValue ( (int) p_progress * 100);
}

void SessionInformationDialog::on_btnPhrasePlay_clicked()
{
    Phrase* phrase = m_session->corpus()->phraseAt (m_ui->horizontalSliderPhrase->value());
    Phonon::MediaObject* media = new Phonon::MediaObject(this);
    Phonon::createPath(media,(new Phonon::AudioOutput(Phonon::NoCategory,this)));
    media->setCurrentSource(QUrl(phrase->audio()->fileName()));
    media->play();
}

void SessionInformationDialog::on_horizontalSliderPhrase_valueChanged (const int p_value)
{
    Phrase* phrase = m_session->corpus()->phraseAt (p_value);
    m_ui->btnPlayPhrase->setEnabled (phrase->isCompleted());
    m_ui->lblPhraseText->setText (phrase->text());
}

SessionInformationDialog::~SessionInformationDialog()
{
    delete m_ui;
}

#include "session-information-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
