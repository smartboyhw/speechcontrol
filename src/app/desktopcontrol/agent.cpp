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
#include "command.hpp"

namespace SpeechControl
{
namespace DesktopControl
{

Agent* Agent::s_instance = 0;

Agent::Agent() : AbstractAgent (AbstractCategory::global())
{
    _asr = new DesktopASR (DesktopASR::getStandardDescription(), parent());
    connect (this, SIGNAL (stopped()), _asr, SLOT (stop()));
    connect (_asr, SIGNAL (finished (QString&)), this, SLOT (invokeCommand (QString&)));

}

Agent::~Agent()
{

}

Agent* Agent::instance()
{
    if (s_instance == 0)
        s_instance = new Agent;

    return s_instance;
}

AbstractAgent::OperationState Agent::onStateChanged (const AbstractAgent::OperationState p_state)
{
    switch (p_state) {
    case Enabled: {
        if (!_asr->run())
            qWarning() << "[DesktopControl::Agent] Start unsuccessful.";

    }
    break;

    case Disabled:
        _asr->stop();
        break;

    case Undefined:
    default:
        break;
    }

    return Undefined;
}

bool Agent::isActive() const
{
    return _asr->isRunning();
}

void Agent::invokeCommand (QString& cmd)
{

}

}
}
#include "../desktopcontrol/agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
