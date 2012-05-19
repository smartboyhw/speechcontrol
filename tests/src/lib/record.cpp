/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/01/12 4:23:42 PM
 */

#include <QtTest/QtTest>
#include <QGst/Pipeline>
#include <QGst/Bin>
#include <QGst/Init>
#include "record.h"

void TestRecord::init()
{
    QGst::init();
}
void TestRecord::doRecord()
{
    QGst::BinPtr bin = QGst::Bin::fromDescription(QString ("%1 ! level name=level ! audioconvert ! "
                       "audioresample ! audiorate ! volume name=volume ! "
                       "appsink name=sink").arg ("autoaudiosrc"));
    QGst::PipelinePtr pipeline;
}

void TestRecord::cleanup() {

}


QTEST_MAIN (TestRecord)
#include "record.moc"
