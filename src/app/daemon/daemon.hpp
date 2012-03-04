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

#ifndef SPEECHCONTROL_DAEMON_DAEMON_HPP
#define SPEECHCONTROL_DAEMON_DAEMON_HPP

#include <QObject>
#include <QDBusAbstractAdaptor>

namespace SpeechControl {
namespace Daemon {

/**
 * @brief ...
 **/
class Daemon : public QObject {
    Q_OBJECT
    Q_PROPERTY ( bool Active READ isActive )

signals:
    /**
     * @brief ...
     *
     * @return void
     **/
    void started();
    /**
     * @brief ...
     *
     * @return void
     **/
    void stopped();

public:
    /**
     * @brief ...
     *
     **/
    Daemon();
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isActive() const;

public slots:
    /**
     * @brief ...
     *
     * @return void
     **/
    Q_NOREPLY void start();
    /**
     * @brief ...
     *
     * @return void
     **/
    Q_NOREPLY void stop();
    /**
     * @brief ...
     *
     * @return QString
     **/
    QString listen();
};

}

}

#endif // SPEECHCONTROL_DAEMON_DAEMON_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
