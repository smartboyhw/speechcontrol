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

#include "ui_training-dialog.h"
#include "core.hpp"
#include "sessions/session.hpp"
#include "training-dialog.hpp"

#include <phrase.hpp>
#include <sentence.hpp>
#include <corpus.hpp>

#define ICON_RECORD "media-record"
#define ICON_PAUSE "media-playback-pause"
#define ICON_STOP "media-playback-stop"
#define ICON_UNDO "edit-undo"
#define ICON_CLEAR "view-clear"
#define ICON_NEXT "go-next"

using namespace SpeechControl;
using SpeechControl::Windows::TrainingDialog;

TrainingDialog::TrainingDialog ( QWidget* p_parent ) :
    QDialog ( p_parent ),
    m_currentPosition ( 0 ), m_initialPosition ( 0 ),
    m_ui ( new Ui::Training ),
    m_mic ( Microphone::defaultMicrophone() ),
    m_session ( 0 ), m_currentSentence ( 0 ), m_initialSentence ( 0 ) {
    m_ui->setupUi ( this );
    m_ui->pushButtonProgress->setIcon ( QIcon::fromTheme ( ICON_RECORD ) );
    m_ui->pushButtonReset->setIcon ( QIcon::fromTheme ( ICON_CLEAR ) );
    m_ui->pushButtonUndo->setIcon ( QIcon::fromTheme ( ICON_UNDO ) );
    m_ui->pushButtonNext->setIcon ( QIcon::fromTheme ( ICON_NEXT ) );

    connect ( this,SLOT ( updateProgress ( double ) ),m_session,SIGNAL ( progressChanged ( double ) ) );
    connect ( m_mic,SIGNAL ( startedListening() ),this,SLOT ( on_micStartedListening() ) );
    connect ( m_mic,SIGNAL ( stoppedListening() ),this,SLOT ( on_micStoppedListening() ) );
    on_micStoppedListening();
}

TrainingDialog::~TrainingDialog() {
    delete m_ui;
}

void TrainingDialog::on_micStartedListening() {
    m_ui->lblRecording->setPixmap ( QIcon::fromTheme ( "audio-volume-high" ).pixmap ( 32,32 ) );
}

void TrainingDialog::on_micStoppedListening() {
    m_ui->lblRecording->setPixmap ( QIcon::fromTheme ( "audio-volume-muted" ).pixmap ( 32,32 ) );
}


void TrainingDialog::startTraining ( Session* p_session ) {
    if ( !p_session->isCompleted() ) {
        TrainingDialog* l_dialog = new TrainingDialog;
        l_dialog->setSession ( p_session );
        l_dialog->open();
    } else {
        QMessageBox::information ( 0 ,tr ( "Session Completed" ), tr ( "Session <b>%1</b> has been completed already." ).arg ( p_session->name() ) );
    }
}

/// @bug When training reaches the end of the sentence, it's unable to proceed to the next sentence when training is invoked from here. A manual (yet sloppy) fix is to click 'Undo' until you reach the beginning of the sentence and re-record everything.
void TrainingDialog::startCollecting() {
    m_initialSentence = m_currentSentence = m_session->firstIncompleteSentence();

    // Begin an iteration of reading sentences until interrupted or completed.
    if ( m_currentSentence ) {
        int l_start = 0;
        const int l_max = m_currentSentence->phrases().count();

        for ( ; l_start < l_max; l_start++ ) {
            if ( !m_currentSentence->isPhraseCompleted ( l_start ) ) {
                m_initialPosition = l_start;
                break;
            }
        }

        // Configure the button.
        m_ui->pushButtonNext->setEnabled ( true );
        m_ui->pushButtonReset->setEnabled ( false );
        m_ui->pushButtonProgress->setIcon ( QIcon::fromTheme ( ICON_PAUSE ) );
        m_ui->pushButtonProgress->setText ( tr ( "Pause" ) );
        m_ui->labelText->setEnabled ( true );
        m_ui->labelText->setText ( tr ( "<i>Rendering...</i>" ) );

        navigateToPart ( l_start , m_currentSentence );
    } else {
        reject();
        hide();
        QMessageBox::information ( this,tr ( "Session Completed" ),tr ( "No text is available for this session." ) );
    }
}

