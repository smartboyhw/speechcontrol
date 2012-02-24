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

#ifdef WITH_PYTHON_BINDINGS

#include "acousticmodel.hpp"
#include "corpus.hpp"
#include "dictionary.hpp"
#include "microphone.hpp"
#include "sentence.hpp"
#include "sphinx.hpp"
#include "system.hpp"

#include <boost/python.hpp>
#include <boost/noncopyable.hpp>

struct SystemStruct {
public:
    static void start() {
        SpeechControl::System::start();
    }

    static void stop() {
        SpeechControl::System::stop();
    }
};

BOOST_PYTHON_MODULE ( spchcntrl ) {
    using namespace boost::python;
    using namespace SpeechControl;

    class_<AcousticModel> ( "AcousticModel", no_init )
    .add_property ( "samplerate"   , &AcousticModel::sampleRate, &AcousticModel::setSampleRate )
    .add_property ( "parameters"   , &AcousticModel::parameters, &AcousticModel::setParameters )
    ;

    class_<Microphone> ( "Microphone", no_init )
    .add_property ( "uuid"     , &Microphone::id )
    .add_property ( "active"       , &Microphone::active )
    .add_property ( "friendlyName" , &Microphone::friendlyName )
    //.def("defaultMicrophone", &Microphone::defaultMicrophone)
    ;

    class_<Sphinx> ( "Sphinx", init<const AcousticModel*> ( "AcousticModel" ) )
    .add_property ( "text", &Sphinx::text )
    ;

    class_<SystemStruct> ( "System", no_init )
    .def ( "stop" , &SystemStruct::stop )
    .def ( "start", &SystemStruct::start )
    ;
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
