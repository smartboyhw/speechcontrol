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

#include "core.hpp"
#include "agent.hpp"
#include "xdo.hpp"
#include "sphinx.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Dictation;

Agent* Agent::s_inst = 0;

Agent::Agent() : AbstractAgent (KeyboardEmulator::instance()), m_sphinx (0)
{
}

AbstractAgent::ActivityState Agent::onStateChanged (const AbstractAgent::ActivityState p_stt)
{
    switch (p_stt) {
    case Enabled:

        if (!m_sphinx) {
            m_sphinx = new Sphinx (Sphinx::standardDescription(), parent());
            connect (m_sphinx, SIGNAL (finished (QString)), this, SLOT (handleText (QString)));
        }

        if (!m_sphinx->start()) {
            qWarning() << "[Dictation::Agent::onStateChanged()] Start unsuccessful.";
            return AbstractAgent::Disabled;
        }
        else {
            qDebug() << "[Dictation::Agent::onStateChanged()] Enabled.";
        }

        return AbstractAgent::Enabled;
        break;

    case Disabled:

        if (m_sphinx) {
            m_sphinx->stop();
            delete m_sphinx;
            qDebug() << "[DesktopControl::Agent::onStateChanged()] Disabled.";
        }

        break;

    case Undefined:
    default:
        break;
    }

    return AbstractAgent::Undefined;
}

bool Agent::isActive() const
{
    return state() == AbstractAgent::Enabled;
}

bool Agent::isEnabled() const
{
    return Core::configuration ("Dictation/Enabled").toBool() == true;
}

bool Agent::isSafetyModeActive() const
{
    return m_mode == Active;
}

bool Agent::isSafetyModeEnabled() const
{
    return Core::configuration ("Dictation/UseSafetyWords").toBool();
}

Agent::SafetyMode Agent::safetyMode() const
{
    return m_mode;
}

void Agent::setSafetyMode (const Agent::SafetyMode& p_mode)
{
    switch (p_mode) {
    case Enabled:
    case Disabled:
        Core::setConfiguration ("Dictation/UseSafetyWords", ( (p_mode == Enabled) ? true : false));
        qDebug() << "[Dictation::Agent::setSafetyMode()] Is safety mode enabled? " << (p_mode == Enabled);
        break;

    default:
        break;
    }

    m_mode = p_mode;
}

void Agent::handleText (const QString& p_text)
{
    qDebug() << "[Dictation::Agent::handleText()] Got text from Sphinx:" << p_text;

    if (!handleSafetyMode (p_text))
        return;

    KeyboardEmulator::instance()->sendPhrase (p_text);
}

bool Agent::handleSafetyMode (const QString& p_text) const
{
    if (!isSafetyModeEnabled())
        return true;

    const QString startWord = Core::configuration ("Dictation/StartWord").toString();
    const QString endWord   = Core::configuration ("Dictation/EndWord").toString();

    if (p_text == startWord || p_text == endWord) {
        if (p_text == startWord) {
            m_mode = Agent::Active;
        }
        else if (p_text == endWord) {
            m_mode = Agent::Inactive;
        }

        return false;
    }
    else {
        if (isSafetyModeActive()) {
            qDebug() << "[Dictation::Agent::handleText()] Text " << p_text << "ignored since safety mode is active.";
            return false;
        }
    }

    return true;
}

Agent::~Agent()
{
}


#include "services/dictation/agent.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
