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
#include <QDebug>
#include <QtPlugin>
#include <QIcon>

#include "global.hpp"
#include "plugin.hpp"
#include "commands.hpp"

MPRIS_NAMESPACE_BEGIN

Plugin::Plugin (QObject* parent) : AbstractPlugin (PLUGIN_ID, parent)
{
}

void Plugin::initialize()
{
    qDebug() << "Plug-in loaded! (mpris)";
    MprisCategory::instance()->addCommand ( (new PlayerCommand));
    MprisCategory::instance()->addCommand ( (new LibraryCommand));
}

void Plugin::deinitialize()
{
    qDebug() << "Plug-in unloaded! (mprislist)";
    MprisCategory::instance()->deleteLater();
}

QPixmap Plugin::pixmap() const
{
    return QIcon::fromTheme ("applications-multimedia").pixmap (64, 64);
}

Plugin::~Plugin()
{

}

#include "plugin.moc"

MPRIS_NAMESPACE_END

Q_EXPORT_PLUGIN2 (spchcntrl-mpris, SpeechControl::Plugins::DesktopControl::MPRIS::Plugin)

// kate: indent-mode cstyle; replace-tabs on; 
