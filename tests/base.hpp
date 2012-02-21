/**  This file is part of SpeechControl
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef TEST_BASE_HPP
#define TEST_BASE_HPP

#include <QObject>
#include <cpptest-suite.h>

class MODULE;

namespace SpeechControl {
    class TestModule : public QObject, public Test::Suite {
        Q_OBJECT

    signals:
        void testInvoked();

    private:
        void handleTest();
        static TestModule* s_inst;

    public:
        TestModule();
        virtual ~TestModule();
        void dumpBacktrace();
        static TestModule* instance();
    };
}
#endif