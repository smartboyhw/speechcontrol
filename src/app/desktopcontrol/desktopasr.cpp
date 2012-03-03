/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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

#include "desktopasr.hpp"

using namespace SpeechControl::DesktopControl;

DesktopASR::DesktopASR ( QObject* parent ) : ASR ( parent )
{

}

DesktopASR::DesktopASR ( const char* description, QObject* parent ) : ASR ( description, parent )
{

}

DesktopASR::DesktopASR ( const QString& description, QObject* parent ) : ASR ( description, parent )
{

}

DesktopASR::DesktopASR ( QGst::PipelinePtr pipeline, QObject* parent ) : ASR ( pipeline, parent )
{

}

void DesktopASR::applicationMessage ( const QGst::MessagePtr& message )
{
    QString msgType    = message->internalStructure()->name();
    QString hypothesis = message->internalStructure()->value ( "hyp" ).toString();
    QString uttid      = message->internalStructure()->value ( "uttid" ).toString();
    if ( msgType == "result" ) {
        qDebug() << "ASR result:" << hypothesis;
        emit finished ( hypothesis );
    }
}

#include "desktopasr.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
