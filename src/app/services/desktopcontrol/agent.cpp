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

#include <QDeclarativeView>

#include <lib/acousticmodel.hpp>
#include <app/ui/main-window.hpp>

#include "core.hpp"
#include "command.hpp"
#include "sphinx.hpp"

#include "agent.hpp"

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

Agent* Agent::s_inst = 0;

Agent::Agent() : AbstractAgent (AbstractCategory::global()), m_view(new QDeclarativeView)
{
    m_sphinx = new Sphinx (Sphinx::standardDescription(), parent());
    connect (m_sphinx, SIGNAL (finished (QString)), this, SLOT (invokeCommand (QString)));

    QString defAcousticModel = Core::configuration ("DesktopControl/AcousticModel").toString();
    QString defLanguageModel = Core::configuration ("DesktopControl/LanguageModel").toString();

    if (!defAcousticModel.isEmpty())
        m_sphinx->setAcousticModel (defAcousticModel);

    if (!defLanguageModel.isEmpty())
        m_sphinx->setLanguageModel (defLanguageModel);

    m_view->setSource(QUrl ("qrc:///qml/dskptctlui"));
}

Agent::~Agent()
{

}

void Agent::start()
{
    if (!m_sphinx->start()) {
        qWarning() << "[DesktopControl::Agent::start()] Start unsuccessful.";
    }

    m_view->show();

    qDebug() << "[DesktopControl::Agent::start()] Enabled.";
}

void Agent::stop()
{
    if (!m_sphinx->stop()) {
        qWarning() << "[DesktopControl::Agent::stop()] Stop unsuccessful.";
    }

    m_view->hide();

    qDebug() << "[DesktopControl::Agent::stop()] Stopped desktop control agent.";
}

AbstractAgent::ActivityState Agent::onStateChanged (const AbstractAgent::ActivityState p_state)
{
    switch (p_state) {
    case Enabled:
        instance()->start();

        if (!m_sphinx->isRunning())
            return Disabled;

        return Enabled;
        break;

    case Disabled:
        instance()->stop();

        if (m_sphinx->isRunning() || m_sphinx->isReady())
            return Enabled;

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
    Core::setConfiguration ("DesktopControl/AcousticModel", acModel->path());
}

void Agent::invokeCommand (const QString& cmd)
{
    qDebug() << "[DesktopControl::Agent::invokeCommand()] Heard " << cmd << "from the user.";
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
        qDebug() << "[DesktopControl::Agent::invokeCommand()] Heard mumble-gumble, nothing useful.";
    }
}

#include "services/desktopcontrol/agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
