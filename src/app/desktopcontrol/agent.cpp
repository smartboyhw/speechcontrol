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

Agent* Agent::s_inst = 0;

/// @todo #issue 0000032
Agent::Agent(QObject* p_prnt) : QObject(p_prnt)
{
  s_inst = this;
  m_sphnx = new Sphinx;
  connect(this, SIGNAL(started()), m_sphnx, SLOT(startRecognizing()));
  connect(this, SIGNAL(stopped()), m_sphnx, SLOT(stopRecognizing()));
}

Agent::Agent(const Agent& p_other) : QObject(p_other.parent()),
    m_sphnx(p_other.m_sphnx)
{

}

/// @todo Properly designate a means of determing if this agent is active.
const bool Agent::isActive()
{
    return instance()->m_sphnx->isListening();
}

Agent* Agent::instance()
{
  if (!s_inst)
    s_inst = new Agent;

  return s_inst;
}

/// @todo Instead of using Sphinx directly, use the ASR function.
void Agent::start()
{
    instance()->m_sphnx->startRecognizing();
    emit instance()->stateChanged(true);
    emit instance()->started();
}

void Agent::stop()
{
    instance()->m_sphnx->stopRecognizing();
    emit instance()->stateChanged(false);
    emit instance()->stopped();
}

void Agent::invokeCommand ( const QString& p_command )

{

}

Agent::~Agent()
{

}
