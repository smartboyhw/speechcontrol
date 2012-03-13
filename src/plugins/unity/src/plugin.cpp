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
 */
#include <QtPlugin>
#include <qicon.h>

#include "config.hpp"
#include "plugin.hpp"

using namespace SpeechControl::Plugins::UnityHud;

Plugin::Plugin (QObject* parent) : AbstractPlugin (QUuid (PLUGIN_UUID), parent)
{

}

void Plugin::initialize()
{

}

void Plugin::deinitialize()
{

}

QPixmap Plugin::pixmap() const
{
    return QIcon::fromTheme("ubuntu-logo").pixmap(64,64);
}

Plugin::~Plugin()
{

}


Q_EXPORT_PLUGIN2 (spchcntrl-unity-hud, SpeechControl::Plugins::UnityHud::Plugin)
#include "plugin.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
