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


#ifndef TRAINING_WINDOW_HPP
#define TRAINING_WINDOW_HPP

// Qt includes
#include <QDialog>

// libspchcntrl includes
#include <sentence.hpp>
#include <microphone.hpp>

// local includes
#include <sessions/session.hpp>

namespace Ui {
class Training;
}

namespace SpeechControl {

namespace Windows {

/**
 * @brief ...
 **/
class TrainingDialog : public QDialog {
    Q_OBJECT

public:
    explicit TrainingDialog ( QWidget *parent = 0 );
    virtual ~TrainingDialog();
    static void startTraining ( Session* );
    void setSession ( Session* );
    Session* session() const;

public slots:
    void stopCollecting();
    void startCollecting();
    virtual void open();

private slots:
    void updateProgress ( const double p_progress );
    void stoppedListening();
    void startedListening();
    void on_pushButtonClose_clicked();
    void on_pushButtonProgress_toggled ( const bool& );
    void on_pushButtonReset_clicked();
    void on_pushButtonUndo_clicked();
    void on_pushButtonNext_clicked();

private:
    void navigateToPart ( const uint& l_index );
    void navigateNextPart();
    void navigatePreviousPart();

    int m_curPos;
    uint m_initPos;
    uint m_initPosPhrs;
    uint m_posMin;
    uint m_posMax;
    Ui::Training *m_ui;
    Microphone* m_mic;
    Session* m_session;
    Sentence* m_curSntct;
    Sentence* m_initSntct;
};

}
}

#endif // TRAINING_WINDOW_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
