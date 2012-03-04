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
#include "sphinx.hpp"

namespace SpeechControl
{
namespace DesktopControl
{

Agent* Agent::s_instance = 0;

<<<<<<< HEAD
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
=======
Agent::Agent() : AbstractAgent ( AbstractCategory::global() ) {
    m_sphinx = new Sphinx ( Sphinx::standardDescription(), parent() );
    connect ( m_sphinx, SIGNAL ( finished ( QString ) ), this, SLOT ( invokeCommand ( QString ) ) );
}

Agent* Agent::instance() {
    if ( s_instance == 0 ) {
>>>>>>> 3be7f4875b010f24d8041104bb49f5b1c63e8548
        s_instance = new Agent;
    }

    return s_instance;
}

AbstractAgent::OperationState Agent::onStateChanged (const AbstractAgent::OperationState p_state)
{
    switch (p_state) {
    case Enabled: {
<<<<<<< HEAD
        if (!_asr->run())
=======
        if ( !isEnabled() )
            return Disabled;

        if ( !m_sphinx->start() ) {
>>>>>>> 3be7f4875b010f24d8041104bb49f5b1c63e8548
            qWarning() << "[DesktopControl::Agent] Start unsuccessful.";
            return Disabled;
        }

    }
<<<<<<< HEAD
    break;

    case Disabled:
        _asr->stop();
=======
    return Enabled;
    break;

    case Disabled:
        m_sphinx->stop();
        return Disabled;
>>>>>>> 3be7f4875b010f24d8041104bb49f5b1c63e8548
        break;

    case Undefined:
    default:
        break;
    }

    return Undefined;
}

<<<<<<< HEAD
bool Agent::isActive() const
{
    return _asr->isRunning();
=======
bool Agent::isActive() const {
    return m_sphinx->isRunning();
}

bool Agent::isEnabled() {
    return Core::configuration ( "DesktopControl/Enabled" ).toBool();
}

/// @todo Since this returns more than one command, should we provide a UI that allows you to pick which command you might want?
void Agent::invokeCommand ( const QString& p_cmd ) {
    AbstractCategory* l_glbl = AbstractCategory::global();
    CommandList l_cmds = l_glbl->matchAllCommands ( p_cmd );

    Q_FOREACH ( AbstractCommand* l_cmd, l_cmds ) {
        qDebug() << l_cmd->id() << l_cmd->statements();
    }
>>>>>>> 3be7f4875b010f24d8041104bb49f5b1c63e8548
}

void Agent::invokeCommand (QString& cmd)
{

}

}
}
<<<<<<< HEAD
#include "../desktopcontrol/agent.moc"
=======
#include "desktopcontrol/agent.moc"
>>>>>>> 3be7f4875b010f24d8041104bb49f5b1c63e8548
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
