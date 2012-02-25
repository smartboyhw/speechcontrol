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
#include "sphinx.hpp"

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

Agent* Agent::_instance = 0;

/// @todo #issue 0000032
Agent::Agent(QObject* parent) : QObject(parent)
{
  _instance = this;
  m_sphinx = new Sphinx;
  connect(this, SIGNAL(started()), m_sphinx, SLOT(startRecognizing()));
  connect(this, SIGNAL(stopped()), m_sphinx, SLOT(stopRecognizing()));
}

Agent::Agent(const Agent& p_other) : QObject(p_other.parent()),
    m_sphinx(p_other.m_sphinx)
{

}

/// @todo Properly designate a means of determing if this agent is active.
const bool Agent::isActive()
{
    return instance()->m_sphinx->isListening();
}

Agent* Agent::instance()
{
  return _instance;
}

/// @todo Instead of using Sphinx directly, use the ASR function.
void Agent::start()
{
    instance()->m_sphinx->startRecognizing();
    emit instance()->stateChanged(true);
    emit instance()->started();
}

void Agent::stop()
{
    instance()->m_sphinx->stopRecognizing();
    emit instance()->stateChanged(false);
    emit instance()->stopped();
}

/// @todo That's bigger issue, we have to make a good design.
void Agent::invokeCommand (const QString& command)
{

}

Agent::~Agent()
{

}
