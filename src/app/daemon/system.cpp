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

#include <lib/system.hpp>
#include <lib/config.hpp>

#include "dbus.hpp"
#include "system.hpp"

using namespace SpeechControl::Daemon;

System* System::s_inst = 0;

System::System (int p_argc, char** p_argv) : QObject (qApp),
    m_app (p_argc, p_argv), m_cnntn(0)
{
    m_app.setApplicationName ("SpeechControl");
    m_app.setApplicationVersion (SPCHCNTRL_BUILD_VERSION);
    m_app.setOrganizationDomain ("thesii.org");
    m_app.setOrganizationName ("Synthetic Intellect Institute");

    SpeechControl::System::start (&p_argc, &p_argv);
    connect (&m_app, SIGNAL (aboutToQuit()), Daemon::instance(), SLOT (stop()));

    buildDBus();
}

void System::buildDBus()
{
    m_cnntn = new QDBusConnection(QDBusConnection::sessionBus().connectToBus(QDBusConnection::SessionBus,"SpeechControl"));
    qDebug() << m_cnntn->registerObject ("/Daemon", (new DBus::DaemonAdaptor));
    qDebug() << m_cnntn->registerService("org.thesii.SpeechControl.Daemon");
}

System* System::instance (int p_argc, char** p_argv)
{
    if (s_inst == 0) {
        s_inst = new System (p_argc, p_argv);
    }

    return s_inst;
}

Daemon* System::daemon()
{
    return Daemon::instance();
}

int System::exec()
{
    return m_app.exec();
}

#include "system.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