void TrainingDialog::stopCollecting() {
    m_ui->pushButtonNext->setEnabled ( false );
    m_ui->pushButtonUndo->setEnabled ( false );
    m_ui->pushButtonReset->setEnabled ( false );
    m_ui->labelText->setEnabled ( false );
    m_ui->labelText->setText ( tr ( "<i>training paused</i>" ) );
    m_ui->pushButtonProgress->setIcon ( QIcon::fromTheme ( ICON_RECORD ) );
    m_ui->pushButtonProgress->setText ( tr ( "Start" ) );
}

/// @todo Connect the changing of progress of the session to this window.
void TrainingDialog::setSession ( Session *p_session ) {
    m_session = p_session;
    this->setWindowTitle ( tr ( "Training (0%) - SpeechControl" ).arg ( m_session->name() ) );
    updateProgress ( 0.0 );
    connect ( m_session,SIGNAL ( progressChanged ( double ) ),this,SLOT ( updateProgress ( double ) ) );
}

Session* TrainingDialog::session() const {
    return m_session;
}

void TrainingDialog::on_pushButtonClose_clicked() {
    reject();
}

void TrainingDialog::on_pushButtonProgress_toggled ( const bool& checked ) {
    m_ui->labelText->setEnabled ( !checked );
    if ( !checked ) {
        stopCollecting();
    } else {
        startCollecting();
    }
}

void TrainingDialog::updateProgress ( const double p_progress ) {
    const int l_progress = ( int ) ( p_progress * 100 );
    this->setWindowTitle ( tr ( "Training (%1%) - SpeechControl" ).arg ( l_progress ) );
    m_ui->groupBoxTitle->setTitle ( QString ( "%1 - %2%" ).arg ( m_session->name() ).arg ( QString::number ( l_progress ) ) );
    m_ui->progressBar->setValue ( l_progress );
}

void TrainingDialog::open() {
    QDialog::open();
    m_ui->pushButtonProgress->setChecked ( true );
}

/// @todo In order for this to work properly, we'd need to detect empty pauses in the user's speech. We'd might have to record a 'garbage' model of empty noises and detect when empty noises are made and then advance.
void TrainingDialog::navigateToPart ( const uint &p_index, Sentence* p_sentence ) {
    QString l_text;
    p_sentence = ( p_sentence == 0 ) ? m_currentSentence : p_sentence;
    PhraseList l_phrsLst = p_sentence->phrases();

    for ( uint i = 0; i < ( uint ) l_phrsLst.count(); i++ ) {
        const QString l_curWord = l_phrsLst.at ( i )->text();

        if ( p_index == i ) {
            Q_FOREACH ( const QChar l_chr, l_curWord ) {
                if ( l_chr.isLetterOrNumber() ) {
                    l_text += QString ( "<b>%1</b>" ).arg ( l_chr );
                } else {
                    l_text += l_chr;
                }
            }
        } else {
            l_text += QString ( "<font style='color: gray; font-size: small;'>%1</font>" ).arg ( l_curWord );
        }

        if ( i != ( uint ) l_phrsLst.count() - 1 ) {
            l_text += " ";
        }
    }

    qDebug() << l_text << p_sentence->phrases();

    m_currentPosition = p_index;
    m_currentSentence = p_sentence;
    m_ui->labelText->setText ( l_text );
    m_session->assessProgress();
    m_ui->pushButtonReset->setEnabled ( ! ( ( int ) m_initialPosition == m_currentPosition && m_initialSentence == m_currentSentence ) );
    m_ui->pushButtonUndo->setEnabled ( m_ui->pushButtonReset->isEnabled() );
}

void TrainingDialog::navigateNextPart() {
    if ( m_currentPosition + 1 < m_currentSentence->phrases().count() ) {
        navigateToPart ( m_currentPosition + 1 );
    } else {
        navigateToPart ( m_currentSentence->phrases().count() - 1 );
    }
}

