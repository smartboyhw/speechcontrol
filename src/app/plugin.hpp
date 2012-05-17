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

#ifndef SPCHCNTRL_PLUGINS_HPP
#define SPCHCNTRL_PLUGINS_HPP

#include <QUrl>
#include <QList>
#include <QObject>
#include <QStringList>

#include <lib/export.hpp>
#include <app/global.hpp>

class QPixmap;
class QAction;
class QSettings;
class QPluginLoader;

SPCHCNTRL_BEGIN_NAMESPACE

class AbstractPlugin;
struct AbstractPluginPrivate;
/**
 * @brief Represents a list of plug-ins.
 **/
typedef QList<AbstractPlugin*> PluginList;

/**
 * @brief Represents a mapping of IDs and plug-ins.
 **/
typedef QMap<QString, AbstractPlugin*> PluginMap;

/**
 * @brief An abstract base for plug-ins to define their entry class.
 *
 * This class provides a basis for all plug-ins to begin their executional
 * cycle as well as to terminate it. It comes with convenience methods for
 * most of the values you'd find  in the plug-in's configuration file like it's
 * name (@c name()), version (@c version()) and dependencies (@c plugins()). Such
 * values make the Factory's execution more simpler, but on the developer's end; they
 * can easily read + write configuration using QSettings by obtaining the plug-in's
 * method of nabbing its settings (@c settings()).
 *
 * @see Factory
 **/
class SPCH_EXPORT AbstractPlugin : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (AbstractPlugin)
    Q_DECLARE_PRIVATE(AbstractPlugin)
    QScopedPointer<AbstractPluginPrivate> d_ptr;
    friend class Factory;
    friend class GenericPlugin;

signals:
    /**
     * @brief Emitted when the plug-in has been started.
     **/
    void started();

    /**
     * @brief Emitted when the plug-in has been stopped.
     **/
    void stopped();

public:
    /**
     * @brief Default constructor.
     * @param p_parent The parent of this QObject.
     **/
    explicit AbstractPlugin (QObject* p_parent = 0);

    /**
     * @brief Initializing constructor.
     *
     * Builds an AbstractPlugin and loads the configuration.
     *
     * @param p_id The ID of the plug-in to build.
     * @param p_parent The parent of this QObject.
     **/
    AbstractPlugin (const QString& p_id, QObject* p_parent = 0);

    /**
     * @brief Destructor.
     **/
    virtual ~AbstractPlugin();

    /**
     * @brief Determines if the plug-in has been loaded.
     * @return TRUE, if the plug-in has been loaded. FALSE otherwise.
     **/
    bool hasLoaded() const;

    /**
     * @brief Obtains the version of the plug-in.
     * @return A positive value if the version's found, else -1.0 if not.
     **/
    double version() const;

    /**
     * @brief Obtains the name of the plug-in.
     * @return QString::null if it couldn't be determined, a QString otherwise.
     **/
    const QString name() const;

    /**
     * @brief Obtains the description of the plug-in.
     * @return QString::null if it couldn't be determined, a QString otherwise.
     **/
    const QString description() const;

    /**
     * @brief ...
     *
     * @return const QString
     **/
    const QString author() const;

    /**
     * @brief Obtains a URL to a page that provides more information about the plug-in.
     * @return An invalid QUrl if it couldn't be determined, a QUrl otherwise.
     **/
    const QUrl url() const;

    /**
     * @brief Obtains the id of the plug-in.
     **/
    const QString id() const;

    virtual QPixmap pixmap() const = 0;

    /**
     * @brief Obtains a list of dependency plug-ins.
     * @return A PluginList of plug-ins that have to be loaded in order for this plug-in to be used correctly.
     **/
    const PluginList plugins() const;

    /**
     * @brief Determines if the plug-in can be loaded at the moment.
     * @return TRUE if the plug-in is supported, FALSE otherwise.
     **/
    bool isSupported() const;

    /**
     * @brief Determines if the plug-in has been enabled.
     * @return TRUE if the plug-in is enabled, FALSE otherwise.
     **/
    bool isEnabled() const;

    /**
     * @brief Determines if the plug-in has been loaded.
     * @return TRUE if the plug-in is loaded, FALSE otherwise.
     **/
    bool isLoaded() const;

public slots:

    /**
     * @brief Loads the required components of the plug-in to SpeechControl.
     * @return TRUE if the load was successful, FALSE otherwise.
     **/
    bool load();

protected:

    /**
     * @brief This method is invoked when the plug-in has been successfully loaded.
     * @internal
     **/
    virtual void initialize() = 0;

    /**
     * @brief This method is invoked when the plug-in is about to be unloaded.
     * @internal
     **/
    virtual void deinitialize() = 0;

    /**
     * @brief Loads the internal parts of the plug-in like dependencies and the library.
     * @internal
     * @return TRUE if the loading was successful, FALSE otherwise.
     **/
    bool loadComponents();

    /**
     * @brief Obtains the QSettings object that represents the settings (custom) for the plug-in.
     *
     * @return QSettings* A pointer to the QSettings object.
     **/
    QSettings* settings() const;

    /**
     * @brief Obtains the QSettings* object that represents the configuration (non-custom) of the plug-in.
     *
     * @return QSettings* A pointer to the QSettings object.
     **/
    QSettings* configuration() const;

    /**
     * @brief Obtains the list of QActions used by this plug-in.
     *
     * @return A QList< QAction* > of actions. It'll be empty if no QActions have been added.
     **/
    QList<QAction*> actions() const;

    /**
     * @brief Adds a QAction for this plug-in to the Main window.
     *
     * @param p_action The QAction to be added.
     **/
    void addAction (QAction* p_action);

    /**
     * @brief Adds a list of QActions for the plug-in to the Main window.
     *
     * @param p_actions A list of QActions to be added.
     **/
    void addActions (QList<QAction*> p_actions);

private slots:

    /**
     * @brief Starts the plug-in's internal workings.
     * @internal
     **/
    void start();

    /**
     * @brief Stops the plug-in's activity and readies it for unloading.
     * @internal
     **/
    void stop();
};

/**
 * @brief Represents a generic, empty plug-in.
 *
 * Generic plug-ins are used to load information about a specific plug-in without
 * having to load the library for it into memory.
 **/
class GenericPlugin : public AbstractPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY (GenericPlugin)

public:
    /**
     * @brief Constructor.
     *
     * @param p_id The ID of the plug-in to load.
     **/
    GenericPlugin (const QString& p_id);

    /**
     * @brief Obtains the pixmap of a Generic plugin.
     *
     * @return The SpeechControl's  application icon.
     **/
    QPixmap pixmap() const;

protected:
    virtual inline void initialize() { };
    virtual inline void deinitialize() { };
};

SPCHCNTRL_END_NAMESPACE

#endif // SPCHCNTRL_PLUGINS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
