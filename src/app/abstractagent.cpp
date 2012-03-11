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

#include "abstractagent.hpp"

using SpeechControl::AbstractAgent;

AbstractAgent::AbstractAgent (QObject* p_parent) : QObject (p_parent)
{
}

AbstractAgent::AbstractAgent (const AbstractAgent&  p_agent) : QObject (p_agent.parent()),
    m_state (p_agent.m_state)
{

}

void AbstractAgent::setState (const AbstractAgent::ActivityState p_state)
{
    if (p_state == m_state)
        return;

    m_state = onStateChanged (p_state);
    emit stateChanged (m_state);
}

void AbstractAgent::start() {
    setState(Enabled);
}

void AbstractAgent::stop() {
    setState(Disabled);
}

AbstractAgent::ActivityState AbstractAgent::state() const
{
    return m_state;
}

AbstractAgent::~AbstractAgent()
{
}

#include "abstractagent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
