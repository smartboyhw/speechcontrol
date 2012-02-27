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

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <QObject>
#include "plugins.hpp"

namespace SpeechControl {
namespace Plugins {
class Factory;

/**
 * @brief ...
 **/
class Factory : public QObject {
    Q_OBJECT

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
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void pluginLoaded ( const QUuid& );
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void pluginUnloaded ( const QUuid& );

public slots:
    void start();
    void stop();

public:
    virtual ~Factory();

    /**
     * @brief ...
     *
     * @param  ...
     * @return const bool
     **/
    static bool loadPlugin ( const QUuid& );

    /**
     * @brief ...
     *
     * @param  ...
     * @return const bool
     **/
    void unloadPlugin ( const QUuid& p_uuid );

    /**
     * @brief ...
     *
     * @param  ...
     * @return bool
     **/
    static bool isPluginLoaded ( const QUuid& );

    /**
     * @brief ...
     *
     * @return :Plugins::PluginList
     **/
    static PluginList loadedPlugins();
    /**
     * @brief ...
     *
     * @return :Plugins::PluginList
     **/
    static PluginMap availablePlugins();
    /**
     * @brief ...
     *
     * @return :Plugins::Factory*
     **/
    static Factory* instance();

    /**
     *
     *
     * @brief ...
     *
     * @param p_uuid ...
     * @return QSettings*
     **/
    static QSettings* pluginConfiguration ( QUuid p_uuid );

    /**
     * @brief ...
     *
     * @param p_uuid ...
     * @return QSettings*
     **/
    static QSettings* pluginSettings ( QUuid p_uuid );

private:
    explicit Factory();
    static Factory* s_inst;
    static PluginMap s_ldPlgns;
};
}
}

#endif // FACTORY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
