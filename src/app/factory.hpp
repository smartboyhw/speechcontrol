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
 * @date 05/20/12 12:26:56 PM
 */

#ifndef SPCHCNTRL_FACTORY_HPP
#define SPCHCNTRL_FACTORY_HPP

#include <QObject>
#include <app/global.hpp>

namespace SpeechControl {

class Factory;
class AbstractPlugin;
struct FactoryPrivate;

/**
 * @brief Represents a manipulative system to control plug-ins.
 *
 * The Factory of SpeechControl allows the system to load and unload plug-ins at whim.
 * Configuration and settings about plug-ins can also be obtained via convenience methods
 * provided by the Factory.
 *
 * @section configuration-settings Settings and Configuration
 *
 * The term 'configuration' and 'setting' are synonyms, but here in SpeechControl, we use the
 * terms to define two separate things.
 *
 * @subsection configuration Plug-in Configuration
 * <b>Configuration</b> refers to the settings used to define the plug-in so that SpeechControl can determine its dependencies and manage the (un)loading process.
 *
 * @subsection settings Plug-in Settings
 * <b>Settings</b> refer to the options and values that the plug-in will use within SpeechControl to extend its functionality.
 *
 **/
class Factory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Factory)
    Q_DECLARE_PRIVATE(Factory)
    SPCHCNTRL_SINGLETON (Factory)
    QScopedPointer<FactoryPrivate> d_ptr;

signals:
    /**
     * @brief Emitted when the Factory's started.
     **/
    void started();

    /**
     * @brief Emitted when the Factory's stopped.
     **/
    void stopped();

    /**
     * @brief Emitted when a plug-in is loaded.
     * @param p_uuid The UUID of the plug-in loaded.
     **/
    void pluginLoaded (const QString& p_id);

    /**
     * @brief Emitted when a plug-in is unloaded.
     * @param p_uuid The UUID of the plug-in unloaded.
     **/
    void pluginUnloaded (const QString& p_id);

public slots:
    /**
     * @brief Starts the Factory.
     * @see started()
     **/
    void start();

    /**
     * @brief Stops the Factory.
     * @see stopped()
     **/
    void stop();

public:
    /**
     * @brief Destructor.
     **/
    virtual ~Factory();

    /**
     * @brief Loads a plug-in.
     *
     * Invokes the loading process for a plug-in and readies it
     * for integration with SpeechControl.
     *
     * @param p_uuid The UUID of the plug-in to load.
     * @return bool True if the plug-in loaded successfully, false otherwise.
     **/
    static bool loadPlugin (const QString& p_id);

    /**
     * @brief Unloads a plug-in.
     *
     * Invokes the unloading process for a plug-in and readies it
     * for removal from SpeechControl.
     *
     * @param p_uuid The UUID of the plug-in to unload.
     * @return bool True if the plug-in unloaded successfully, false otherwise.
     **/
    static void unloadPlugin (const QString& p_id);

    /**
     * @brief Determines if a plug-in has been loaded.
     * @param p_uuid The plug-in's UUID to verify.
     * @return bool True if the plug-in is loaded, false otherwise.
     **/
    static bool isPluginLoaded (const QString& p_id);

    /**
     * @brief Obtains a pointer to a loaded plugin.
     *
     * @param p_uuid The UUID of the plugin.
     * @return NULL if the plug-in isn't loaded. A pointer to the plug-in otherwise.
     **/
    static AbstractPlugin* plugin (const QString& p_id);

    /**
     * @brief Obtains a list of loaded plug-ins.
     * @return A PluginList. length() == 0 if there's no loaded plug-ins.
     **/
    static QList<AbstractPlugin*> loadedPlugins();

    /**
     * @brief Obtains a mapping of all of the plug-ins that can be loaded by SpeechControl.
     * @return A PluginMap of all of the discovered plug-ins for SpeechControl on the system.
     **/
    static QMap<QString, AbstractPlugin*> availablePlugins();

    /**
     * @brief Obtains the configuration of a plug-in by its specified UUID.
     * @param p_uuid The UUID of the plug-in whose configuration is requested.
     * @return A pointer to a QSettings* object. NULL if the UUID points to an invalid plug-in.
     * @see pluginSettings()
     **/
    static QSettings* pluginConfiguration (const QString& p_id);

    /**
     * @brief Obtains the settings of a plug-in by its specified UUID.
     * @param p_uuid The UUID of the plug-in whose settings is requested.
     * @return A pointer to a QSettings* object. NULL if the UUID points to an invalid plug-in.
     * @see pluginConfiguration()
     **/
    static QSettings* pluginSettings (const QString& p_id);

    /**
     * @brief Defines a specific plug-in to be loaded on start.
     *
     * Sets the plugin by its specific ID, p_id, to be loaded on initialization of
     * SpeechControl if p_state == TRUE.
     *
     * @param p_id The ID of the Plugin.
     * @param p_state The state of the plug-in's start-up load state.
     **/
    static void setLoadOnStart (const QString& p_id, const bool p_state);

    /**
     * @brief Determines if a specific plug-in loads on start.
     *
     * @param p_id The plug-in in question.
     * @return TRUE if the plug-in loads on start, FALSE otherwise.
     **/
    static bool doesLoadOnStart (const QString& p_id);

    /**
     * @brief Obtains a QStringList of the plug-ins loaded on start.
     **/
    static QStringList autoStart();
};

}

#endif // FACTORY_HPP
// kate: indent-mode cstyle; replace-tabs on;
