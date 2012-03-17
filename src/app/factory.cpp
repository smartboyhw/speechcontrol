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
#include <QDebug>

// Local
#include "core.hpp"
#include "config.hpp"
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
        const QString l_pth = l_specDir.absolutePath() + "/" + l_specPth;
        QSettings* l_specCfg = new QSettings (l_pth, QSettings::IniFormat, instance());
        QFile l_file (l_pth);
        const QUuid l_uuid (l_specCfg->value ("Plugin/UUID").toString());
        l_plgnLst.insert (l_uuid, new GenericPlugin (l_uuid));
    }

    return l_plgnLst;
}

AbstractPlugin* Factory::plugin (const QUuid& p_uuid)
{
    if (isPluginLoaded (p_uuid))
        return s_ldPlgns.value (p_uuid);

    return 0;
}

PluginList Factory::loadedPlugins()
{
    return s_ldPlgns.values();
}

bool Factory::isPluginLoaded (const QUuid& p_uuid)
{
    return s_ldPlgns.keys().contains (p_uuid);
}

bool Factory::loadPlugin (const QUuid& p_uuid)
{
    if (isPluginLoaded (p_uuid)) {
        return true;
    }

    GenericPlugin* l_gnrcPlgn = new GenericPlugin (p_uuid);

    if (l_gnrcPlgn->isSupported() && l_gnrcPlgn->loadComponents()) {
        AbstractPlugin* l_plgn = qobject_cast<AbstractPlugin*> (l_gnrcPlgn->m_ldr->instance());

        if (!l_plgn) {
            qDebug() << "Couldn't nab core object.";
            return false;
        }
        else if (l_plgn->load()) {
            s_ldPlgns.insert (p_uuid, l_plgn);
            l_plgn->start();
            emit instance()->pluginLoaded (p_uuid);
            qDebug() << "Plugin" << l_plgn->name() << "loaded.";
            return true;
        }
        else {
            qDebug() << "Plugin" << l_plgn->name() << "refused to load.";
            return false;
        }
    }
    else {
        qDebug() << "Plugin" << p_uuid << "unsupported.";
    }

    qDebug() << "Plugin" << p_uuid << "not loaded.";
    return false;
}

void Factory::unloadPlugin (const QUuid& p_uuid)
{
    if (!isPluginLoaded (p_uuid)) {
        return;
    }

    if (s_ldPlgns.contains (p_uuid)) {
        AbstractPlugin* l_plgn = s_ldPlgns.value (p_uuid);
        l_plgn->stop();
        s_ldPlgns.remove (p_uuid);
        qDebug() << "Plugin" << l_plgn->name() << "unloaded.";
        delete l_plgn;
        emit instance()->pluginUnloaded (p_uuid);
    }
}

QSettings* Factory::pluginConfiguration (QUuid p_uuid)
{
    const QString pth (QString (SPCHCNTRL_PLUGINS_SPEC_DIR) + QString ("/") + p_uuid.toString() + QString (".spec"));
    return new QSettings (pth , QSettings::IniFormat, Factory::instance());
}

QSettings* Factory::pluginSettings (QUuid p_uuid)
{
    const QString pth (QDir::homePath() + Core::configurationPath().absolutePath() + "/plugins/" + p_uuid.toString() + QString (".conf"));
    return new QSettings (pth , QSettings::IniFormat, Factory::instance());
}

QList< QUuid > Factory::autoStart()
{
    QStringList strings = Core::configuration ("Plugins/AutoStart").toStringList();
    strings.removeDuplicates();
    QList<QUuid> uuids;

    Q_FOREACH (const QString string, strings) {
        uuids << QUuid (string);
    }

    return uuids;
}

bool Factory::doesLoadOnStart (QUuid uuid)
{
    return autoStart().contains (uuid);
}

void Factory::setLoadOnStart (QUuid uuid, bool state)
{
    QList<QUuid> uuids = autoStart();

    if (state)
        uuids << uuid;
    else
        uuids.removeAll (uuid);

    QStringList strings;
    Q_FOREACH (const QUuid uuid, uuids) {
        strings << uuid.toString();
    }

    strings.removeDuplicates();
    Core::setConfiguration ("Plugins/AutoStart", strings);
}

void Factory::start()
{
    Q_FOREACH (const QUuid plgn, autoStart()) {
        Plugins::Factory::loadPlugin (plgn);
    }
}

void Factory::stop()
{
    Q_FOREACH (AbstractPlugin * l_plgn, loadedPlugins()) {
        unloadPlugin (l_plgn->id());
    }
}

Factory::~Factory()
{

}

#include "factory.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
