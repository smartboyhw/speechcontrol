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
#include "service.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Voxforge;
Service* Service::s_inst = 0;

Service::Service() : AbstractModule (Core::instance())
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
    return Core::configuration ("Voxforge/Enabled").toBool();
}

QString Service::id() const
{
    return "vxfrg";
}

QPixmap Service::pixmap() const
{
    return QIcon::fromTheme ("audio-input").pixmap (64, 64);
}

QString Service::name() const
{
    return "Voxforge";
}

/// @todo This should return TRUE whenever an upload's in progress or underway.
bool Service::isActive() const
{
    return isEnabled();
}

Service::~Service()
{

}

#include "services/voxforge/service.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
