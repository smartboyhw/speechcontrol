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

#include <cpptest-assert.h>

#include <lib/dummysc.hpp>

// local includes
#include "include/asr.hpp"
#include "base.hpp"

using namespace SpeechControl;

MODULE::MODULE() {
    TEST_ADD( MODULE::testInitialize )
    TEST_ADD( MODULE::testInvoke )

    TestModule::instance()->add(std::auto_ptr<Test::Suite>(this));
}

void MODULE::testInitialize() {
    SpeechControl::DummySC* l_asr;
    TEST_ASSERT_MSG(l_asr == new SpeechControl::DummySC, "Failed to initialize variable.")
}

void MODULE::testInvoke() {
    TEST_FAIL( "Testing failures." )
}