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
#include "dictation.hpp"

using SpeechControl::Core;
using SpeechControl::AbstractAgent;
using SpeechControl::DictationAgent;

DictationAgent* DictationAgent::s_inst = 0;

DictationAgent::DictationAgent() : AbstractAgent ( Core::instance() ) {

}

DictationAgent* DictationAgent::instance() {
    if ( s_inst == 0 )
        s_inst = new DictationAgent;

    return s_inst;
}

AbstractAgent::OperationState DictationAgent::onStateChanged ( const AbstractAgent::OperationState p_stt ) {
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

bool DictationAgent::isActive() const {
    return state() == Enabled;
}

DictationAgent::~DictationAgent() {

}

#include "dictation.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
