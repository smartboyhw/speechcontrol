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
class AbstractPlugin;

/**
 * @brief ...
 **/
typedef QList<AbstractPlugin*> PluginList;

/**
 * @brief ...
 **/
typedef QMap<QUuid, AbstractPlugin*> PluginMap;

/**
 * @brief ...
 **/
class AbstractPlugin : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( AbstractPlugin )
    friend class Factory;

signals:
    /**
     * @brief ...
     *
     * @return void
     **/
    void started();

    /**
     * @brief ...
     *
     * @return void
     **/
    void stopped();

public:
    /**
     * @brief ...
     *
     * @param  ... Defaults to 0.
     **/
    explicit AbstractPlugin ( QObject* = 0 );

    /**
     * @brief ...
     *
     **/
    virtual ~AbstractPlugin();

    /**
     * @brief ...
     *
     * @return const bool
     **/
    bool hasLoaded() const;

    /**
     * @brief ...
     *
     * @return const double
     **/
    double version() const;

    /**
     * @brief ...
     *
     * @return const QString
     **/
    const QString name() const;

    /**
     * @brief ...
     *
     * @return const QString
     **/
    const QString description() const;

    /**
     * @brief ...
     *
     * @return const QUrl
     **/
    const QUrl url() const;

    /**
     * @brief ...
     *
     * @return const QUuid
     **/
    const QUuid uuid() const;

    /**
     * @brief ...
     *
     * @return :Plugins::PluginList
     **/
    const PluginList plugins() const;

    /**
     * @brief ...
     *
     * @return const bool
     **/
    bool isSupported() const;

public slots:
    /**
     * @brief ...
     *
     * @return void
     **/
    void unload();

    /**
     * @brief ...
     *
     * @return void
     **/
    void load();

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
    QSettings* m_cfg;
    QSettings* m_sttgs;

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
