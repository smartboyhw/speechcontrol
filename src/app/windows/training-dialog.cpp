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

#ifdef HAVE_KDE
#include <KMessageBox>
#define MESSAGEBOX KMessageBox
#else
#include <QMessageBox>
#define MESSAGEBOX QMessageBox
#endif

#include "ui_training-dialog.h"
#include "training-dialog.hpp"
#include "session.hpp"
#include <core.hpp>

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

TrainingDialog::TrainingDialog ( QWidget *parent ) :
    QDialog ( parent ),
    m_curPos ( 0 ), m_initPos ( 0 ), m_posMin ( 0 ), m_posMax ( 0 ),
    m_ui ( new Ui::Training ),
    m_mic ( Microphone::defaultMicrophone() ),
    m_session ( 0 ), m_curSntct ( 0 ), m_initSntct ( 0 ) {
    m_ui->setupUi ( this );
    m_ui->pushButtonProgress->setIcon ( QIcon::fromTheme ( ICON_RECORD ) );
    m_ui->pushButtonReset->setIcon ( QIcon::fromTheme ( ICON_CLEAR ) );
    m_ui->pushButtonUndo->setIcon ( QIcon::fromTheme ( ICON_UNDO ) );
    m_ui->pushButtonNext->setIcon ( QIcon::fromTheme ( ICON_NEXT ) );

    connect ( this,SLOT ( updateProgress ( double ) ),m_session,SIGNAL ( progressChanged ( double ) ) );
    connect ( m_mic,SIGNAL ( startedListening() ),this,SLOT ( startedListening() ) );
    connect ( m_mic,SIGNAL ( stoppedListening() ),this,SLOT ( stoppedListening() ) );
}

TrainingDialog::~TrainingDialog() {
    delete m_ui;
}

void TrainingDialog::startedListening() {
    m_ui->lblRecording->setPixmap(QIcon::fromTheme("audio-volume-high").pixmap(32,32));
}

void TrainingDialog::stoppedListening() {
    m_ui->lblRecording->setPixmap(QIcon::fromTheme("audio-volume-muted").pixmap(32,32));
}


void TrainingDialog::startTraining ( Session* p_session ) {
    if ( !p_session->isCompleted() ) {
        TrainingDialog* l_dialog = new TrainingDialog;
        l_dialog->setSession ( p_session );
        l_dialog->open();
    } else
        MESSAGEBOX::information ( Core::mainWindow() ,tr ( "Session Completed" ), tr ( "Session <b>%1</b> has been completed already." ).arg ( p_session->name() ) );
}

