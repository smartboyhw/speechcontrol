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

#include "core.hpp"
#include "agent.hpp"

using SpeechControl::Core;
using SpeechControl::AbstractAgent;
using SpeechControl::DesktopControl::Agent;

Agent* Agent::s_inst = 0;

Agent::Agent() : AbstractAgent ( Core::instance() ) {

}

Agent* Agent::instance() {
    if ( s_inst == 0 )
        s_inst = new Agent;

    return s_inst;
}

AbstractAgent::OperationState Agent::onStateChanged ( const AbstractAgent::OperationState p_stt ) {
    switch ( p_stt ) {
    case Enabled:
        break;

    case Disabled:
        break;

    case Undefined:
    default:
        break;
    }

    return Undefined;
}

bool Agent::isActive() const {
    return state() == Enabled;
}

Agent::~Agent() {

}

#include "agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
