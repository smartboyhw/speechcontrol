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
#include "windows/main-window.hpp"
#include "ui_main-window.h"

#include <QDebug>
#include <QSettings>
#include <QPluginLoader>
#include <QApplication>
#include <QAction>

using namespace SpeechControl;
using SpeechControl::Plugins::Factory;
using SpeechControl::Plugins::PluginMap;
using SpeechControl::Plugins::PluginList;
using SpeechControl::Plugins::AbstractPlugin;

/// @bug The symbols for this class aren't exported properly.
AbstractPlugin::AbstractPlugin ( QObject* p_prnt ) :
    QObject ( p_prnt ), m_ldr ( 0 ), m_cfg ( 0 ), m_sttgs ( 0 ) {
}

AbstractPlugin::AbstractPlugin ( const QUuid& p_uuid , QObject* p_parent ) : QObject ( p_parent ),
    m_ldr ( 0 ), m_cfg ( Factory::pluginConfiguration ( p_uuid ) ), m_sttgs ( Factory::pluginSettings ( p_uuid ) ) {

}


AbstractPlugin::AbstractPlugin ( const AbstractPlugin& p_other ) : QObject ( p_other.parent() ),
    m_ldr ( p_other.m_ldr ), m_cfg ( p_other.m_cfg ), m_sttgs ( p_other.m_sttgs ) {

}

/// @todo Determine if a plug-in has been loaded.
bool AbstractPlugin::hasLoaded() const {
    return m_ldr && m_ldr->isLoaded();
}

bool AbstractPlugin::isSupported() const {
    return true;
}

const QString AbstractPlugin::name() const {
    if ( hasLoaded() ) {
        return m_cfg->value ( "Plugin/Name" ).toString();
    }
    return QString::null;
}

const QUuid AbstractPlugin::uuid() const {
    if ( hasLoaded() ) {
        return QUuid ( m_cfg->value ( "Plugin/UUID" ).toString() );
    }

    return QUuid ( QString::null );
}

double AbstractPlugin::version() const {
    if ( hasLoaded() ) {
        return m_cfg->value ( "Plugin/Version" ).toDouble();
    }

    return -1.0;
}

const QString AbstractPlugin::description() const {
    if ( hasLoaded() ) {
        return m_cfg->value ( "Plugin/Description" ).toString();
    }

    return QString::null;
}

const QUrl AbstractPlugin::url() const {
    if ( hasLoaded() ) {
        return m_cfg->value ( "Plugin/URL" ).toUrl();
    }

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
    if ( loadPlugins() ) {
        return loadLibrary();
    }

    qDebug() << "Failed to load components for " << name();
    return false;
}

bool AbstractPlugin::loadLibrary() {
    const QString l_libName = "lib" + m_cfg->value ( "Dependencies/Library" ).toString() + ".so";
    const QString l_pth = SPCHCNTRL_PLUGINS_LIB_DIR "/" + l_libName;
    m_ldr = new QPluginLoader;
    m_ldr->setFileName ( l_pth );
    if ( !m_ldr->load() ) {
        qDebug() << name() << "'s library failed to load."
                 << m_ldr->errorString() << m_ldr->fileName() << l_pth;
        return false;
    }

    return true;
}

bool AbstractPlugin::loadPlugins() {
    Q_FOREACH ( AbstractPlugin* l_plgn, plugins() ) {
        if ( ! ( l_plgn->isSupported() && Factory::isPluginLoaded ( l_plgn->uuid() ) ) ) {
            qDebug() << "Plugin" << name() << "is missing a dependency:" << l_plgn->name();
            return false;
        }
    }

    return true;
}

void AbstractPlugin::start() {
    initialize();
    emit started();
}

void AbstractPlugin::stop() {
    Q_FOREACH ( QAction* l_action, actions() ) {
        if ( !l_action )
            continue;
        Core::mainWindow()->m_ui->menuPlugins->removeAction ( l_action );
    }
    deinitialize();
    emit stopped();
}

bool AbstractPlugin::load() {
    if ( !isSupported() ) {
        return false;
    }

    return loadComponents();
}

QSettings* AbstractPlugin::configuration() const {
    return m_cfg;
}

QSettings* AbstractPlugin::settings() const {
    return m_sttgs;
}

QList< QAction* > AbstractPlugin::actions() {
    return m_acts;
}

void AbstractPlugin::addAction ( QAction* p_action ) {
    p_action->setParent ( this );
    Core::mainWindow()->m_ui->menuPlugins->insertAction ( 0,p_action );
}

void AbstractPlugin::addActions ( QList< QAction* > p_actions ) {
    Q_FOREACH ( QAction* l_action, p_actions ) {
        addAction ( l_action );
    }
}

AbstractPlugin::~AbstractPlugin() {

}

Plugins::GenericPlugin::GenericPlugin ( const QUuid& p_uuid ) : AbstractPlugin ( p_uuid, Core::instance() ) {
}

#include "plugins.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
