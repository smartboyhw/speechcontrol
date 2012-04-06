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

#ifndef DICTATION_AGENT_HPP
#define DICTATION_AGENT_HPP

#include <QUuid>
#include <QObject>

#include <abstractagent.hpp>
#include <macros.hpp>

namespace SpeechControl
{
namespace Dictation
{
class Sphinx;
class Agent : public AbstractAgent
{
    Q_OBJECT
    SC_SINGLETON (Agent)
public:
    enum SafetyMode {
        Undefined = -1,
        Disabled,
        Enabled,
        Active,
        Inactive
    };
    virtual ~Agent();
    SafetyMode safetyMode() const;
    virtual bool isActive() const;
    bool isSafetyModeActive() const;
    bool isSafetyModeEnabled() const;
    bool isEnabled() const;
    void setSafetyMode(const SafetyMode& p_mode);

public slots:
    void handleText (const QString& p_text);

private:
    virtual ActivityState onStateChanged (const SpeechControl::AbstractAgent::ActivityState p_stt);
    Sphinx* m_sphinx;
    SafetyMode m_mode;
};
}
}

#endif // DICTATION_AGENT_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
