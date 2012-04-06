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

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <QObject>

#include <config.hpp>
#include <export.hpp>

namespace SpeechControl
{

class SPCH_EXPORT System : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (System)

signals:
    void started();
    void stopped();

public:
    static System* self();
    static void start (int*, char** []);

public slots:
    static void start();
    static void stop();

private:
    explicit System (int* argc, char** argv[]);
    static System* s_inst;
};

}

#endif // SYSTEM_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

