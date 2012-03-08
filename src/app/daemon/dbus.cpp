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

#include "dbus.hpp"
#include "daemon.hpp"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

DaemonAdaptor::DaemonAdaptor ( )
    : QDBusAbstractAdaptor ( SpeechControl::Daemon::Daemon::instance() ) {
    setAutoRelaySignals ( true );
}

DaemonAdaptor::~DaemonAdaptor() {
}

bool DaemonAdaptor::active() const {
    return qvariant_cast< bool > ( parent()->property ( "Active" ) );
}

QString DaemonAdaptor::listen() {
    return parent()->listen();
}

void DaemonAdaptor::start() {
    parent()->start();
}

void DaemonAdaptor::stop() {
    parent()->stop();
}

#include "dbus.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
