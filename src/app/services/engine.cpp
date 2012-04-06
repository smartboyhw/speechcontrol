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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "module.hpp"
#include "engine.hpp"

using namespace SpeechControl::Services;

QMap<QString, AbstractModule*> Engine::s_list;
Engine* Engine::s_inst = 0;

Engine::Engine() : QObject()
{
}

Engine::Engine (const Engine& p_other) : QObject (p_other.parent())
{

}

ModuleList Engine::allModules()
{
    return s_list.values();
}

void Engine::registerModule (AbstractModule* p_module)
{
    if (!findModule (p_module->id())) {
        s_list.insert (p_module->id(), p_module);
    }
}

void Engine::unregisterModule (AbstractModule* p_module)
{
    if (findModule (p_module->id())) {
        if (p_module->isActive())
            p_module->stop();

        s_list.remove (p_module->id());
    }
}

AbstractModule* Engine::findModule (const QString& p_id)
{
    return s_list.value (p_id);
}

void Engine::start()
{
    emit instance()->started();
}

void Engine::stop()
{
    emit instance()->stopped();
}


#include "engine.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
