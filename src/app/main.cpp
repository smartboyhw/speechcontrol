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

/*!
 * \mainpage %SpeechControl Application
 *
 * SpeechControl's primary goal is to provide efficient and simple speech recognition
 * services to the Linux desktop (or whichever desktop it happens to be compiled on).
 * Using CMU's Sphinx speech recognition libraries and the Voxforge collective, we take
 * on a task never to be ventured before by a open-source community. It happens to also
 * be one of the many applications to be released by the Synthetic Intellect Institute.
 *
 * \section authors Authors
 * The authors of this documentation are largely the developers who took the time out
 * to comment out their code as they continue to develop SpeechControl.
 */

// local includes
#include "core.hpp"

// pretty simple, huh?
int main ( int argc, char** argv ) {
    SpeechControl::Core l_core ( argc, argv );
    l_core.start();
    return l_core.exec();
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
