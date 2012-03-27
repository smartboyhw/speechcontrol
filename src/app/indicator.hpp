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
#include <QVariantMap>
#include <QSystemTrayIcon>

#include "macros.hpp"

class QImage;

namespace SpeechControl
{
class Indicator;

/**
 * @brief Provides an abstract means of accessing the indicator system used in SpeechControl.
 *
 * In an attempt to ensure ABI support with any plug-in or future linkages of SpeechControl's code;
 * the Indicator singleton class is provided to permit the raising and using of indicators.
 **/
class Indicator : public QObject
{
    Q_OBJECT
    SC_SINGLETON (Indicator)

public:

    class Message
    {
    public:
        Message (QString const& p_keyName = QString::null);
        QString description() const;
        QString key() const;
        bool enabled() const;
        void setEnabled (bool const& p_isEnabled);

        static bool exists (QString const& p_keyName);
        static Message* create (QString const& p_keyName, QString const& p_keyDescription, bool const& p_isEnabled);

    private:
        QString m_key;
        static QVariantMap objectData(const QString& p_keyName);

    };

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
     * @param p_title The title of the message to present to the user.
     * @param p_message The message to present to the user.
     * @param p_timeout How long should be the message be displayed for.
     **/
    static void presentMessage (const QString& p_title, const QString& p_message = QString::null, const int& p_timeout = 3000, const Indicator::Message* p_messageIndicator = new Indicator::Message());

    /**
     * @brief Determines whether or not the Indicator's icon is visible.
     **/
    static QIcon icon();

    /**
     * @brief Determines if the Indicator is visible.
     **/
    static bool isVisible();

    /**
     * @brief Determines if the Indicator is enabled.
     **/
    static bool isEnabled();

private slots:
    void showMainWindow();
    void buildMenu();

private:
    QSystemTrayIcon* m_icon;            ///< The tray icon.
public slots:
    void on_mIcon_activated (QSystemTrayIcon::ActivationReason p_reason);
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
