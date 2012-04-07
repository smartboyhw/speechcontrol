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

#ifndef CORE_HPP
#define CORE_HPP

#include <QDir>
#include <QObject>
#include <QVariant>

#include "macros.hpp"

class QSettings;
class QApplication;
class QTranslator;

namespace SpeechControl
{

namespace Windows
{
class Main;
}

class CorePrivate;
/**
 * @brief Represents the entire heart of SpeechControl.
 *
 * The Core class is where most of the event handling for most utilities
 * lock into. It holds the @c Factory and all of the derived forms of @c AbstractPlugin
 * objects.
 **/
class Core : public QObject
{
    friend class Windows::Main;

    Q_OBJECT
    Q_DISABLE_COPY (Core)
    Q_DECLARE_PRIVATE (Core)
    SC_SINGLETON (Core)

public:
    /**
     * @brief Constructor.
     * @param p_argc The argument count from command-line.
     * @param p_argv The arguments passed from command-line.
     * @param p_app The QApplication instance.
     * @internal
     **/
    Core (int p_argc, char** p_argv, QApplication* app);

    /**
     * @brief Destructor.
     * @internal
     **/
    virtual ~Core();

    /**
     * @brief Obtains a core configuration option of SpeechControl.
     * This method allows for a centralized means of obtaining values about
     * SpeechControl's core configuration.
     * @param  p_attrName The path to the value to find.
     * @param  p_attrDefValue The default value to return. Defaults to QVariant().
     * @return QVariant
     **/
    static QVariant configuration (const QString& p_attrName, QVariant p_attrDefValue = QVariant());

    /**
     * @brief Sets a core configuration option of SpeechControl.
     *
     * This method allows for a centralized means of obtaining values about
     * SpeechControl's core configuration.
     * @param  p_attrName The path to the value to find.
     * @param  p_attrValue The value to be set.
     *
     **/
    static void setConfiguration (const QString& p_attrName, const QVariant& p_attrValue);

    /**
     * @brief Provides a pointer to the Main Window instance.
     **/
    static Windows::Main* mainWindow();

    /**
     * @brief Invoke the application's main execution loop.
     */
    static int exec();

    /**
     * @brief Loads the specified locale p_locale into SpeechControl for translation.
     *
     * @param p_locale The locale to load.
     **/
    static void loadTranslations (const QLocale& p_locale);

    /**
     * @brief Sets whether or not SpeechControl will invoke itself on user log-in.
     *
     * @param p_toggle The new state of the auto-start.
     **/
    static void setAutoStart (const bool p_toggle);

    /**
     * @brief Determines whether or not SpeechControl starts on user log-in.
     *
     **/
    static bool doesAutoStart();

    /**
     * @brief Obtains the base QDir to the configuration data of SpeechControl.
     **/
    static QDir configurationPath();

    /**
     * @brief Quits the application's main execution loop.
     * @param p_exitCode The exit code for the application to use.
     */
    void quit (const int& p_exitCode = 0);

signals:
    /**
     * @brief Emitted when SpeechControl has started.
     * @return void
     **/
    void started();

    /**
     * @brief Emitted when SpeechControl has completed its shutdown process.
     * @return void
     **/
    void stopped();

public slots:
    /**
     * @brief Starts SpeechControl's main loop.
     *
     **/
    void start();

    /**
     * @brief Stops SpeechControl's main loop.
     *
     **/
    void stop();

private slots:
    void invokeAutoStart();
    void hookUpSignals();

private:
    QScopedPointer<CorePrivate> d_ptr;
};

}

#endif // CORE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
