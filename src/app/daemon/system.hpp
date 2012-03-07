/***
 *  This file is part of SpeechContrl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechContrl is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechContrl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechContrl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef SPEECHCONTROL_DAEMON_SYSTEM_HPP
#define SPEECHCONTROL_DAEMON_SYSTEM_HPP

#include <QObject>
#include <QCoreApplication>
#include "daemon.hpp"

namespace SpeechControl {
namespace Daemon {

/**
 * @brief ...
 **/
class System : public QObject {
    Q_OBJECT
public:
    /**
     * @brief ...
     *
     * @param p_argc ...
     * @param p_argv ...
     **/
    explicit System ( int p_argc, char** p_argv );
    /**
     * @brief ...
     *
     * @param p_argc ...
     * @param p_argv ...
     * @return :Daemon::System*
     **/
    static System* instance ( int p_argc,char** p_argv );
    /**
     * @brief ...
     *
     * @return :Daemon::Daemon*
     **/
    Daemon* daemon();
    /**
     * @brief ...
     *
     * @return int
     **/
    int exec();

private:
    static System* s_inst;
    QCoreApplication m_app;
};

}
}

#endif // SPEECHCONTROL_DAEMON_SYSTEM_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
