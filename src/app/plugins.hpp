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

#ifndef PLUGINS_HPP
#define PLUGINS_HPP

#define SPCHCNTRL_PLUGINS_DIR "@SPCHCNTRL_PLUGINS_DIR@"
#define SPCHCNTRL_PLUGINS_CONFIG_DIR "@SPCHCNTRL_PLUGINS_CONFIG_DIR@"

#include <QUrl>
#include <QUuid>
#include <QList>
#include <QObject>
#include <QStringList>

class QPluginLoader;
class QSettings;

namespace SpeechControl {
namespace Plugins {
class Factory;
class AbstractHandle;
class AbstractPlugin;

/**
 * @brief Represents a list of plug-ins.
 **/
typedef QList<AbstractPlugin*> PluginList;

/**
 * @brief Represents a mapping of UUIDs and plug-ins.
 **/
typedef QMap<QUuid, AbstractPlugin*> PluginMap;

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
class AbstractPlugin : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( AbstractPlugin )
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
    explicit AbstractPlugin ( QObject* p_parent = 0 );

    /**
     * @brief Initializing constructor.
     *
     * Builds an AbstractPlugin and loads the configuration.
     *
     * @param p_uuid The UUID of the plug-in to build.
     * @param p_parent The parent of this QObject.
     **/
    AbstractPlugin ( const QUuid& p_uuid, QObject* p_parent = 0 );

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
     * @brief Obtains a URL to a page that provides more information about the plug-in.
     * @return An invalid QUrl if it couldn't be determined, a QUrl otherwise.
     **/
    const QUrl url() const;

    /**
     * @brief Obtains the QUuid of the plug-in.
     * @return A invalid QUuid if it couldn't be determined, a QUuid otherwise.
     **/
    const QUuid uuid() const;

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

public slots:

    /**
     * @brief ...
     *
     * @return void
     **/
    bool load();

protected:
    /**
     * @brief ...
     *
     * @return void
     **/
    virtual void initialize() = 0;
    /**
     * @brief ...
     *
     * @return void
     **/
    virtual void deinitialize() = 0;
    /**
     * @brief ...
     *
     * @return void
     **/
    bool loadComponents();

    QSettings* settings() const;
    QSettings* configuration() const;

private slots:
    /**
     * @brief ...
     *
     * @return void
     **/
    void start();
    /**
     * @brief ...
     *
     * @return void
     **/
    void stop();

private:
    /**
     * @brief ...
     *
     * @return void
     **/
    bool loadLibrary();
    /**
     * @brief ...
     *
     * @return void
     **/
    bool loadPlugins();
    QPluginLoader* m_ldr;
    QSettings* m_cfg;
    QSettings* m_sttgs;
};

/**
 * @brief ...
 **/
class GenericPlugin : public AbstractPlugin {
    Q_OBJECT
    Q_DISABLE_COPY ( GenericPlugin )

public:
    /**
     * @brief ...
     *
     * @param  ...
     **/
    GenericPlugin ( const QUuid& );

protected:
    /**
     * @brief ...
     *
     * @return void
     **/
    virtual void initialize() { };
    /**
     * @brief ...
     *
     * @return void
     **/
    virtual void deinitialize() { };
};

}
}

#endif // PLUGINS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
