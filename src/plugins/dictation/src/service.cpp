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

#include <QIcon>

#include "app/core.hpp"
#include "app/services/engine.hpp"
#include "app/services/module.hpp"
#include "app/services/dictation/xdo.hpp"
#include "app/services/dictation/sphinx.hpp"
#include "app/services/dictation/serviceprivate.hpp"
#include "app/services/dictation/service.hpp"

using SpeechControl::Core;
using namespace SpeechControl::Dictation;
Service* Service::s_inst = 0;

ServicePrivate::ServicePrivate (Service* p_qPtr) :
AbstractModulePrivate (p_qPtr), m_safetyMode (Service::Active),
m_sphinx (0)
{

}

AbstractModule::ActivityState ServicePrivate::handleStateChange (const AbstractModule::ActivityState p_state)
{
    Q_Q (Service);

    switch (p_state) {
        case AbstractModule::Enabled:

            if (!m_sphinx) {
                m_sphinx = new Sphinx (Sphinx::standardDescription(), q);
                q->connect (m_sphinx, SIGNAL (finished (QString)), SLOT (handleText (QString)));
            }

            if (!m_sphinx->start()) {
                qWarning() << "[Dictation::ServicePrivate::onStateChanged()] Start unsuccessful.";
                return AbstractModule::Disabled;
            }
            else {
                qDebug() << "[Dictation::ServicePrivate::onStateChanged()] Enabled.";
            }

            return AbstractModule::Enabled;
            break;

        case AbstractModule::Disabled:
            if (m_sphinx) {
                m_sphinx->stop();
                m_sphinx = 0;
                qDebug() << "[Dictation::ServicePrivate::onStateChanged()] Disabled.";
            }

            break;

        case AbstractModule::Undefined:
        default:
            break;
    }

    return AbstractModule::Undefined;
}

void ServicePrivate::changeState (AbstractModule::ActivityState p_state)
{
    m_state = handleStateChange(p_state);
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

Service::Service() : AbstractServiceModule (new ServicePrivate (this), KeyboardEmulator::instance())
{
    Services::Engine::registerModule (this);
}

void Service::deinitialize()
{
}

void Service::initialize()
{
}

bool Service::isEnabled() const
{
    return Core::configuration ("Dictation/Enabled").toBool() == true;
}

QString Service::id() const
{
    return "dctn";
}

QPixmap Service::pixmap() const
{
    return QIcon::fromTheme ("audio-input").pixmap (64, 64);
}

QString Service::name() const
{
    return "Dictation";
}

bool Service::isSafetyModeActive() const
{
    Q_D (const Service);
    return d->safetyMode() == Active;
}

bool Service::isSafetyModeEnabled() const
{
    return Core::configuration ("Dictation/UseSafetyWords").toBool();
}

Service::SafetyMode Service::safetyMode() const
{
    Q_D (const Service);
    return d->safetyMode();
}

void Service::setSafetyMode (const Service::SafetyMode& p_mode)
{
    Q_D (Service);

    switch (p_mode) {
    case Service::Enabled:
    case Service::Disabled:
        Core::setConfiguration ("Dictation/UseSafetyWords", ( (p_mode == Service::Enabled) ? true : false));
        qDebug() << "[Dictation::Service::setSafetyMode()] Is safety mode enabled? " << (p_mode == Service::Enabled);
        break;

    default:
        break;
    }

    d->setSafetyMode (p_mode);
}

void Service::handleText (const QString& p_text)
{
    Q_D (Service);
    qDebug() << "[Dictation::Agent::handleText()] Got text from Sphinx:" << p_text;
    const QString startWord = Core::configuration ("Dictation/StartWord").toString();
    const QString endWord = Core::configuration ("Dictation/EndWord").toString();

    if (isSafetyModeEnabled()) {
        const bool usedStartSafetyWord = p_text == startWord;
        const bool usedEndSafetyWord = p_text == endWord;
        const bool usedSafetyWord = usedStartSafetyWord || usedEndSafetyWord;

        if (usedSafetyWord) {
            if (usedStartSafetyWord) {
                d->m_safetyMode = Service::Active;
            }
            else if (usedEndSafetyWord) {
                d->m_safetyMode = Service::Inactive;
            }

            return;
        }
        else {
            if (isSafetyModeActive()) {
                qDebug() << "[Dictation::Agent::handleText()] Text" << p_text << "ignored since safety mode is active.";
                return;
            }
            else {
                qDebug() << "[Dictation::Agent::handleText()] Valid text for echoing:" << p_text;
            }
        }
    }

    KeyboardEmulator::instance()->sendPhrase (p_text);
}

Service::~Service()
{

}

#include "services/dictation/service.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
