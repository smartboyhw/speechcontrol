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

/*!
 * \mainpage SpeechControl Library
 *
 */

#include "system.hpp"
#include "audiosource/device.hpp"
#include "audio/filerecorder.hpp"
#include "audio/devicemanager.hpp"

#include <QDir>
#include <QGst/Init>
#include <QCoreApplication>

using namespace SpeechControl;

System* System::s_inst = 0;

System::System (int* argc, char** argv[])
    : QObject (QCoreApplication::instance())
{
    if (argc && argv)
        QGst::init (argc, argv);
    else
        QGst::init();

    QDir configDir;

    DeviceAudioSource::allDevices();

    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/corpus");
    configDir.mkpath (QDir::homePath() + "/.config/speechcontrol/dictionaries");

    Audio::DeviceManager::setup();
    /// @todo Use app configuration instead.
    Audio::DeviceManager::setMux("Wav");
}

void System::start()
{
    if (!s_inst)
        s_inst = new System (0, 0);
}

void System::start (int* argc, char** argv[])
{
    if (!s_inst)
        s_inst = new System (argc, argv);
}

System* System::self()
{
    return s_inst;
}

void System::stop()
{
    self()->deleteLater();
}

#include "system.moc"
// kate: indent-mode cstyle; replace-tabs on; 
