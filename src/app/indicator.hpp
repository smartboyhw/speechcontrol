/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 12:50:47 PM
 */

#ifndef SPCHCNTRL_INDICATOR_HPP
#define SPCHCNTRL_INDICATOR_HPP

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "global.hpp"

class QAction;
class QImage;
class QSystemTrayIcon;

SPCHCNTRL_BEGIN_NAMESPACE

struct IndicatorPrivate;
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
    Q_DECLARE_PRIVATE(Indicator)
    SPCHCNTRL_SINGLETON (Indicator)

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

    static void addActionForPlugins (QAction* p_action);
    static void removeActionForPlugins (QAction* p_action);

private slots:
    void on_actionOptions_triggered();
    void on_actionAboutSpeechControl_triggered ();
    void on_actionStartTraining_triggered();
    void on_actionAdaptModels_triggered();

private:
    QScopedPointer<IndicatorPrivate> d_ptr;
};

SPCHCNTRL_END_NAMESPACE

#endif
// kate: indent-mode cstyle; replace-tabs on;
