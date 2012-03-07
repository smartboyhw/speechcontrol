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

#include "core.hpp"
#include "sessions/session.hpp"
#include "training-dialog.hpp"
#include "session-information-dialog.hpp"
#include "content-information-dialog.hpp"
#include "ui_session-information-dialog.h"

using SpeechControl::Core;
using SpeechControl::Windows::TrainingDialog;
using SpeechControl::Windows::SessionInformationDialog;
using SpeechControl::Windows::ContentInformationDialog;

SessionInformationDialog::SessionInformationDialog ( Session* p_session ) : QDialog ( 0 ),
    m_ui ( new Ui::SessionInformationDialog ), m_session ( p_session ) {
    m_ui->setupUi ( this );
    connect ( m_session,SIGNAL ( progressChanged ( double ) ),this,SLOT ( updateProgress ( double ) ) );
    updateUi();
}

void SessionInformationDialog::updateUi() {
    int l_sharedSessionCount = 0;

    Q_FOREACH ( const Session* l_session, Session::allSessions() ) {
        if ( l_session->content() == m_session->content() )
            l_sharedSessionCount += 0;
    }

    const int l_progress = ( int ) ( m_session->assessProgress() * 100.0 );
    m_ui->lblContentInfo->setText ( tr ( "This session uses the text from <b>%1</b>.<br />"
                                         "%2 other sessions use <b>%1</b> for transcription as well." )
                                    .arg ( m_session->name() )
                                    .arg ( l_sharedSessionCount )
                                  );

    if ( l_progress > 0 ) {
        m_ui->progressBarCompletion->setFormat ( tr ( "%p% complete" ) );
        m_ui->progressBarCompletion->setValue ( l_progress );
    } else {
        m_ui->progressBarCompletion->setFormat ( tr ( "no training progress" ) );
        m_ui->progressBarCompletion->setValue ( 0 );
    }

    m_ui->lblTitle->setText ( m_session->name() );
    m_ui->lineEditNickname->setText ( ( m_session->name().isEmpty() ) ? QString::null : m_session->name() );
}

void SessionInformationDialog::on_btnOpenContent_clicked() {
    ContentInformationDialog l_dialog ( m_session->content() );
    l_dialog.setParent ( this );
    l_dialog.show();
}

void SessionInformationDialog::on_btnTrainSession_clicked() {
    TrainingDialog::startTraining ( m_session );
}

void SessionInformationDialog::on_lineEditNickname_textChanged ( const QString& p_newNickname ) {
    if ( p_newNickname.isEmpty() || p_newNickname.isNull() ) {
        return;
    }

    m_session->setName ( p_newNickname );
    updateUi();
}

void SessionInformationDialog::updateProgress ( const double p_progress ) {
    m_ui->progressBarCompletion->setValue ( ( int ) p_progress * 100 );
}

SessionInformationDialog::~SessionInformationDialog() {
    delete m_ui;
}

#include "session-information-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
