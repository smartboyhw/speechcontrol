/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef DUMMYSC_HPP
#define DUMMYSC_HPP

#include <QObject>
#include <QGst/Pipeline>
#include <asr.hpp>

namespace SpeechControl {

class DummySC : public ASR
{
    Q_OBJECT

public:
    explicit DummySC (QObject* parent = 0);

    DummySC (QGst::PipelinePtr pipeline, QObject* parent = 0);

    DummySC (const char* description, QObject* parent = 0);

    DummySC (const QString& description, QObject* parent = 0);

public slots:
    void applicationMessage(const QGst::MessagePtr& message);
};

}
#endif // DUMMYSC_HPP
