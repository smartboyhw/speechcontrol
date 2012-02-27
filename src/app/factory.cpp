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

#include <QSettings>
#include <QPluginLoader>

#include "core.hpp"
#include "config.hpp"
#include "factory.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;

Factory* Factory::s_inst = 0;
PluginMap Factory::s_ldPlgns;

Factory::Factory( ) : QObject ( Core::instance() ) {
    s_inst = this;
    SC_APP::addLibraryPath ( SPCHCNTRL_PLUGINS_LIB_DIR );
}

PluginMap Factory::availablePlugins() {
    QDir l_specDir ( SPCHCNTRL_PLUGINS_SPEC_DIR );
    l_specDir.setNameFilters ( QStringList() << "*.spec" );
    l_specDir.setFilter ( QDir::Files );
    const QStringList l_specPths = l_specDir.entryList();
    PluginMap l_plgnLst;

    Q_FOREACH ( const QString l_specPth, l_specPths ) {
        const QString l_pth = l_specDir.absolutePath() + "/" + l_specPth;
        QSettings* l_specCfg = new QSettings ( l_pth,QSettings::IniFormat,instance() );
        QFile l_file ( l_pth );
        const QUuid l_uuid ( l_specCfg->value ( "Plugin/UUID" ).toString() );
        l_plgnLst.insert ( l_uuid, new GenericPlugin ( l_uuid ) );
    }

    return l_plgnLst;
}

PluginList Factory::loadedPlugins() {
    return s_ldPlgns.values();
}

bool Factory::isPluginLoaded ( const QUuid& p_uuid ) {
    return s_ldPlgns.keys().contains ( p_uuid );
}

bool Factory::loadPlugin ( const QUuid& p_uuid ) {
    GenericPlugin* l_gnrcPlgn = new GenericPlugin ( p_uuid );

    if ( l_gnrcPlgn->isSupported() && l_gnrcPlgn->loadComponents() ) {
        AbstractPlugin* l_plgn = qobject_cast<AbstractPlugin*> ( l_gnrcPlgn->m_ldr->instance() );
        if ( l_plgn->load() ) {
            s_ldPlgns.insert ( p_uuid,l_plgn );
            l_plgn->start();
            emit instance()->pluginLoaded ( p_uuid );
            qDebug() << "Plugin" << l_plgn->name() << "loaded.";
            return true;
        } else {
            qDebug() << "Plugin" << l_plgn->name() << "refused to load.";
            return false;
        }
    } else
        qDebug() << "Plugin" << p_uuid << "unsupported.";

    qDebug() << "Plugin" << p_uuid << "not loaded.";
    return false;
}

void Factory::unloadPlugin ( const QUuid& p_uuid ) {
    if ( s_ldPlgns.contains ( p_uuid ) ) {
        AbstractPlugin* l_plgn = s_ldPlgns.value ( p_uuid );
        l_plgn->stop();
        s_ldPlgns.remove ( p_uuid );
        qDebug() << "Plugin" << p_uuid << "unloaded.";
        emit instance()->pluginUnloaded ( p_uuid );
    }
}

Factory* Factory::instance() {
    if ( s_inst == 0 )
        s_inst = new Factory;

    return s_inst;
}

QSettings* Factory::pluginConfiguration ( QUuid p_uuid ) {
    const QString l_pth ( QString ( SPCHCNTRL_PLUGINS_SPEC_DIR ) + QString ( "/" ) + p_uuid.toString().replace ( "{","" ).replace ( "}","" ) + QString ( ".spec" ) );
    return new QSettings ( l_pth ,QSettings::IniFormat,Factory::instance() );
}

QSettings* Factory::pluginSettings ( QUuid p_uuid ) {
    const QString l_pth ( QDir::homePath() + QString ( "/.speechcontrol/plugins/" ) + p_uuid.toString().replace ( "{","" ).replace ( "}","" ) + QString ( ".conf" ) );
    return new QSettings ( l_pth ,QSettings::IniFormat,Factory::instance() );
}

void Factory::start() {
    qDebug() << "Factory started.";
    const QStringList l_plgnLst = Core::configuration ( "Plugins/AutoStart" ).toStringList();
    Q_FOREACH ( const QUuid l_plgn, availablePlugins().keys() ) {
        Plugins::Factory::loadPlugin ( l_plgn );
    }
}

void Factory::stop() {
    Q_FOREACH ( AbstractPlugin* l_plgn, loadedPlugins() ) {
        unloadPlugin ( l_plgn->uuid() );
    }

    qDebug() << "Factory stopped.";
}

Factory::~Factory() {

}

#include "factory.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
