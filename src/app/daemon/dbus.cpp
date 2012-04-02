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

#include <QMap>
#include <QList>
#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QMetaObject>
#include <QStringList>

#include "daemon.hpp"
#include "dbus.hpp"

using namespace SpeechControl::Daemon;
using namespace SpeechControl::Daemon::DBus;

DaemonAdaptor::DaemonAdaptor ()
    : QDBusAbstractAdaptor (Daemon::instance())
{
    setAutoRelaySignals (true);
    qDebug() << "[SpeechControl::Daemon::DBus::DaemonAdaptor::{constructor}] Created daemon's core adaptor.";
}

DaemonAdaptor::~DaemonAdaptor()
{
    qDebug() << "[SpeechControl::Daemon::DBus::DaemonAdaptor::{destructor}] Destroyed.";
}

bool DaemonAdaptor::active() const
{
    qDebug() << "[SpeechControl::Daemon::DBus::DaemonAdaptor::active] Is active?" << Daemon::instance()->isActive();
    return qvariant_cast< bool > (parent()->property ("Active"));
}

QString DaemonAdaptor::listen()
{
    return Daemon::instance()->listen();
}

void DaemonAdaptor::start()
{
    Daemon::instance()->start();
}

void DaemonAdaptor::stop()
{
    Daemon::instance()->stop();
}

#include "dbus.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
