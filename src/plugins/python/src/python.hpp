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

#ifndef SPCHCNTRL_PYTHON_PYTHON_HPP_
#define SPCHCNTRL_PYTHON_PYTHON_HPP_

#include <PythonQt/PythonQt.h>

#include <macros.hpp>

namespace SpeechControl
{
namespace Plugins
{
namespace Python
{

class Instance  : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Instance)
    SC_SINGLETON (Instance)

public slots:
    void start();
    void stop();

private:
    PythonQtObjectPtr obtainMainModule();
};
}
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
