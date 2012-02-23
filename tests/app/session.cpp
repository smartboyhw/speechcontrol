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

#include <cpptest-assert.h>

// application includes
#include <app/session.hpp>

// local includes
#include "include/session.hpp"
#include "include/base.hpp"

using namespace SpeechControl;

Content* g_cntn = 0;

MODULE::MODULE() {
    TEST_ADD ( MODULE::testInitialize )
    TEST_ADD ( MODULE::testCreateContent )
    TEST_ADD ( MODULE::testCreateSession )
    TEST_ADD ( MODULE::testDeinitialize )
    TestModule::instance()->add ( std::auto_ptr<Test::Suite> ( this ) );
}

void MODULE::testInitialize() {
    Session::init();
}

void MODULE::testCreateContent() {
    g_cntn = Content::create ( "John Doe", "Hello World",
                               "The licenses for most software are designed to take away your"
                               "freedom to share and change it.  By contrast, the GNU General Public"
                               "License is intended to guarantee your freedom to share and change free"
                               "software--to make sure the software is free for all its users.  This"
                               "General Public License applies to most of the Free Software"
                               "Foundation's software and to any other program whose authors commit to"
                               "using it.  (Some other Free Software Foundation software is covered by"
                               "the GNU Lesser General Public License instead.)  You can apply it to"
                               "your programs, too." );

    TEST_ASSERT_MSG ( g_cntn != 0, "Failed to render Content." );
}

void MODULE::testCreateSession() {
}

/// @todo When a deinitialization method for sessions is defined, call it here.
void MODULE::testDeinitialize() {
    TEST_THROWS_NOTHING_MSG ( 0,"No deinitialization available." );
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
