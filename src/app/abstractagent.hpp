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

#ifndef ABSTRACTAGENT_HPP
#define ABSTRACTAGENT_HPP

#include <QObject>

namespace SpeechControl {
/**
 * @brief Abstract class representing an executing module within SpeechControl.
 *
 * Agents are used in SpeechControl to wrap the status and exportability of
 * certain events within it like desktop control and dictation. Using agents
 * also permit a basis of toggling specific features of the system on and off.
 *
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 **/
class AbstractAgent : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( AbstractAgent )

public:
    /**
     * @brief Defines the possible operational states of an AbstractAgent.
     *
     * States allow agents to publicly express what mode of activity that
     * the agent is currently in. They can be either in the Enabled or Disabled
     * state. The Undefined state is provided for mediation purposes and may be
     * removed in a future release.
     **/
    enum OperationState {
        Disabled = -1,  ///< Defines the state of the agent as disabled.
        ///< All activity defined by the agent should be halted if active.

        Undefined,      ///< Defines the state of the agent as undefined.

        Enabled         ///< Defines the state of the agent as enabled.
        ///< All activity defined by the agent should be activated.
    };

    /**
     * @brief Constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit AbstractAgent ( QObject* p_parent = 0 );

    /**
     * @brief Destructor.
     **/
    virtual ~AbstractAgent();

    /**
     * @brief Determines active state of the agent.
     *
     * A convenience method as opposed to using the state() method.
     *
     * @return TRUE if state() == Enabled, else FALSE.
     **/
    virtual bool isActive() const = 0;

    /**
     * Obtains the current state of the Agent.
     * @return OperationState
     * @see OperationState
     **/
    OperationState state() const;

public slots:
    /**
     * Changes the state of this Agent to the specified state.
     * @return void
     * @see OperationState
     **/
    void setState ( const OperationState p_state );

signals:
    /**
     * Emitted when the state of this Agent is set to the Enabled state.
     * @return void
     * @see OperationState::Enabled
     **/
    void enabled();

    /**
     * Emitted when the state of this Agent is set to the Disabled state.
     * @return void
     * @see OperationState::Disabled
     **/
    void disabled();

    /**
     * Emitted when the state of this Agent has changed to an arbitrary state.
     * @return void
     * @see OperationState
     **/
    void stateChanged ( const OperationState& p_state );

protected:
    /**
     * Handles the actions depending on each of the states changing.
     * @return State The accepted state to render to, or Undefined if this state is invalid.
     * @see OperationState
     **/
    virtual OperationState onStateChanged ( const OperationState p_state ) = 0;

private:
    OperationState m_state; ///< The state of the agent.
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
