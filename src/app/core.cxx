/***
 *  This file is part of SpeechControl.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QSettings>
#include <QTranslator>
#include <QApplication>

#include "app/services/module.hpp"
#include "app/services/desktopcontrol/service.hpp"
#include "app/services/dictation/service.hpp"
#include "app/services/voxforge/service.hpp"
#include "app/plugins.hpp"
#include "app/factory.hpp"
#include "app/core.hpp"
#include "app/core.hxx"

using namespace SpeechControl;
using SpeechControl::Services::AbstractModule;
using SpeechControl::Core;

CorePrivate::CorePrivate(Core* p_qPtr) : m_app (0),
    m_settings (0), m_trnsltr (0), q_ptr(p_qPtr)
{
    Q_Q (Core);
    m_trnsltr = new QTranslator (q);
    m_settings = new QSettings (QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", q);

    m_app->installTranslator (m_trnsltr);
}

void CorePrivate::hookUpSignals()
{
    Q_Q (Core);
    QObject::connect (m_app, SIGNAL (aboutToQuit()), q, SLOT (stop()));
    QObject::connect (q, SIGNAL (started()), Services::Engine::instance(), SLOT (start()));
    QObject::connect (q, SIGNAL (started()), Plugins::Factory::instance(), SLOT (start()));

    QObject::connect (q, SIGNAL (stopped()), Services::Engine::instance(), SLOT (stop()));
    QObject::connect (q, SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (stop()));

    bootServices();
}

void CorePrivate::invokeAutoStart()
{
    const bool dsktpCntrlState = Core::configuration ("DesktopControl/AutoStart", false).toBool();
    const bool dctnState = Core::configuration ("Dictation/AutoStart", false).toBool();
    DesktopControl::Service::instance()->setState ( (dsktpCntrlState) ? AbstractModule::Enabled  : AbstractModule::Disabled);
    Dictation::Service::instance()->setState ( (dctnState) ? AbstractModule::Enabled  : AbstractModule::Disabled);

}

void CorePrivate::bootServices()
{
    qDebug() << "[CorePrivate::bootServices()] Initializing services...";
    DesktopControl::Service::instance();
    Dictation::Service::instance();
    Voxforge::Service::instance();
    qDebug() << "[CorePrivate::bootServices()] Initialized. services.";
}

CorePrivate::~CorePrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
