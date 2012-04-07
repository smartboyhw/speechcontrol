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

#include "app/core.hpp"
#include "app/services/dictation/xdo.hpp"
#include "app/services/dictation/service.hxx"

using SpeechControl::Core;
using namespace SpeechControl::Dictation;

ServicePrivate::ServicePrivate (Service* p_qPtr) :
    AbstractModulePrivate (p_qPtr), m_safetyMode (Service::Active),
    m_sphinx (0)
{

}

AbstractModule::ActivityState ServicePrivate::handleStateChange (const AbstractModule::ActivityState p_state)
{
    Q_Q (Service);

    switch (p_stt) {
    case AbstractModule::Enabled:

        if (!m_sphinx) {
            m_sphinx = new Sphinx (Sphinx::standardDescription(), q);
            q->connect (m_sphinx, SIGNAL (finished (QString)), SLOT (handleText (QString)));
        }

        if (!m_sphinx->start()) {
            qWarning() << "[DictationPrivate::Service::onStateChanged()] Start unsuccessful.";
            return AbstractModule::Disabled;
        }
        else {
            qDebug() << "[DictationPrivate::Service::onStateChanged()] Enabled.";
            q->handleText("This is Jacky messing around with SpeechControl.");
        }

        return AbstractModule::Enabled;
        break;

    case AbstractModule::Disabled:

        if (m_sphinx) {
            m_sphinx->stop();
            delete m_sphinx;
            qDebug() << "[DictationPrivate::Service::onStateChanged()] Disabled.";
        }

        break;

    case AbstractModule::Undefined:
    default:
        break;
    }

    return AbstractModule::Undefined;
}

Service::SafetyMode ServicePrivate::safetyMode() const
{
    return m_safetyMode;
}

void ServicePrivate::setSafetyMode (const Service::SafetyMode& p_mode)
{
    m_safetyMode = p_mode;
}

ServicePrivate::~ServicePrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
