 /**
  * This file is part of SpeechControl
  *
  * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
  *
  * SpeechControl is free software; you can redistribute it and/or modify
  * it under the terms of the GNU Library General Public License as
  * published by the Free Software Foundation; either version 2 of the
  * License, or (at your option) any later version.
  *
  * SpeechControl is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public
  * License along with SpeechControl; if not, write to the
  * Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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
            void on_btnDsktpCntrl_toggled(bool checked);
            void on_btnDctn_toggled(bool checked);
            void desktopControlToggled(const bool);
            void dictationToggled(const bool);
            void on_btnDsktpCntrl_clicked();
            void on_btnDctn_clicked();
            void updateContent();

        private:
            Ui::MainWindow* m_ui;
            QProgressBar* m_prgTraining;
            QTimer* m_tckr;
        };
    }
}

#endif // MAIN_HPP
