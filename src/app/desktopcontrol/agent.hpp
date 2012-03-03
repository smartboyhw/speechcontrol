/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *            (C) 2012 Jacky Alcine <jacky.alcine@thesii.org>
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
 *
 */

#ifndef DESKTOPCONTROL_AGENT_HPP
#define DESKTOPCONTROL_AGENT_HPP

// Qt
#include <QObject>

// Local
#include "desktopasr.hpp"
#include <app/abstractagent.hpp>

namespace SpeechControl {
/**
 * @brief Container namespace for all of the desktop control related clases.
 **/
namespace DesktopControl {
/**
 * @brief Agent managing desktop control.
 *
 * The desktop control system revolves (in a hierarchical manner) around the
 * agent; if it's not active, no desktop control can occur.
 * The agent provides an API for enabling our desktop control facilities.
 *
 * @section about-dsktpcntrl About DesktopControl
 * Desktop Control is a technology of management using user's voice, it is one of the
 * main SpeechControl's features. User has basically control over every aspect of work
 * with his computer. Will it be creating/removing files, opening programs and files, starting new
 * IM session, etc.
 *
 * @see AbstractAgent
 **/
class Agent : public AbstractAgent {
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     **/
    Agent();
    virtual ~Agent();

    /**
     * @brief Determines the active status of desktop control.
     * @overload Determines the active status of desktop control.
     * @return bool
     **/
    virtual bool isActive() const;

    /**
     * @brief Determines if the Desktop Control agent has been enabled or disabled by the user.
     * @return TRUE if it has been enabled, FALSE if it's disabled.
     **/
    bool isEnabled();

    /**
     * @brief Pointer to agent instance.
     *
     * A means of obtaining a pointer to the desktop control class.
     * If it doesn't exist, it's created.
     *
     * @return A pointer to the desktop control class's singleton instance.
     **/
    static Agent* instance();

public slots:
    /**
     * @brief Invokes a command for desktop control to parse.
     *
     * Takes a string p_cmd as the argument spoken by the user
     * and searches all of the commands available to the system
     * to find a match (or matches) to invoke said commands.
     *
     * @param p_cmd The command to be parsed.
     **/
    void invokeCommand ( const QString& p_cmd );

private:
    virtual OperationState onStateChanged ( const AbstractAgent::OperationState p_state );
    static Agent* s_instance;
    DesktopASR* _asr;
};
}
}

#endif // DESKTOPCONTROL_AGENT_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
