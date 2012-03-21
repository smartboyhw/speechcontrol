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

Agent* Agent::s_inst = 0;

Agent::Agent() : AbstractAgent (AbstractCategory::global()), m_sphinx (0)
{
}

Agent::~Agent()
{

}

AbstractAgent::ActivityState Agent::onStateChanged (const AbstractAgent::ActivityState p_state)
{
    switch (p_state) {
    case Enabled:

        if (!m_sphinx) {
            m_sphinx = new Sphinx (Sphinx::standardDescription(), parent());
            connect (m_sphinx, SIGNAL (finished (QString)), this, SLOT (invokeCommand (QString)));
        }

        if (!m_sphinx->start()) {
            qWarning() << "[DesktopControl::Agent::onStateChanged()] Start unsuccessful.";
            return Disabled;
        } else {
            qDebug() << "[DesktopControl::Agent::onStateChanged()] Enabled.";
        }

        return Enabled;
        break;

    case Disabled:

        if (m_sphinx) {
            if (!m_sphinx->stop()) {
                qWarning() << "[DesktopControl::Agent::onStateChanged()] Stop unsuccessful.";
                return Enabled;
            }

            qDebug() << "[DesktopControl::Agent::onStateChanged()] Stopped desktop control agent.";
            delete m_sphinx;
        }

        return Disabled;
        break;

    default:
        return Undefined;
    }

}

bool Agent::isActive() const
{
    return m_sphinx && m_sphinx->isRunning();
}

bool Agent::isEnabled()
{
    return Core::configuration ("DesktopControl/Enabled").toBool();
}

/// @todo Since this returns more than one command, should we provide a UI that allows you to pick which command you might want?
void Agent::invokeCommand (const QString& cmd)
{
    qDebug() << "[DesktopControl::Agent::invokeCommand()] I heard " << cmd << "from the user.";
    AbstractCategory* l_glbl = AbstractCategory::global();
    CommandList l_cmds = l_glbl->matchAllCommands (cmd);

    if (!l_cmds.isEmpty()) {
        Q_FOREACH (AbstractCommand * l_cmd, l_cmds) {
            qDebug() << "[DesktopControl::Agent::invokeCommand()] Command " << l_cmd->id() << "matched with statements" << l_cmd->statements();

            if (l_cmd == l_cmds.first()) {
                qDebug() << "[DesktopControl::Agent::invokeCommand()] Only invoking first command " << l_cmd->id();
                l_cmd->invoke (cmd);
            }
        }
    }
    else {
        qDebug() << "[DesktopControl::Agent::invokeCommand()] I heard mumble-gumble, nothing useful. Tell me something I want to hear!";
    }
}

}
}
#include "desktopcontrol/agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
