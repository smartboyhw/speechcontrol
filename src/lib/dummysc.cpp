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


#include "dummysc.hpp"

#include <QDebug>

using namespace SpeechControl;

DummySC::DummySC (QObject* parent) : ASR (parent)
{

}

DummySC::DummySC (QGst::PipelinePtr pipeline, QObject* parent) : ASR (pipeline, parent)
{

}

DummySC::DummySC (const char* description, QObject* parent) : ASR (description, parent)
{

}

DummySC::DummySC (const QString& description, QObject* parent) : ASR (description, parent)
{

}

void DummySC::applicationMessage (const QGst::MessagePtr& message)
{
    QString msgType    = message->internalStructure()->name();
    QString hypothesis = message->internalStructure()->value("hyp").toString();
    QString uttid      = message->internalStructure()->value("uttid").toString();
    if (msgType == "partial_result")
        qDebug() << "Partial ASR result:\n"
                 << "Hypothesis:" << hypothesis
                 << "\nUtterance ID:" << uttid;
    else if (msgType == "result") {
        qDebug() << "ASR result:" << hypothesis;
        stop();
        emit finished(hypothesis);
    }
}