/// @todo When this hits -1, it should head back to the previous sentence.
void TrainingDialog::navigatePreviousPart() {
    if ( m_currentPosition - 1 >= 0 ) {
        navigateToPart ( m_currentPosition - 1 );
    } else {
        navigateToPart ( 0 );
    }
}

/// @todo This should clear all of the progress made since the start of training WHEN this dialog opened.
/// @todo Implement a means of tracking history.
void SpeechControl::Windows::TrainingDialog::on_pushButtonReset_clicked() {
    if ( QMessageBox::No == QMessageBox::question ( this, tr ( "Undo Current Progress" ) ,tr ( "Are you sure you want to reset your training progress back to the state it was initially?" ) ) ) {
        return;
    }

    // Undo the work up to the initial point.
    for ( int i = m_currentSentence->index(); i >= m_initialSentence->index(); i-- ) {
        Sentence* l_sntc = m_session->corpus()->sentenceAt ( i );
        qDebug() << "Wiping sentence" << l_sntc->text();
        Q_FOREACH ( Phrase* l_phrs, l_sntc->phrases() ) {
            l_phrs->audio()->remove();
        }
    }

    // Now, revert and jump to the place that training when this dialog opened began at.
    m_currentSentence = m_initialSentence;
    m_currentPosition = m_initialPosition;
    navigateToPart ( m_currentPosition );
}

/// @todo This should undo progress at a decrementing interval until it hits the point of where the dialog opened.
/// @todo Prevent going back further than what the history index recommends.
void SpeechControl::Windows::TrainingDialog::on_pushButtonUndo_clicked() {
    // Wipe out the previous part (and this part).
    m_currentSentence->phrase ( m_currentPosition )->audio()->remove();
    uint l_pos = 0;

    if ( m_currentPosition == 0 ) {
        Sentence* l_prevSntct = m_session->corpus()->sentenceAt ( m_currentSentence->index() - 1 );

        if ( l_prevSntct ) {
            l_pos = l_prevSntct->phrases().count() - 1;

            if ( l_prevSntct == m_initialSentence && l_pos == m_initialPosition ) {
                return;
            }

            m_currentSentence = l_prevSntct;
        } else {
            return;
        }
    } else {
        m_currentSentence->phrase ( m_currentPosition - 1 )->audio()->remove();
        qDebug() << "Wiping phrase" << m_currentSentence->phrase ( m_currentPosition - 1 )->text();
        l_pos = m_currentPosition - 1;
    }

    // Rewind to that part.
    navigateToPart ( l_pos );
}

/// @todo This method here should handle the act of recording audio.
void SpeechControl::Windows::TrainingDialog::on_pushButtonNext_clicked() {
    if ( m_mic->isRecording() ) {
        m_mic->stopRecording();
        QFile* l_file = m_currentSentence->phrase ( m_currentPosition )->audio();
        l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
        l_file->write ( m_mic->data() );
        l_file->close();
    }

    if ( m_currentSentence->allPhrasesCompleted() ) {
        m_currentSentence = m_session->firstIncompleteSentence();

        if ( m_currentSentence == 0 ) {
            updateProgress ( 1.0 );
            QMessageBox* l_msgComplete = new QMessageBox ( this );
            l_msgComplete->setWindowTitle ( "Session Complete - SpeechControl" );
            l_msgComplete->setText ( "You've successfully completed the recording part of this session!" );
            l_msgComplete->setInformativeText ( "With the session completed, you can now queue it for either adaption or generation of a language model." );
            l_msgComplete->setIcon ( QMessageBox::Information );
            l_msgComplete->setIconPixmap ( QIcon::fromTheme ( "task-complete" ).pixmap ( 64,64 ) );
            l_msgComplete->open();
            close();
            return;
        }

        m_currentPosition = -1;
    }

    // Advance to that part and start recording.
    navigateNextPart();
    m_mic->startRecording();
}

#include "training-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;