/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 12:26:34 PM
 */

class QApplication;
class QSettings;
class QTranslator;

#include <app/global.hpp>

SPCHCNTRL_BEGIN_NAMESPACE

class Core;
struct CorePrivate {
    Q_DECLARE_PUBLIC(Core)
    CorePrivate(Core* p_qPtr);
    virtual ~CorePrivate();
    void hookUpSignals();

    QApplication* m_app;
    QSettings* m_settings;
    QTranslator* m_trnsltr;
    Core* q_ptr;
};

SPCHCNTRL_END_NAMESPACE
// kate: indent-mode cstyle; replace-tabs on;
