/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef AGENT_HPP
#define AGENT_HPP

// Qt
#include <QObject>

// Local
#include "desktopasr.hpp"

/**
 * @brief Namespace containing all of SpeechControl's definitions.
 **/

namespace SpeechControl
{
/**
 * @brief Namespace containing the Desktop Control utilities.
 **/

namespace DesktopControl {

class Agent;

/**
 * @brief Agent providing desktop control.
 * DesktopControl::Agent provides an API for enabling our desktop control facilities.
 * 
 * Desktop Control is a technology of management using user's voice, it is one of the
 * main SpeechControl's features. User has basically control over every aspect of work
 * with his computer. Will it be creating/removing files, opening programs and files, starting new
 * IM session, etc.
 */

class Agent : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Agent)

private:
    static Agent* _instance;
    DesktopASR* _asr;
    
public:
    /**
     * @brief ...
     *
     * @return void
     **/
    explicit Agent (QObject* parent = 0);
    
    /**
     * @brief ...
     *
     **/
    virtual ~Agent();

    /**
     * @brief ...
     *
     * @return void
     **/
    void start();
    
    /**
     * @brief ...
     *
     * @return void
     **/
    void stop();
    
    /**
     * @brief ...
     *
     * @return :DesktopControl::Agent*
     **/
    static Agent* instance();
    
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isActive() const;

signals:
    /**
     * @brief ...
     *
     * @return void
     **/
    void started();
    
    /**
     * @brief ...
     *
     * @return void
     **/
    void stopped();
    
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    void stateChanged (bool);
    
public slots:
    /**
     * @brief ...
     *
     * @param  ...
     * @return void
     **/
    static void invokeCommand (QString& command);
};

}
}

#endif // AGENT_H
