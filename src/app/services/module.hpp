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
#ifndef SPCHCNTRL_SERVICES_MODULE_HPP
#define SPCHCNTRL_SERVICES_MODULE_HPP

#include <QObject>
#include <QList>
#include <QPixmap>
#include <app/global.hpp>

SPCHCNTRL_BEGIN_NAMESPACE

class AbstractServiceModule;
class AbstractModulePrivate;
/**
 * @brief Represents a list of Modules.
 **/
typedef QList<AbstractServiceModule*> ModuleList;

/**
 * @brief Represents a service module.
 *
 * Modules serve as wrappers to the interfaces that internal and external components
 * of SpeechControl can provide.
 **/
class AbstractServiceModule : public QObject
{
    Q_OBJECT
    Q_ENUMS (ActivityState)

public:
    /**
     * @brief Defines the possible operational states of an AbstractAgent.
     *
     * States allow agents to publicly express what mode of activity that
     * the agent is currently in. They can be either in the Enabled or Disabled
     * state. The Undefined state is provided for mediation purposes and may be
     * removed in a future release.
     **/
    enum ActivityState {

        Undefined = -1, /** Defines the state of the agent as undefined.
        */

        Disabled,       /** Defines the state of the agent as disabled.
        *  All activity defined by the agent should be halted if active.
        */

        Enabled         /** Defines the state of the agent as enabled.
        *  All activity defined by the agent should be activated.
        */
    };

    /**
     * Obtains the current state of the Agent.
     * @return ActivityState
     * @see ActivityState
     **/
    ActivityState state() const;

    /**
     * @brief Obtains the friendly name of this AbstractModule.
     **/
    virtual QString name() const = 0;

    /**
     * @brief Obtains the ID of the AbstractModule.
     **/
    virtual QString id() const = 0;

    /**
     * @brief Obtains the QPixmap to use with this AbstractModule.
     **/
    virtual QPixmap pixmap() const = 0;

    /**
     * @brief Determines if this AbstractModule is enabled.
     **/
    virtual bool isEnabled() const = 0;

    /**
     * @brief Determines if this AbstractModule is active.
     **/
    virtual bool isActive() const;

signals:
    /**
     * Emitted when the state of this Agent is set to the Enabled state.
     * @return void
     * @see ActivityState::Enabled
     **/
    void enabled();

    /**
     * Emitted when the state of this Agent is set to the Disabled state.
     * @return void
     * @see ActivityState::Disabled
     **/
    void disabled();

    /**
     * Emitted when the state of this Agent has changed to an arbitrary state.
     * @return void
     * @see ActivityState
     **/
    void stateChanged (const ActivityState& p_state);

public slots:
    /**
     * @brief Starts this AbstractModule.
     **/
    void start();

    /**
     * @brief Stops this AbstractModule.
     **/
    void stop();

    /**
     * Changes the state of this Agent to the specified state.
     * @return void
     * @see ActivityState
     **/
    void setState (const ActivityState p_state);
protected:
    QScopedPointer<AbstractModulePrivate> d_ptr;
    Q_DISABLE_COPY (AbstractServiceModule)
    Q_DECLARE_PRIVATE (AbstractModule)

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;
    explicit AbstractServiceModule (QObject* parent = 0);
    AbstractServiceModule (AbstractModulePrivate* p_dd, QObject* p_parent = 0);
    virtual ~AbstractServiceModule();
};

SPCHCNTRL_END_NAMESPACE

#endif // SPEECHCONTROL_SERVICES_MODULE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
