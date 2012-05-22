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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 14:49:01 PM
 */

#include <QDebug>
#include "moduleprivate.hpp"
#include "module.hpp"

SPCHCNTRL_USE_NAMESPACE

AbstractModulePrivate::AbstractModulePrivate (AbstractServiceModule* p_qPtr) :
    q_ptr (p_qPtr), m_state (AbstractServiceModule::Undefined)
{

}

AbstractModulePrivate::~AbstractModulePrivate()
{

}

AbstractServiceModule::AbstractServiceModule (QObject* p_parent) : QObject (p_parent),
    d_ptr (0)
{

}

AbstractServiceModule::AbstractServiceModule (AbstractModulePrivate* p_dd, QObject* p_parent) :
    QObject (p_parent), d_ptr (p_dd)
{

}

void AbstractServiceModule::setState (const AbstractServiceModule::ActivityState p_state)
{
    Q_D (AbstractModule);
    qDebug() << "[AbstractModule::setState()] State changing to " << p_state << "...";
    d->changeState (p_state);

    if (p_state == state())
        emit stateChanged (p_state);
}

bool AbstractServiceModule::isActive() const
{
    return state() == Enabled;
}

void AbstractServiceModule::start()
{
    if (!isActive()) {
        qDebug() << "[AbstractModule::start()] Initializing...";
        initialize();
        qDebug() << "[AbstractModule::start()] Enabling module...";
        setState (Enabled);

        if (state() == Enabled) {
            qDebug() << "[AbstractModule::start()] Module enabled.";
        }
        else {
            qDebug() << "[AbstractModule::start()] Failed to enable module.";
        }
    }
}

void AbstractServiceModule::stop()
{
    if (isActive()) {
        qDebug() << "[AbstractModule::stop()] Initializing...";
        deinitialize();
        qDebug() << "[AbstractModule::stop()] Disabling module...";
        setState (Disabled);

        if (state() == Disabled) {
            qDebug() << "[AbstractModule::stop()] Module disabled.";
        }
        else {
            qDebug() << "[AbstractModule::stop()] Failed to disable module.";
        }
    }
}

AbstractServiceModule::ActivityState AbstractServiceModule::state() const
{
    Q_D (const AbstractModule);
    return d->m_state;
}


AbstractServiceModule::~AbstractServiceModule()
{

}

#include "services/module.moc"
// kate: indent-mode cstyle; replace-tabs on;
