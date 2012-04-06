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

#include "module.hxx"
#include "module.hpp"

using namespace SpeechControl::Services;

AbstractModule::AbstractModule (QObject* p_parent) : QObject (p_parent),
    d_ptr (new AbstractModulePrivate (this))
{

}

AbstractModule::AbstractModule (AbstractModulePrivate* p_dd, QObject* p_parent) :
    QObject (p_parent), d_ptr (p_dd)
{

}

void AbstractModule::setState (const AbstractModule::ActivityState p_state)
{
    Q_D (AbstractModule);
    d->changeState (p_state);
}

bool AbstractModule::isActive() const
{
    return state() == Enabled;
}

void AbstractModule::start()
{
    if (!isActive()) {
        initialize();
        setState (Enabled);
    }
}

void AbstractModule::stop()
{
    if (isActive()) {
        deinitialize();
        setState (Disabled);
    }
}

AbstractModule::ActivityState AbstractModule::state() const
{
    Q_D (const AbstractModule);
    return d->m_state;
}


AbstractModule::~AbstractModule()
{

}

#include "services/module.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
