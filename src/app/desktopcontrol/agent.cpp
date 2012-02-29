/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "agent.hpp"

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

Agent* Agent::_instance = 0;

Agent::Agent(QObject* parent) : QObject(parent)
{
  _asr = new DesktopASR(DesktopASR::getStandardDescription(), parent);
  connect(this, SIGNAL(started()), _asr, SLOT(run()));
  connect(this, SIGNAL(stopped()), _asr, SLOT(stop()));
  connect(_asr, SIGNAL(finished(QString&)), this, SLOT(invokeCommand(QString&)));
}

// Agent::Agent(const Agent& p_other) : QObject(p_other.parent()),
//     _asr(p_other._asr)
// {
// 
// }

void Agent::setup()
{
    if (!_instance)
        _instance = new Agent();
}

Agent::~Agent()
{
    
}

bool Agent::isActive() const
{
    return _asr->isRunning();
}

Agent* Agent::instance()
{
  return _instance;
}

void Agent::start()
{
    if(_asr->run())
        emit started();
    else {
        qWarning() << "[DesktopControl::Agent] Start unsuccessful.";
    }
}

void Agent::stop()
{
    _asr->stop();
    emit stopped();
}

/// @todo That's bigger issue, we have to make a good design.
void Agent::invokeCommand (QString& command)
{

}
