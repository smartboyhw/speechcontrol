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

// local includes
#include "session.hpp"

namespace Ui {
class Training;
}

namespace SpeechControl {

namespace Windows {

/**
 * @brief ...
 **/
class Training : public QDialog {
    Q_OBJECT

public:
    explicit Training ( QWidget *parent = 0 );
    virtual ~Training();
    static void startTraining ( Session* );
    void setSession ( Session* );
    Session* session() const;

public slots:
    void stopCollecting();
    void startCollecting();
    virtual void open();

private slots:
    void updateProgress ( const double p_progress );
    void on_pushButtonClose_clicked();
    void on_pushButtonProgress_toggled ( const bool& );
    void on_pushButtonReset_clicked();
    void on_pushButtonUndo_clicked();
    void on_pushButtonNext_clicked();

private:
    /// @todo Use these functions to segment the phrase into parts that the user can read.
    /// @todo In order for this to work properly, we'd need to detect empty pauses in the user's speech. We'd might have to record a 'garbage' model of empty noises
    ///       and detect when empty noises are made and then advance.
    /// @todo We also have to return this information to the root sentence, how do we combine these phrases to the originating sentence?
    void navigateToPart ( const int& );
    void navigateNextPart();
    void navigatePreviousPart();
    void startNavigating();
    void stopNavigating();

    int m_curPos;
    int m_initPos;
    int m_posMin;
    int m_posMax;
    Ui::Training *m_ui;
    Session* m_session;
    Sentence* m_curSntct;
    Sentence* m_initSntct;
};

}
}

#endif // TRAINING_WINDOW_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
