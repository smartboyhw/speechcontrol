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

#ifndef SPCHCNTRL_LIB_AUDIOSOURCE_DEVICE_HPP_
#define SPCHCNTRL_LIB_AUDIOSOURCE_DEVICE_HPP_

#include <QObject>
#include <lib/audiosource/abstract.hpp>

namespace SpeechControl
{

class DeviceAudioSource;
struct DeviceAudioSourcePrivate;

/**
 * @brief Represents a device used as an audio source on the user's system.
 *
 * DeviceAudioSource objects are more commonly used to represent input devices
 * on a user's system to obtain audio directly from the device.
 **/
class SPCH_EXPORT DeviceAudioSource : public AbstractAudioSource
{
    Q_OBJECT
    Q_PROPERTY (QString DeviceName READ deviceName) ///< The internal name of this DeviceAudioSource.
    Q_PROPERTY (QString HumanName READ humanName)   ///< The presentable name of this DeviceAudioSource.
    Q_DISABLE_COPY (DeviceAudioSource)
    Q_DECLARE_PRIVATE(DeviceAudioSource)
    friend class AbstractAudioSource;

public:
    explicit DeviceAudioSource();
    DeviceAudioSource (const AbstractAudioSource& p_other);
    virtual ~DeviceAudioSource();
    QString deviceName() const;
    QString humanName() const;
    static AudioSourceList allDevices();
    static DeviceAudioSource* defaultDevice();
    static DeviceAudioSource* obtain (const QString& p_deviceName);

protected:
    DeviceAudioSource (const QString& p_deviceName);
    virtual QString pipelineDescription() const;
    virtual void buildPipeline();

private:
    QScopedPointer<DeviceAudioSourcePrivate> d_ptr;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
