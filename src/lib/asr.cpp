/**
 *   @file asr.cpp
 *   Copyright (C) 2012  Adrian Borucki <gentoolx@gmail.com>
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


#include "asr.hpp"
using namespace SpeechControl;

ASR::ASR ( QObject* parent ) : QObject ( parent )
{

}

ASR::ASR ( QGst::BinPtr bin, QObject* parent ) : _bin ( bin ), QObject ( parent )
{
}

ASR::ASR ( const char* description, QObject* parent ) : QObject ( parent )
{
    _bin = QGst::Bin::fromDescription ( description );
}

ASR::ASR ( const QString& description, QObject* parent ) : QObject ( parent )
{
    _bin = QGst::Bin::fromDescription ( description.toStdString().c_str() );
}

const QGst::BinPtr ASR::getBin()
{
    return _bin;
}

QString ASR::getStandardDescription()
{
    QString desc ( "autoaudiosrc name=audiosrc ! audioconvert" );
    desc.append ( " ! audioresample ! audiorate ! volume name=volume" );
    desc.append ( " ! vader name=vad auto_threshold=true" );
    desc.append ( " ! pocketsphinx name=asr" );
    desc.append ( " ! fakesink" );
    return desc;
}


// kate: indent-mode cstyle; space-indent on; indent-width 4; 