/// @todo Start training the session.
void TrainingDialog::startCollecting() {
    // Determine the last saved sentence in the session.
    m_initSntct = m_curSntct = m_session->firstIncompleteSentence();

    // Begin an iteration of reading sentences until interrupted or completed.
    if ( m_curSntct ) {
        int l_start = 0;
        const int l_max = m_curSntct->phrases().count();

        for ( ; l_start < l_max; l_start++ ) {
            if ( !m_curSntct->isPhraseCompleted ( l_start ) ) {
                m_initPos = l_start;
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

        navigateToPart ( l_start );
    } else {
        reject();
        hide();
        MESSAGEBOX::information ( this,tr ( "Session Completed" ),tr ( "No text is available for this session." ) );
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
    if ( !checked )
        stopCollecting();
    else
        startCollecting();
}

void TrainingDialog::updateProgress ( const double p_progress ) {
    const int l_progress = ( int ) ( p_progress * 100 );
    this->setWindowTitle ( tr ( "Training (%1%) - SpeechControl" ).arg ( l_progress ) );
    m_ui->groupBoxTitle->setTitle ( QString ( "%1 - %2%" ).arg ( m_session->name() ).arg ( QString::number ( l_progress ) ) );
    m_ui->progressBar->setValue ( l_progress );
}

void TrainingDialog::open() {
    m_ui->pushButtonProgress->setChecked ( true );
    QDialog::open();
}

/// @todo Use these functions to segment the phrase into parts that the user can read.
/// @todo In order for this to work properly, we'd need to detect empty pauses in the user's speech. We'd might have to record a 'garbage' model of empty noises
///       and detect when empty noises are made and then advance.
/// @todo We also have to return this information to the root sentence, how do we combine these phrases to the originating sentence?
void TrainingDialog::navigateToPart ( const uint &l_index ) {
    QString l_text;
    PhraseList l_phrsLst = m_curSntct->phrases();

    for ( uint i = 0; i < (uint) l_phrsLst.count(); i++ ) {
        const QString l_curWord = l_phrsLst.at ( i )->text();

        if ( l_index == i ) {
            Q_FOREACH ( const QChar l_chr, l_curWord ) {
                if ( l_chr.isLetterOrNumber() )
                    l_text += QString ( "<b>%1</b>" ).arg ( l_chr );
                else
                    l_text += l_chr;
            }
        } else
            l_text += QString ( "<font style='color: gray; font-size: small;'>%1</font>" ).arg ( l_curWord );

        if ( i != (uint) l_phrsLst.count() - 1 )
            l_text += " ";
    }

    m_curPos = l_index;
    m_ui->labelText->setText ( l_text );
    m_session->assessProgress();
    m_ui->pushButtonReset->setEnabled ( ! ( m_initPos == m_curPos && m_initSntct == m_curSntct ) );
    m_ui->pushButtonUndo->setEnabled ( m_ui->pushButtonReset->isEnabled() );
}

/// @todo When this goes over, advance to the next sentence.
void TrainingDialog::navigateNextPart() {
    if ( m_curPos + 1 < m_curSntct->phrases().count() ) {
        navigateToPart ( m_curPos + 1 );
    }
}

/// @todo When this hits -1, it should head back to the previous sentence.
void TrainingDialog::navigatePreviousPart() {
    if ( m_curPos - 1 >= 0 )
        navigateToPart ( m_curPos - 1 );
}

/// @todo This should clear all of the progress made since the start of training WHEN this dialog opened.
/// @todo Implement a means of tracking history.
void SpeechControl::Windows::TrainingDialog::on_pushButtonReset_clicked() {
    if ( QMessageBox::No == QMessageBox::question ( this, tr ( "Undo Current Progress" ) ,tr ( "Are you sure you want to reset your training progress back to the state it was initially?" ) ) )
        return;

    // Undo the work up to the initial point.
    for ( int i = m_curSntct->index(); i >= m_initSntct->index(); i-- ) {
        Sentence* l_sntc = m_session->corpus()->sentenceAt ( i );
        qDebug() << "Wiping sentence" << l_sntc->text();
        Q_FOREACH ( Phrase* l_phrs, l_sntc->phrases() ) {
            l_phrs->audio()->remove();
        }
    }

    // Now, revert and jump to the place that training when this dialog opened began at.
    m_curSntct = m_initSntct;
    m_curPos = m_initPos;
    navigateToPart(m_curPos);
}

/// @todo This should undo progress at a decrementing interval until it hits the point of where the dialog opened.
/// @todo Prevent going back further than what the history index recommends.
void SpeechControl::Windows::TrainingDialog::on_pushButtonUndo_clicked() {
    // Wipe out the previous part (and this part).
    m_curSntct->phrase ( m_curPos )->audio()->remove();
    uint l_pos = 0;

    if ( m_curPos == 0 ) {
        Sentence* l_prevSntct = m_session->corpus()->sentenceAt ( m_curSntct->index() - 1 );

        if ( l_prevSntct ) {
            l_pos = l_prevSntct->phrases().count() - 1;

            if ( l_prevSntct == m_initSntct && l_pos == m_initPos )
                return;

            m_curSntct = l_prevSntct;
        } else
            return;
    } else {
        m_curSntct->phrase ( m_curPos - 1 )->audio()->remove();
        qDebug() << "Wiping phrase" << m_curSntct->phrase ( m_curPos - 1 )->text();
        l_pos = m_curPos - 1;
    }

    // Rewind to that part.
    navigateToPart ( l_pos );
}

/// @todo This method here should handle the act of recording audio.
void SpeechControl::Windows::TrainingDialog::on_pushButtonNext_clicked() {
    if ( m_mic->isRecording() ) {
        m_mic->stopRecording();
        QFile* l_file = m_curSntct->phrase ( m_curPos )->audio();
        l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
        l_file->write ( m_mic->data() );
        l_file->close();
    }

    if ( m_curSntct->allPhrasesCompleted() ) {
        m_curSntct = m_session->firstIncompleteSentence();

        if ( m_curSntct == 0 ) {
            updateProgress ( 1.0 );
            QMessageBox* l_msgComplete = new QMessageBox ( this );
            l_msgComplete->setWindowTitle("Session Complete - SpeechControl");
            l_msgComplete->setText ( "You've successfully completed the recording part of this session!" );
            l_msgComplete->setInformativeText ( "With the session completed, you can now queue it for either adaption or generation of a language model." );
            l_msgComplete->setIcon ( QMessageBox::Information );
            l_msgComplete->setIconPixmap(QIcon::fromTheme("task-complete").pixmap(64,64));
            l_msgComplete->open();
            close();
            return;
        }

        m_curPos = -1;
    }

    // Advance to that part and start recording.
    navigateNextPart();
    m_mic->startRecording();
}

#include "training-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

