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

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QPluginLoader>
#include <QApplication>

// Local
#include "core.hpp"
#include "config.hpp"
#include "plugins.hpp"
#include "factory.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Plugins;

Factory* Factory::s_inst = 0;
PluginMap Factory::s_ldPlgns;

Factory::Factory() : QObject (Core::instance())
{
    s_inst = this;
    QApplication::addLibraryPath (SPCHCNTRL_PLUGINS_LIB_DIR);
}

PluginMap Factory::availablePlugins()
{
    QDir l_specDir (SPCHCNTRL_PLUGINS_SPEC_DIR);
    l_specDir.setNameFilters (QStringList() << "*.spec");
    l_specDir.setFilter (QDir::Files);
    const QStringList l_specPths = l_specDir.entryList();
    PluginMap l_plgnLst;

    Q_FOREACH (const QString l_specPth, l_specPths) {
        const QString pth = l_specDir.absolutePath() + "/" + l_specPth;
        QSettings* specCfg = new QSettings (pth, QSettings::IniFormat, instance());
        const QString id (specCfg->value ("Plugin/ID").toString());
        l_plgnLst.insert (id, new GenericPlugin (id));
    }

    return l_plgnLst;
}

AbstractPlugin* Factory::plugin (const QString& p_id)
{
    if (isPluginLoaded (p_id))
        return s_ldPlgns.value (p_id);

    return 0;
}

PluginList Factory::loadedPlugins()
{
    return s_ldPlgns.values();
}

bool Factory::isPluginLoaded (const QString& p_id)
{
    return s_ldPlgns.keys().contains (p_id);
}

bool Factory::loadPlugin (const QString& p_id)
{
    if (isPluginLoaded (p_id)) {
        qDebug() << "[Factory::loadPlugin()] Plugin" << p_id << "already loaded.";
        return true;
    }

    GenericPlugin* l_gnrcPlgn = new GenericPlugin (p_id);

    if (l_gnrcPlgn->isSupported() && l_gnrcPlgn->loadComponents()) {
        AbstractPlugin* l_plgn = qobject_cast<AbstractPlugin*> (l_gnrcPlgn->m_ldr->instance());

        if (!l_plgn) {
            qDebug() << "[Factory::loadPlugin()] Couldn't nab core object.";
            return false;
        }
        else if (l_plgn->load()) {
            s_ldPlgns.insert (p_id, l_plgn);
            l_plgn->start();
            emit instance()->pluginLoaded (p_id);
            qDebug() << "[Factory::loadPlugin()] Plugin" << l_plgn->name() << "loaded.";
            return true;
        }
        else {
            qDebug() << "[Factory::loadPlugin()] Plugin" << l_plgn->name() << "refused to load.";
            return false;
        }
    }
    else {
        qDebug() << "[Factory::loadPlugin()] Plugin" << p_id << "unsupported.";
    }

    qDebug() << "[Factory::loadPlugin()] Plugin" << p_id << "not loaded.";
    return false;
}

void Factory::unloadPlugin (const QString& p_id)
{
    if (!isPluginLoaded (p_id)) {
        return;
    }

    if (s_ldPlgns.contains (p_id)) {
        AbstractPlugin* l_plgn = s_ldPlgns.value (p_id);
        l_plgn->stop();
        s_ldPlgns.remove (p_id);
        qDebug() << "[Factory::unloadPlugin()] Plugin" << l_plgn->name() << "unloaded.";
        delete l_plgn;
        emit instance()->pluginUnloaded (p_id);
    }
}

QSettings* Factory::pluginConfiguration (const QString& p_id)
{
    const QString pth (QString (SPCHCNTRL_PLUGINS_SPEC_DIR) + QString ("/") + p_id + QString (".spec"));
    return new QSettings (pth , QSettings::IniFormat, Factory::instance());
}

QSettings* Factory::pluginSettings (const QString& p_id)
{
    const QString pth (QDir::homePath() + Core::configurationPath().absolutePath() + "/plugins/" + p_id + QString (".conf"));
    return new QSettings (pth , QSettings::IniFormat, Factory::instance());
}

QStringList Factory::autoStart()
{
    return Core::configuration ("Plugins/AutoStart").toStringList();
}

bool Factory::doesLoadOnStart (const QString& id)
{
    return autoStart().contains (id);
}

void Factory::setLoadOnStart (const QString& p_id, const bool p_state)
{
    QStringList ids = autoStart();

    if (p_state)
        ids << p_id;
    else
        ids.removeAll (p_id);

    ids.removeDuplicates();
    Core::setConfiguration ("Plugins/AutoStart", ids);
}

void Factory::start()
{
    qDebug() << "[Factory::start()] Loading auto-start plug-ins..." << endl
             << autoStart();
    Q_FOREACH (const QString plgn, autoStart()) {
        qDebug() << "[Factory::start()] Attempting to load plug-in" << Factory::pluginConfiguration (plgn)->value("Plugin/Name").toString() << "...";
        if (Factory::pluginConfiguration (plgn)->value ("Plugin/Enabled").toBool())
            Plugins::Factory::loadPlugin (plgn);
    }
    qDebug() << "[Factory::start()] Auto-start plug-ins loaded.";
}

void Factory::stop()
{
    Q_FOREACH (AbstractPlugin * plgn, loadedPlugins()) {
        unloadPlugin (plgn->id());
    }
}

Factory::~Factory()
{

}

#include "factory.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
