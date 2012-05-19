/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 22:21:00 PM
 */

#include "app/core.hpp"
#include "app/services/module.hpp"
#include "app/services/engineprivate.hpp"
#include "app/services/engine.hpp"

SPCHCNTRL_USE_NAMESPACE

QMap<QString, AbstractServiceModule*> ServiceEnginePrivate::s_list;
ServiceEngine* ServiceEngine::s_inst = 0;

ServiceEngine::ServiceEngine() : QObject (Core::instance()), d_ptr (new ServiceEnginePrivate)
{
}

ServiceEngine::ServiceEngine (const ServiceEngine& p_other) : QObject (p_other.parent()), d_ptr (const_cast<ServiceEnginePrivate*> (p_other.d_ptr.data()))
{

}

ServiceModuleList ServiceEngine::allModules()
{
    return ServiceEnginePrivate::s_list.values();
}

void ServiceEngine::registerModule (AbstractServiceModule* p_module)
{
    if (!findModule (p_module->id())) {
        ServiceEnginePrivate::s_list.insert (p_module->id(), p_module);
    }
}

void ServiceEngine::unregisterModule (AbstractServiceModule* p_module)
{
    if (findModule (p_module->id())) {
        if (p_module->isActive())
            p_module->stop();

        ServiceEnginePrivate::s_list.remove (p_module->id());
    }
}

AbstractServiceModule* ServiceEngine::findModule (const QString& p_id)
{
    return ServiceEnginePrivate::s_list.value (p_id);
}

void ServiceEngine::start()
{
    Q_FOREACH(AbstractServiceModule* module, allModules()){
        module->start();
    }

    emit instance()->started();
}

void ServiceEngine::stop()
{
    Q_FOREACH(AbstractServiceModule* module, allModules()){
        module->stop();
    }

    emit instance()->stopped();
}


#include "services/engine.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
