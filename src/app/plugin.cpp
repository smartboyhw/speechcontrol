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
 * @date 05/16/12 21:41:05 PM
 */

#include <QDebug>
#include <QSettings>
#include <QPluginLoader>
#include <QApplication>
#include "core.hpp"
#include "factory.hpp"
#include "indicator.hpp"
#include "plugin.hpp"
#include "pluginprivate.hpp"

using namespace SpeechControl;

AbstractPluginPrivate::AbstractPluginPrivate(AbstractPlugin* p_Qptr) :
    ldr(0), id(QString::null), acts(), menus(), q_ptr(p_Qptr)
{

}

bool AbstractPluginPrivate::loadLibrary()
{
    Q_Q(AbstractPlugin);
    const QString libName = "lib" + q->configuration()->value ("Dependencies/Library").toString() + ".so";
    const QString pth = SPCHCNTRL_PLUGINS_LIB_DIR "/" + libName;
    ldr = new QPluginLoader;
    ldr->setFileName (pth);

    if (!ldr->load()) {
        qDebug() << "[AbstractPlugin::loadLibrary()]" << q->name() << "'s library failed to load."
                 << ldr->errorString() << ldr->fileName() << pth << id;
        return false;
    }

    return true;
}

bool AbstractPluginPrivate::loadPlugins()
{
    Q_Q(AbstractPlugin);
    Q_FOREACH (AbstractPlugin * plgn, q->plugins()) {
        if (! (plgn->isSupported() && Factory::isPluginLoaded (plgn->id()))) {
            qDebug() << "[AbstractPlugin::loadPlugins()] Plugin" << q->name() << "is missing a dependency:" << plgn->name();
            return false;
        }
    }

    return true;
}

AbstractPluginPrivate::~AbstractPluginPrivate() {

}


AbstractPlugin::AbstractPlugin (QObject* p_prnt) :
    QObject (p_prnt), d_ptr(new AbstractPluginPrivate(this))
{
    /// @todo Implement the system such that types are respected
    ptype = Service;
}

AbstractPlugin::AbstractPlugin (const QString& p_id, QObject* p_parent) : QObject (p_parent),
    d_ptr(new AbstractPluginPrivate(this))
{
    Q_D(AbstractPlugin);
    d->id  = p_id;

    /// @todo Implement the system such that types are respected
    ptype = Service;
}

/// @todo Add implementation for copy constructor.
AbstractPlugin::AbstractPlugin (const AbstractPlugin& p_other) : QObject (p_other.parent()),
    d_ptr(new AbstractPluginPrivate(this))
{
}

/// @todo Determine if a plug-in has been loaded.
bool AbstractPlugin::hasLoaded() const
{
    Q_D(const AbstractPlugin);
    return d->ldr && d->ldr->isLoaded();
}

bool AbstractPlugin::isSupported() const
{
    qDebug() << "[AbstractPlugin::isSupported()] Is enabled?" << settings()->value ("Plugin/Enabled", false).toBool();

    if (settings())
        return settings()->value ("Plugin/Enabled", false).toBool();

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

AbstractPlugin::PluginType AbstractPlugin::type() const
{
    return ptype;
}

bool AbstractPlugin::loadComponents()
{
    Q_D(AbstractPlugin);
    if (d->loadPlugins()) {
        return d->loadLibrary();
    }

    qDebug() << "[AbstractPlugin::loadComponents()] Failed to load components for " << name();
    return false;
}

void AbstractPlugin::start()
{
    initialize();
    emit started();
}

void AbstractPlugin::stop()
{
    Q_FOREACH (QAction * action, actions()) {
        if (!action)
            continue;

        Indicator::removeActionForPlugins (action);
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
    Q_D(const AbstractPlugin);
    return Factory::pluginConfiguration (d->id);
}

QSettings* AbstractPlugin::settings() const
{
    Q_D(const AbstractPlugin);
    return Factory::pluginSettings (d->id);
}

QList< QAction* > AbstractPlugin::actions() const
{
    Q_D(const AbstractPlugin);
    return d->acts;
}

QList< QMenu* > AbstractPlugin::menus() const
{
    Q_D(const AbstractPlugin);
    return d->menus;
}

void AbstractPlugin::addAction (QAction* p_action)
{
    p_action->setParent (Indicator::instance());
    Indicator::addActionForPlugins (p_action);
    qDebug() << "[AbstractPlugin::addAction()] Added action" << p_action->text() << "to the Indicator";
}

void AbstractPlugin::addMenu (QMenu* p_menu)
{
    Indicator::addMenuForPlugins(p_menu);
}

void AbstractPlugin::addActions (QList< QAction* > p_actions)
{
    Q_FOREACH (QAction * action, p_actions) {
        addAction (action);
    }
}

void AbstractPlugin::addMenus (QList< QMenu* > p_menus)
{
    Q_FOREACH (QMenu * menu, p_menus) {
        addMenu (menu);
    }
}

bool AbstractPlugin::isEnabled() const
{
    return settings()->value ("Plugin/Enabled").toBool();
}

bool AbstractPlugin::isLoaded() const
{
    return Factory::isPluginLoaded (id());
}

AbstractPlugin::~AbstractPlugin()
{
}

GenericPlugin::GenericPlugin (const QString& p_id) : AbstractPlugin (p_id, Core::instance())
{
}

GenericPlugin::GenericPlugin (const GenericPlugin& p_other) : AbstractPlugin (p_other.id(), Core::instance())
{
    qFatal ("Shouldn't hit here.");
}

QPixmap GenericPlugin::pixmap() const
{
    return QApplication::windowIcon().pixmap (64, 64);
}

#include "plugin.moc"
// kate: indent-mode cstyle; replace-tabs on;
