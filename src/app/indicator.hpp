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

    /**
     * @brief Represents an indicator message.
     *
     * Indicator messages allow SpeechControl to determine if a message has been
     * used already and to prevent the "spamming" of notifications to the user.
     **/
    class Message
    {
    public:
        /**
         * @brief Constructor.
         *
         * @param p_keyName The key of the notification. Defaults to QString::null.
         **/
        explicit Message (QString const& p_keyName = QString::null);

        /**
         * @brief Obtains the description of the Message.
         **/
        QString description() const;

        /**
         * @brief Obtains the key of the Message.
         **/
        QString key() const;

        /**
         * @brief Determines if the Message's enabled.
         **/
        bool enabled() const;

        /**
         * @brief Sets the enabled state of the Message.
         *
         * @param p_isEnabled The new state of the Message.
         **/
        void setEnabled (bool const& p_isEnabled);

        /**
         * @brief Determines if the specific message exists.
         *
         * @param p_keyName The key of the Message.
         * @return TRUE if the Message exists, FALSE otherwise.
         **/
        static bool exists (QString const& p_keyName);

        /**
         * @brief Creates a new Message.
         *
         * @param p_keyName The key of the Message.
         * @param p_keyDescription The description of the Message.
         * @param p_isEnabled The enabled state of the Message.
         * @return A pointer to the newly created Message.
         **/
        static Message* create (QString const& p_keyName, QString const& p_keyDescription, bool const& p_isEnabled);

    private:
        QString m_key;
        static QVariantMap objectData (const QString& p_keyName);

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
    void on_mIcon_activated (QSystemTrayIcon::ActivationReason p_reason);
    void showMainWindow();
    void buildMenu();

private:
    QSystemTrayIcon* m_icon;            ///< The tray icon.
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
