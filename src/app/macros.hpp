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

#ifndef SC_MACROS_HPP
#define SC_MACROS_HPP

#include <QDebug>

#define SC_SINGLETON(Type) \
    private:\
        static Type* s_inst;\
        Type();\
    public:\
        static Type* instance ( )\
        {\
            if (s_inst == 0)\
            {\
                s_inst = new Type;\
                qDebug() << "[SC_SINGLETON()] Created singleton instance of " #Type << ".";\
            }\
            return s_inst;\
        }\
 
#ifndef QT_NO_DEBUG
# define SC_ASSERT(condition,message) ((!(#condition)) ? qt_assert("condition##message",__FILE__,__LINE__) : qt_noop())
#else
# define SC_ASSERT(condition,message) qt_noop()
#endif

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
