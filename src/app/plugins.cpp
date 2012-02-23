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

#include "core.hpp"
#include "config.hpp"
#include "plugins.hpp"
#include "factory.hpp"

#include <QSettings>
#include <QPluginLoader>
#include <QApplication>

#include "factory.hpp"

#include <QSettings>
#include <QPluginLoader>
#include <QApplication>

using namespace SpeechControl;
using SpeechControl::Plugins::PluginMap;
using SpeechControl::Plugins::PluginList;
using SpeechControl::Plugins::AbstractPlugin;

/// @bug #0000034
AbstractPlugin::AbstractPlugin ( QObject* p_prnt ) :
    QObject ( p_prnt ), m_cfg ( 0 ), m_sttgs ( 0 ), m_ldr ( 0 ) {
}

AbstractPlugin::AbstractPlugin ( const AbstractPlugin& p_other ) : QObject ( p_other.parent() ),
    m_cfg ( p_other.m_cfg ), m_sttgs ( p_other.m_sttgs ), m_ldr ( p_other.m_ldr ) {

}

/// @todo Determine if a plug-in has been loaded.
bool AbstractPlugin::hasLoaded() const {
    return m_ldr && m_ldr->isLoaded();
}

bool AbstractPlugin::isSupported() const {
    return false;
}

const QString AbstractPlugin::name() const {
    if ( hasLoaded() )
        return m_cfg->value ( "Plugin/Name" ).toString();

    return QString::null;
}

const QUuid AbstractPlugin::uuid() const {
    if ( hasLoaded() )
        return QUuid ( m_cfg->value ( "Plugin/UUID" ).toString() );

    return QUuid ( QString::null );
}

double AbstractPlugin::version() const {
    if ( hasLoaded() )
        return m_cfg->value ( "Plugin/Version" ).toDouble();

    return -1.0;
}

const QString AbstractPlugin::description() const {
    if ( hasLoaded() )
        return m_cfg->value ( "Plugin/Description" ).toString();

    return QString::null;
}

const QUrl AbstractPlugin::url() const {
    if ( hasLoaded() )
        return m_cfg->value ( "Plugin/URL" ).toUrl();

    return QUrl ( QString::null );
}

const PluginList AbstractPlugin::plugins() const {
    if ( hasLoaded() ) {
        PluginList l_lst;
        const QStringList l_plgns = m_cfg->value ( "Dependencies/Plugins" ).toStringList();

        Q_FOREACH ( const QString l_plgnUuid, l_plgns ) {
            l_lst << new GenericPlugin ( l_plgnUuid );
        }

        return l_lst;
    }

    return PluginList();
}

bool AbstractPlugin::loadComponents() {
    if ( loadPlugins() )
        return loadLibrary();

    return false;
}

bool AbstractPlugin::loadLibrary() {
    const QString l_libName = m_cfg->value ( "Dependencies/Library" ).toString();
    m_ldr = new QPluginLoader ( l_libName );
    return m_ldr->load();
}

bool AbstractPlugin::loadPlugins() {
    Q_FOREACH ( AbstractPlugin* l_plgn, plugins() ) {
        if ( ! ( l_plgn->isSupported() && Factory::isPluginLoaded ( l_plgn->uuid() ) ) )
            return false;
    }

    return true;
}

void AbstractPlugin::start() {
    initialize();
    emit started();
}

void AbstractPlugin::stop() {
    deinitialize();
    emit stopped();
}

void AbstractPlugin::load() {
    if ( !isSupported() )
        return;

    loadComponents();
}

void AbstractPlugin::unload() {

}

AbstractPlugin::~AbstractPlugin() {

}

Plugins::GenericPlugin::GenericPlugin ( const QUuid& p_uuid ) : AbstractPlugin ( Core::instance() ) {
    m_cfg = Factory::pluginConfiguration ( p_uuid );
    qDebug() << m_cfg->allKeys();
    m_sttgs = Factory::pluginSettings ( p_uuid );
}

#include "plugins.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
