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

AbstractPlugin::AbstractPlugin (QObject* p_prnt) :
    QObject (p_prnt), m_ldr (0), m_id (QString::null)
{
}

AbstractPlugin::AbstractPlugin (const QString& p_id, QObject* p_parent) : QObject (p_parent),
    m_ldr (0), m_id (p_id)
{

}

AbstractPlugin::AbstractPlugin (const AbstractPlugin& p_other) : QObject (p_other.parent()),
    m_ldr (p_other.m_ldr), m_id (p_other.m_id)
{

}

/// @todo Determine if a plug-in has been loaded.
bool AbstractPlugin::hasLoaded() const
{
    return m_ldr && m_ldr->isLoaded();
}

bool AbstractPlugin::isSupported() const
{
    qDebug() << "[AbstractPlugin::isSupported()] Is enabled?" << configuration()->value("Plugin/Enabled").toBool();

    if (configuration())
        return configuration()->value("Plugin/Enabled").toBool();

    return false;
}

const QString AbstractPlugin::name() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/Name").toString();
    }

    return QString::null;
}

const QString AbstractPlugin::id() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/ID").toString();
    }

    return QString::null;
}

double AbstractPlugin::version() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/Version").toDouble();
    }

    return -1.0;
}

const QString AbstractPlugin::author() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/Author").toString();
    }

    return QString::null;
}

const QString AbstractPlugin::description() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/Description").toString();
    }

    return QString::null;
}

const QUrl AbstractPlugin::url() const
{
    if (configuration()) {
        return configuration()->value ("Plugin/URL").toUrl();
    }

    return QUrl (QString::null);
}

const PluginList AbstractPlugin::plugins() const
{
    if (configuration()) {
        PluginList plgnLst;
        const QStringList plgns = configuration()->value ("Dependencies/Plugins").toStringList();

        if (!plgns.isEmpty()) {
            Q_FOREACH (const QString id, plgns) {
                plgnLst << new GenericPlugin (id);
            }
        }

        return plgnLst;
    }

    return PluginList();
}

bool AbstractPlugin::loadComponents()
{
    if (loadPlugins()) {
        return loadLibrary();
    }

    qDebug() << "[AbstractPlugin::loadComponents()] Failed to load components for " << name();
    return false;
}

bool AbstractPlugin::loadLibrary()
{
    const QString libName = "lib" + configuration()->value ("Dependencies/Library").toString() + ".so";
    const QString pth = SPCHCNTRL_PLUGINS_LIB_DIR "/" + libName;
    m_ldr = new QPluginLoader;
    m_ldr->setFileName (pth);

    if (!m_ldr->load()) {
        qDebug() << "[AbstractPlugin::loadLibrary()]" << name() << "'s library failed to load."
                 << m_ldr->errorString() << m_ldr->fileName() << pth << m_id;
        return false;
    }

    return true;
}

bool AbstractPlugin::loadPlugins()
{
    Q_FOREACH (AbstractPlugin * l_plgn, plugins()) {
        if (! (l_plgn->isSupported() && Factory::isPluginLoaded (l_plgn->id()))) {
            qDebug() << "[AbstractPlugin::loadPlugins()] Plugin" << name() << "is missing a dependency:" << l_plgn->name();
            return false;
        }
    }

    return true;
}

void AbstractPlugin::start()
{
    initialize();
    emit started();
}

void AbstractPlugin::stop()
{
    Q_FOREACH (QAction * l_action, actions()) {
        if (!l_action)
            continue;

        Core::mainWindow()->m_ui->menuPlugins->removeAction (l_action);
    }
    deinitialize();
    emit stopped();
}

bool AbstractPlugin::load()
{
    if (!isSupported()) {
        return false;
    }

    return loadComponents();
}

QSettings* AbstractPlugin::configuration() const
{
    return Factory::pluginConfiguration (m_id);
}

QSettings* AbstractPlugin::settings() const
{
    return Factory::pluginSettings (m_id);
}

QList< QAction* > AbstractPlugin::actions()
{
    return m_acts;
}

void AbstractPlugin::addAction (QAction* p_action)
{
    p_action->setParent (this);
    Core::mainWindow()->m_ui->menuPlugins->insertAction (0, p_action);
    qDebug() << "[AbstractPlugin::addAction()] Added action" << p_action->text() << "to the Main window.";
}

void AbstractPlugin::addActions (QList< QAction* > p_actions)
{
    Q_FOREACH (QAction * l_action, p_actions) {
        addAction (l_action);
    }
}

bool AbstractPlugin::isEnabled() const
{
    return configuration()->value ("Plugin/Enabled").toBool();
}

bool AbstractPlugin::isLoaded() const
{
    return Factory::isPluginLoaded (id());
}

AbstractPlugin::~AbstractPlugin()
{

}

Plugins::GenericPlugin::GenericPlugin (const QString& p_id) : AbstractPlugin (p_id, Core::instance())
{
}

Plugins::GenericPlugin::GenericPlugin (const Plugins::GenericPlugin& p_other) : AbstractPlugin (p_other.id(), Core::instance())
{
    qFatal ("Shouldn't hit here.");
}

QPixmap Plugins::GenericPlugin::pixmap() const
{
    return QApplication::windowIcon().pixmap (64, 64);
}

#include "plugins.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
