/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QIcon>

#include <lib/acousticmodel.hpp>
#include <lib/languagemodel.hpp>

#include "app/core.hpp"
#include "app/services/module.hpp"
#include "app/services/engine.hpp"
#include "app/services/desktopcontrol/sphinx.hpp"
#include "app/services/desktopcontrol/service.hxx"
#include "app/services/desktopcontrol/service.hpp"

using SpeechControl::Core;
using namespace SpeechControl::Services;
using namespace SpeechControl::DesktopControl;
Service* Service::s_inst = 0;

Service::Service() : AbstractModule (new ServicePrivate (this))
{
    Q_D (Service);
    connect (d->m_sphinx, SIGNAL (finished (QString)), this, SLOT (invokeCommand (QString)));

    Services::Engine::registerModule (this);
}

void Service::deinitialize() {}

void Service::initialize() {}

bool Service::isEnabled() const
{
    return Core::configuration ("DesktopControl/Enabled").toBool();
}

QString Service::id() const
{
    return "dsktpctl";
}

QPixmap Service::pixmap() const
{
    return QIcon::fromTheme ("audio-input-headset").pixmap (64, 64);
}

QString Service::name() const
{
    return "Desktop Control";
}

bool Service::isActive() const
{
    Q_D (const Service);
    return d->m_sphinx && d->m_sphinx->isRunning();
}

void Service::setAcousticModel (const AcousticModel& p_acModel)
{
    Q_D (Service);
    d->m_sphinx->setAcousticModel (p_acModel.path());
}

void Service::setDefaultAcousticModel (const AcousticModel& p_acModel)
{
    Core::setConfiguration ("DesktopControl/AcousticModel", p_acModel.path());
    setAcousticModel (p_acModel);
}

void Service::invokeCommand (const QString& cmd)
{
    qDebug() << "[DesktopControl::Service::invokeCommand()] Heard " << cmd << "from the user.";
    AbstractCategory* glbl = AbstractCategory::global();
    CommandList commands = glbl->matchAllCommands (cmd);

    if (!commands.isEmpty()) {
        if (commands.count() == 1) {
            AbstractCommand* onlyCommand = commands.first();
            qDebug() << "[DesktopControl::Service::invokeCommand()] Only matched a command " << onlyCommand->id() << "matched with statements" << onlyCommand->statements();
            emit commandFound (cmd, onlyCommand);
            onlyCommand->invoke (cmd);
            return;
        }
        else {
            emit multipleCommandsFound (cmd, commands);

            Q_FOREACH (AbstractCommand * command, commands) {
                qDebug() << "[DesktopControl::Service::invokeCommand()] Command " << command->id() << "matched with statements" << command->statements();
            }
        }

    }
    else {
        emit noCommandsFound (cmd);
        qDebug() << "[DesktopControl::Service::invokeCommand()] Heard mumble-gumble, nothing useful.";
    }
}

Service::~Service()
{

}

#include "services/desktopcontrol/service.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
