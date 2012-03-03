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


#ifndef MAIN_HPP
#define MAIN_HPP

#include <QTimer>
#include <QObject>
#include <QProgressBar>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace SpeechControl {
/**
 * @brief A namespace containing all of the classes dedicated to the user interface.
 **/
namespace Windows {
/**
 * @brief Represents the Main window of SpeechControl.
 *
 * Being the first thing (and perhaps the last thing) that users see when
 * they start SpeechControl,
 **/
class Main : public QMainWindow {
    Q_OBJECT
public:
    explicit Main();
    ~Main();

public slots:
    virtual void show();
    virtual void close();
    void updateContent();
    void setProgress ( const double p_value );
    void setStatusMessage ( const QString& p_message, const int p_timeout = 8000 );

private slots:
    // desktop control
    void on_actionDesktopControlOptions_triggered();
    void on_actionDesktopControlActive_triggered ( bool p_checked );
    void on_btnDsktpCntrl_clicked ( const bool p_checked );
    void desktopControlStateChanged();

    // dictation
    void on_actionDictationActive_triggered ( const bool p_checked );
    void on_btnDctn_clicked ( const bool p_checked );
    void dictationStateChanged();

    // training
    void on_actionStartTraining_triggered();
    void on_actionAdaptModels_triggered();

    // misc.
    void on_tabWidget_currentChanged ( const int p_index );
    void on_actionOptions_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAboutSpeechControl_triggered();

private:
    Ui::MainWindow* m_ui;
    QProgressBar* m_prgStatusbar;
    QTimer* m_tckr;
};
}
}

#endif // MAIN_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
