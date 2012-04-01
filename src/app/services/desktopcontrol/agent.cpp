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

#include <lib/acousticmodel.hpp>

#include "core.hpp"
#include "command.hpp"
#include "sphinx.hpp"

#include "agent.hpp"

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

Agent* Agent::s_inst = 0;

Agent::Agent() : AbstractAgent (AbstractCategory::global())
{
    m_sphinx = new Sphinx (Sphinx::standardDescription(), parent());
    connect (m_sphinx, SIGNAL (finished (QString)), this, SLOT (invokeCommand (QString)));

    QString defAcousticModel = Core::configuration ("DesktopControl/DefaultAcousticModel").toString();

    if (!defAcousticModel.isEmpty())
        m_sphinx->setAcousticModel (defAcousticModel);
}

Agent::~Agent()
{

}

void Agent::start()
{
    if (!m_sphinx->start()) {
        qWarning() << "[DesktopControl::Agent::onStateChanged()] Start unsuccessful.";
//         return Disabled;
    }

    qDebug() << "[DesktopControl::Agent::onStateChanged()] Enabled.";
//     return Enabled;
}

void Agent::stop()
{
    if (!m_sphinx->stop()) {
        qWarning() << "[DesktopControl::Agent::onStateChanged()] Stop unsuccessful.";
//         return Enabled;
    }

    qDebug() << "[DesktopControl::Agent::onStateChanged()] Stopped desktop control agent.";
//     return Disabled;
}

AbstractAgent::ActivityState Agent::onStateChanged (const AbstractAgent::ActivityState p_state)
{
    switch (p_state) {
    case Enabled:

        if (!m_sphinx->start()) {
            qWarning() << "[DesktopControl::Agent::onStateChanged()] Start unsuccessful.";
            return Disabled;
        }
        else {
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

void Agent::setAcousticModel (AcousticModel* acModel)
{
    m_sphinx->setAcousticModel (acModel->path());
}

void Agent::setDefaultAcousticModel (AcousticModel* acModel)
{
    Core::setConfiguration ("DesktopControl/DefaultAcousticModel", acModel->path());
}

void Agent::invokeCommand (const QString& cmd)
{
    qDebug() << "[DesktopControl::Agent::invokeCommand()] I heard " << cmd << "from the user.";
    AbstractCategory* glbl = AbstractCategory::global();
    CommandList cmds = glbl->matchAllCommands (cmd);

    if (!cmds.isEmpty()) {
        if (cmds.count() == 1) {
            AbstractCommand* onlyCmd = cmds.first();
            emit commandFound (cmd, onlyCmd);
            onlyCmd->invoke (cmd);
        }
        else {
            emit multipleCommandsFound (cmd, cmds);
        }

        Q_FOREACH (AbstractCommand * cmd, cmds) {
            qDebug() << "[DesktopControl::Agent::invokeCommand()] Command " << cmd->id() << "matched with statements" << cmd->statements();
        }

    }
    else {
        emit noCommandsFound (cmd);
        qDebug() << "[DesktopControl::Agent::invokeCommand()] I heard mumble-gumble, nothing useful. Tell me something I want to hear!";
    }
}

#include "services/desktopcontrol/agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
