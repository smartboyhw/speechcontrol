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


#include <QObject>
#include <QVariant>
#include <QApplication>

#include <dummysc.hpp>
#include <windows/main-window.hpp>

class QSettings;

namespace SpeechControl {

/*!
 * \brief Represents the entire heart of SpeechControl.
 *
 * The Core class is where most of the event handling for most utilities
 * lock into. It holds the @c Factory and all of the derived forms of @c AbstractPlugin
 * objects.
 **/
class Core : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Core )

    friend class Windows::Main;

signals:
    /**
     * @brief Emitted when SpeechControl has started.
     * @see start()
     **/
    void started();

    /**
     * @brief Emitted when SpeechControl has completed its shutdown process.
     * @see stop()
     **/
    void stopped();

public:
    /**
     * @brief Constructor.
     * @param p_argc The argument count from command-line.
     * @param p_argv The arguments passed from command-line.
     * @internal
     **/
    Core ( int, char** );

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
     * @param  p_attrDefVal The default value to return. Defaults to QVariant().
     * @return QVariant
     **/
    static QVariant configuration ( const QString&, QVariant = QVariant() );

    /**
     * @brief Sets a core configuration option of SpeechControl.
     *
     * This method allows for a centralized means of obtaining values about
     * SpeechControl's core configuration.
     * @param  p_attrName The path to the value to find.
     * @param  p_attrValue The value to be set.
     * @return void
     **/
    static void setConfiguration ( const QString&, const QVariant& );

    /**
     * @brief Obtains a pointer to the @c Core object, the singleton representing the application.
     * @return A pointer to the @c Core object.
     **/
    static Core* instance();

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
     * @brief Quits the application's main execution loop.
     * @param p_exitCode The exit code for the application to use.
     */
    void quit ( const int& = 0 );

public slots:
    /**
     * @brief Starts SpeechControl's main loop.
     * @return void
     **/
    void start();

    /**
     * @brief Stops SpeechControl's main loop.
     * @return void
     **/
    void stop();

    /// Experimental
    void asrFinished ( QString& text );

private:
    QApplication* m_app;          /// Holds the Application instance.
    Windows::Main* s_mw;    /// Holds the main window.
    QSettings* m_settings;  /// Holds the application's global configuration.
    static Core* s_inst;    /// Holds a instance.

    /// Experimental
    DummySC* dummyASR;

};

}

#endif // CORE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
