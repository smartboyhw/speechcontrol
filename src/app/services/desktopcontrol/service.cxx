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

#include <QObject>
#include <QDeclarativeView>

#include "app/core.hpp"
#include "app/ui/main-window.hpp"
#include "app/services/module.hpp"
#include "app/services/desktopcontrol/command.hpp"
#include "app/services/desktopcontrol/sphinx.hpp"
#include "app/services/desktopcontrol/service.hpp"
#include "app/services/desktopcontrol/service.hxx"

using SpeechControl::Core;
using namespace SpeechControl::DesktopControl;

ServicePrivate::ServicePrivate (Service* p_qPtr) : AbstractModulePrivate (p_qPtr),
    m_sphinx (0),
    m_view (new QDeclarativeView (QUrl ("qrc:///qml/dskptctlui")))
{
    m_sphinx = new Sphinx (Sphinx::standardDescription());

    QString defAcousticModel = Core::configuration ("DesktopControl/AcousticModel").toString();
    QString defLanguageModel = Core::configuration ("DesktopControl/LanguageModel").toString();

    if (!defAcousticModel.isEmpty())
        m_sphinx->setAcousticModel (defAcousticModel);

    if (!defLanguageModel.isEmpty())
        m_sphinx->setLanguageModel (defLanguageModel);
}

ServicePrivate::ServicePrivate (const SpeechControl::Services::AbstractModulePrivate& p_other) : AbstractModulePrivate (p_other)
{

}

AbstractModule::ActivityState ServicePrivate::handleStateChange (const AbstractModule::ActivityState p_state)
{
    switch (p_state) {
    case AbstractModule::Enabled:

        if (!m_sphinx->start()) {
            qWarning() << "[DesktopControl::Agent::start()] Start unsuccessful.";
            return AbstractModule::Disabled;
        }

        m_view->show();

        qDebug() << "[DesktopControl::ServicePrivate::start()] Enabled.";

        return AbstractModule::Enabled;
        break;

    case AbstractModule::Disabled:

        if (m_sphinx->isRunning() || m_sphinx->isReady()) {
            qWarning() << "[DesktopControl::ServicePrivate::stop()] Stop unsuccessful.";
            return AbstractModule::Enabled;
        }

        m_view->hide();

        return AbstractModule::Disabled;
        break;

    default:
        return AbstractModule::Undefined;
    }

}

ServicePrivate::~ServicePrivate()
{

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
