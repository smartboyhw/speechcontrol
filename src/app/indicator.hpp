/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <QObject>
#include <QString>
#include <QSystemTrayIcon>

class QImage;

namespace SpeechControl {
class Indicator;

/**
 * @brief Provides an abstract means of accessing the indicator system used in SpeechControl.
 *
 * In an attempt to ensure ABI support with any plug-in or future linkages of SpeechControl's code;
 * the Indicator singleton class is provided to permit the raising and using of indicators.
 **/
class Indicator : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Destructor.
     **/
    virtual ~Indicator();

    /**
     * @brief Shows the visual representation of SpeechControl's indicator to the system.
     **/
    static void show();

    /**
     * @brief Hides the visual representation of SpeechControl's indicator from the system.
     **/
    static void hide();

    /**
     * @brief Raises a new message to the system.
     * @param p_message The message to present to the user.
     **/
    static void presentMessage ( const QString& p_title, const QString& p_message, const int& p_timeout );

    /**
     * @brief Obtains a pointer to the Indicator instance.
     * @return Indicator* The instance of the class.
     **/
    static Indicator* instance();

    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isVisible();

private slots:
    void showMainWindow();

private:
    explicit Indicator ( QObject* parent = 0 );
    QSystemTrayIcon* m_icon;            ///< The tray icon.
    static Indicator* s_inst;           ///< Singleton instance.
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
