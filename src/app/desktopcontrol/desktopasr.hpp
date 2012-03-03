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

#ifndef DESKTOPASR_HPP
#define DESKTOPASR_HPP

#include <lib/asr.hpp>

namespace SpeechControl {
namespace DesktopControl {

class DesktopASR : public ASR {
    Q_OBJECT

public:
    explicit DesktopASR ( QObject* parent = 0 );

    DesktopASR ( const char* description, QObject* parent = 0 );

    DesktopASR ( const QString& description, QObject* parent = 0 );

    DesktopASR ( QGst::PipelinePtr pipeline, QObject* parent = 0 );

public slots:
    void applicationMessage ( const QGst::MessagePtr& message );
};

}
}

#endif // DESKTOPASR_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
