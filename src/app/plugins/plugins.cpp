/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "plugins.hpp"

#include <QSettings>
#include <QPluginLoader>
#include <QApplication>

using namespace SpeechControl;

using SpeechControl::Plugins::AbstractPlugin;
/// @note is#0000034
AbstractPlugin::AbstractPlugin(QObject* p_prnt) :
    QObject(p_prnt), m_ldr(0), m_cfg(0), m_sttgs(0)
{
}

AbstractPlugin::AbstractPlugin ( const AbstractPlugin& p_other ) : QObject(p_other.parent()),
    m_ldr(p_other.m_ldr), m_cfg(p_other.m_cfg), m_sttgs(p_other.m_sttgs)
{

}

/// @todo Determine if a plug-in has been loaded.
const bool AbstractPlugin::hasLoaded() const
{
    return m_ldr && m_ldr->isLoaded();
}

const bool AbstractPlugin::isSupported() const
{
    return false;
}

const QString AbstractPlugin::name() const
{
    if (hasLoaded())
        return m_cfg->value("Plugin/Name").toString();

    return QString::null;
}

const QUuid AbstractPlugin::uuid() const
{
    if (hasLoaded())
        return QUuid(m_cfg->value("Plugin/Uuid").toString());

    return QUuid(QString::null);
}

const double AbstractPlugin::version() const
{
    if (hasLoaded())
        return m_cfg->value("Plugin/Version").toDouble();

    return -1.0;
}

const QString AbstractPlugin::description() const
{
    if (hasLoaded())
        return m_cfg->value("Plugin/Description").toString();

    return QString::null;
}

const QUrl AbstractPlugin::url() const
{
    if (hasLoaded())
        return m_cfg->value("Plugin/Url").toUrl();

    return QUrl(QString::null);
}

const QStringList AbstractPlugin::packages() const
{
    if (hasLoaded())
        return m_cfg->value("Dependencies/Packages").toStringList();

    return QStringList();
}

const Plugins::PluginList AbstractPlugin::plugins() const
{
    return Plugins::PluginList();
}

void AbstractPlugin::loadComponents()
{
    loadPackages();
    loadPlugins();
    loadLibrary();
}

void AbstractPlugin::loadLibrary()
{

}

void AbstractPlugin::loadPackages()
{

}

void AbstractPlugin::loadPlugins()
{

}

void AbstractPlugin::start()
{
    initialize();
    emit started();
}

void AbstractPlugin::stop()
{
    deinitialize();
    emit stopped();
}

AbstractPlugin::~AbstractPlugin()
{

}

Plugins::GenericPlugin::GenericPlugin ( const QUuid& p_uuid )
{

}

void Plugins::GenericPlugin::initialize()
{

}

void Plugins::GenericPlugin::deinitialize()
{

}
