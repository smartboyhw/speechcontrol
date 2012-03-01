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

#include "system.hpp"

using namespace SpeechControl::Daemon;

System* System::s_inst = 0;

System::System ( int p_argc,char** p_argv ) : QObject ( qApp ),
    m_dmn ( new Daemon ), m_app ( p_argc,p_argv ) {

    connect ( &m_app,SIGNAL ( aboutToQuit() ),m_dmn,SLOT ( stop() ) );
}

System* System::instance ( int p_argc,char** p_argv ) {
    if ( s_inst == 0 )
        s_inst = new System ( p_argc,p_argv );

    return s_inst;
}

Daemon* System::daemon() {
    return m_dmn;
}

int System::exec() {
    daemon()->start();
    return m_app.exec();
}

#include "system.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
