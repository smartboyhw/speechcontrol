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
#include <lib/abstractsphinx.hpp>

namespace SpeechControl
{

class System;
namespace Daemon
{
class Daemon;

class DaemonSphinx : public AbstractSphinx
{
    Q_OBJECT
    friend class Daemon;
    explicit DaemonSphinx ();
    virtual void applicationMessage (const QGst::MessagePtr& p_message);
};

class Daemon : public QObject
{
    Q_OBJECT
    Q_PROPERTY (bool Active READ isActive)
    friend class System;

private:
    Daemon();
    static Daemon* s_inst;
    DaemonSphinx* m_sphnx;

signals:
    /**
     * @brief Emitted when the daemon's started.
     **/
    void started();

    /**
     * @brief Emitted when the daemon's stopped.
     **/
    void stopped();

public:
    /**
     * @brief Determines if the daemon's active.
     **/
    bool isActive() const;

    /**
     * @brief Obtains a pointer to the Daemon instance.
     **/
    static Daemon* instance();

public slots:

    /**
     * @brief Starts the daemon.
     **/
    Q_NOREPLY void start();

    /**
     * @brief Stops the daemon.
     **/
    Q_NOREPLY void stop();

    /**
     * @brief Begins the listening process for text from the user and returns that value.
     * @return The text heard by the user.
     **/
    QString listen();
};

}

}

#endif // SPEECHCONTROL_DAEMON_DAEMON_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
