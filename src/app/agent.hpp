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
 * Agents are used in SpeechControl to wrap the status and exportability of
 * certain events within it like desktop control and dictation.
 **/
class AbstractAgent : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( AbstractAgent )

public:
    /**
     * @brief Defines the possible operational states of an AbstractAgent.
     **/
    enum OperationState {
        Disabled = -1,
        Undefined,
        Enabled
    };

    /**
     * @brief Constructor.
     * @param p_prnt Defaults to 0.
     **/
    explicit AbstractAgent ( QObject* = 0 );

    /**
     * @brief Destructor.
     **/
    virtual ~AbstractAgent();

    /**
     * @brief ...
     *
     * @return bool
     **/
    virtual bool isActive() const = 0;

    /**
     * Obtains the current state of the Agent.
     * @return State&
     **/
    OperationState state() const;

public slots:
    /**
     * Changes the state of this Agent to the specified state.
     * @return void
     **/
    void setState ( const OperationState );

signals:
    /**
     * Emitted when the state of this Agent is set to the Active state.
     * @return void
     **/
    void activated();

    /**
     * Emitted when the state of this Agent is set to the Inactive state.
     * @return void
     **/
    void inactivated();

    /**
     * Emitted when the state of this Agent has changed to an arbitrary state.
     * @return void
     **/
    void stateChanged ( const OperationState& );

protected:
    /**
     * Handles the actions depending on each of the states changing.
     * @return State The accepted state to render to, or Undefined if this state is invalid.
     **/
    virtual const OperationState onStateChanged ( const OperationState ) = 0;

private:
    OperationState m_stt;
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
