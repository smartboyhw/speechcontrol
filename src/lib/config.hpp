/**
 *   @file config.hpp
 *   Copyright (C) 2012  Jacky Alcine <jacky.alcine@thesii.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SPCHCNTRL_LIB_CONFIG_HPP
#define SPCHCNTRL_LIB_CONFIG_HPP

/// Holds the build-time value of the location of PocketSphinx's pre-built modules.
#define POCKETSPHINX_MODELDIR "/usr/share/pocketsphinx/model"

/// Holds the version information of PocketSphinx.
#define POCKETSPHINX_VERSION "0.5.1"

/// Holds the build version of SpeechControl's library.
#define SPCHCNTRL_BUILD_VERSION "0.1"

/// Holds the date that of which SpeechControl's library was built.
#define SPCHCNTRL_BUILD_DATE __TIMESTAMP__

#endif /* SPCHCNTRL_LIB_CONFIG_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
