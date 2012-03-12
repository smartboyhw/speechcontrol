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
#include <QApplication>

#include "macros.hpp"

class QSettings;

namespace SpeechControl
{
namespace Windows
{
class Main;
}


/**
 * @brief Represents the entire heart of SpeechControl.
 *
 * The Core class is where most of the event handling for most utilities
 * lock into. It holds the @c Factory and all of the derived forms of @c AbstractPlugin
 * objects.
 **/
class Core : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Core)
    SC_SINGLETON (Core)

    friend class Windows::Main;

private:
    /// @note It this pointer needed? Every piece of code can just use QApplication::instance() to get the instance...
    QApplication* m_app;    /// Holds the Application instance.
    Windows::Main* m_mw;    /// Holds the main window.
    QSettings* m_settings;  /// Holds the application's global configuration.
    QTranslator* m_trnsltr; /// Holds the translating agent.

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
     * @return KMainWindow*
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
     * @brief ...
     *
     * @return QDir
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
};

}

#endif // CORE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
