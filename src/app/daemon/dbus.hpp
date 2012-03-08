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

#ifndef DBUS_HPP
#define DBUS_HPP

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "daemon.hpp"

class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

class DaemonAdaptor: public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO ( "D-Bus Interface", "org.thesii.SpeechControl.Daemon" )
    Q_CLASSINFO ( "D-Bus Introspection", ""
                  "  <interface name=\"org.thesii.SpeechControl.Daemon\">\n"
                  "    <property access=\"read\" type=\"b\" name=\"Active\"/>\n"
                  "    <signal name=\"started\"/>\n"
                  "    <signal name=\"stopped\"/>\n"
                  "    <method name=\"start\">\n"
                  "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                  "    </method>\n"
                  "    <method name=\"stop\">\n"
                  "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
                  "    </method>\n"
                  "    <method name=\"listen\">\n"
                  "      <arg direction=\"out\" type=\"s\"/>\n"
                  "    </method>\n"
                  "  </interface>\n"
                  "" )
public:
    DaemonAdaptor ();
    virtual ~DaemonAdaptor();

    inline SpeechControl::Daemon::Daemon *parent() const {
        return static_cast<SpeechControl::Daemon::Daemon *> ( QObject::parent() );
    }

public: // PROPERTIES
    Q_PROPERTY ( bool Active READ active )
    bool active() const;

public Q_SLOTS: // METHODS
    QString listen();
    Q_NOREPLY void start();
    Q_NOREPLY void stop();
Q_SIGNALS: // SIGNALS
    void started();
    void stopped();
};

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
