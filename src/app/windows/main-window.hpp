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
    class User;
    /**
     * @brief ...
     **/
    namespace Windows {
        /**
         * @brief ...
         **/
        class Main : public QMainWindow {
            Q_OBJECT
        public:
            explicit Main();
            ~Main();

        private slots:
            void on_actionOptionsDesktopControl_triggered();
            void on_actionOptions_triggered();
            void on_actionStartTraining_triggered();
            void on_actionAdaptModels_triggered();
            void on_actionStartDesktopControl_triggered();
            void on_actionStartDictation_triggered();
            void on_btnDsktpCntrl_checked (bool checked);
            void on_btnDctn_checked (bool checked);
            void on_actionAboutQt_triggered();
            void on_actionAboutSpeechControl_triggered();
            void desktopControlToggled(const bool);
            void dictationToggled(const bool);
            void updateContent();
            void setProgress(const double);

        private:
            Ui::MainWindow* m_ui;
            QProgressBar* m_prgStatusbar;
            QTimer* m_tckr;
        };
    }
}

#endif // MAIN_HPP
