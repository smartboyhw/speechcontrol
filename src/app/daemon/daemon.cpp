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

#include <QApplication>
#include <lib/system.hpp>
#include "daemon.hpp"

using namespace SpeechControl::Daemon;

Daemon* Daemon::s_inst = 0;

DaemonSphinx::DaemonSphinx() : AbstractSphinx() {
    buildPipeline(standardDescription());
}

void DaemonSphinx::applicationMessage ( const QGst::MessagePtr& p_message ) {
    qDebug() << p_message->typeName();
}


/// @todo Add the needed variables and initialization code for the Daemon's activities here.
Daemon::Daemon() : QObject(QApplication::instance()), m_sphnx(new DaemonSphinx){
    connect(this,SIGNAL(started()),SpeechControl::System::instance(),SLOT(start()));
    connect(this,SIGNAL(started()),m_sphnx,SLOT(start()));

    connect(this,SIGNAL(stopped()),SpeechControl::System::instance(),SLOT(stop()));
    connect(this,SIGNAL(stopped()),m_sphnx,SLOT(stop()));
}

/// @todo We need to provide a more concise means of determining if the daemon's active.
///       Or even perhaps a better definition of what would constitute as activity.
bool Daemon::isActive() const {
    return true;
}

/// @todo Activate SpeechControl's ASR and then return a value.
QString Daemon::listen() {
    if ( !isActive() ) {
        return QString::null;
    }

    return QString::null;
}

/// @todo Implement a means of 'activating' the daemon.
/// @todo This action should cause isActive() to return true.
void Daemon::start() {
    emit started();
}

/// @todo Implement a means of 'deactivating the daemon'.
/// @todo This action should cause isActive() to return false.
void Daemon::stop() {
    emit stopped();
}

Daemon* Daemon::instance() {
    if (s_inst == 0)
        s_inst = new Daemon;

    return s_inst;
}

#include "daemon.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
