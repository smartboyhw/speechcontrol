/***
 *  This file is part of the SpeechControlw project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControlw is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  SpeechControlw is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControlw .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 14:49:22 PM
 */

#include <QtGlobal>
#include <QScopedPointer>

#include <app/global.hpp>
#include <app/services/module.hpp>

namespace SpeechControl {
struct AbstractServiceModulePrivate {
    Q_DECLARE_PUBLIC (AbstractServiceModule)
    AbstractServiceModulePrivate (AbstractServiceModule* p_qPtr);
    ~AbstractServiceModulePrivate();
    virtual void changeState (AbstractServiceModule::ActivityState p_state) = 0;
    virtual AbstractServiceModule::ActivityState handleStateChange (const AbstractServiceModule::ActivityState p_state) = 0;

    AbstractServiceModule* q_ptr;
    AbstractServiceModule::ActivityState m_state;
};

}

// kate: indent-mode cstyle; replace-tabs on;
