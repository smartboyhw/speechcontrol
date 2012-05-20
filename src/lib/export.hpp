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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef SPCHCNTRL_LIB_EXPORT_HPP
#define SPCHCNTRL_LIB_EXPORT_HPP
#include <QtGlobal>

#if defined(SPCH_EXPORT_SYMBOLS)
#define SPCH_EXPORT Q_DECL_EXPORT
#else
#define SPCH_EXPORT Q_DECL_IMPORT
#endif

// This define fixes a nasty compile bug. Don't believe me? Comment it out and see what happens.
#define QTGSTREAMERUTILS_EXPORT Q_DECL_IMPORT

#undef Q_D
#undef Q_Q

#define Q_D(Class) Class##Private* d = (Class##Private*) d_func()
#define Q_Q(Class) Class* q = (Class*) q_func()

#endif
// kate: indent-mode cstyle; replace-tabs on; 
