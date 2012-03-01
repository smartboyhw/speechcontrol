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

// local includes
#include "include/mic.hpp"
#include "include/base.hpp"

using namespace SpeechControl;

MODULE::MODULE() {
    TEST_ADD ( MODULE::listMics )
    TEST_ADD ( MODULE::dumpMicProps )

    TestModule::instance()->add ( std::auto_ptr<Test::Suite> ( this ) );
}

void MODULE::listMics() { }
void MODULE::dumpMicProps() { }
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
