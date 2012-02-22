/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *            (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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

