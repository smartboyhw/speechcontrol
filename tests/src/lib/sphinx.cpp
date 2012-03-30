/***
 *  This file is part of SpeechControl.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QString>
#include <QtTest/QTest>
#include <QtGStreamer/QGlib/refpointer.h>
#include <QtGStreamer/QGst/Element>
#include <lib/system.hpp>
#include <lib/abstractaudiosource.hpp>

#include "sphinx.h"
#include "config_sphinx.hpp"
#define SECONDS 18 + 2.5

using namespace QTest;
using namespace SpeechControl;

TestAbstractSphinx::TestAbstractSphinx (QObject* p_object) : AbstractSphinx (p_object)
{
    QString l_pipeline = standardDescription();
    l_pipeline = l_pipeline.replace ("autoaudiosrc name=audiosrc", "filesrc name=audiosrc ! decodebin");
    buildPipeline (l_pipeline);
}

void TestAbstractSphinx::applicationMessage (const QGst::MessagePtr& p_message)
{

}

void TestSphinx::init()
{
    SpeechControl::System::start();
}

void TestSphinx::cleanup()
{
    SpeechControl::System::stop();
}

void TestSphinx::obtainSphinxInstance()
{
    TestAbstractSphinx* l_sphinx = new TestAbstractSphinx (this);
}

void TestSphinx::recognizeTextFromSample()
{
    QString l_audioPath (AUDIO);
    QString l_text, l_uttid;

    TestAbstractSphinx* l_sphinx = new TestAbstractSphinx (this);

    l_sphinx->audioSrcElement()->setProperty<const char*> ("location", l_audioPath.toStdString().c_str());
    QCOMPARE (l_sphinx->start(), true);
    l_sphinx->formResult (l_text, l_uttid);
    QCOMPARE (!l_text.isEmpty(), true);
    QCOMPARE (!l_uttid.isEmpty(), true);
    QCOMPARE (l_sphinx->stop(), true);

}

void TestSphinx::benchSphinx()
{
    if (DeviceAudioSource::allDevices().length() == 0)
        QSKIP ("This test requires at least one input device operational on the test environment.", SkipSingle);

    TestAbstractSphinx* l_sphinx = new TestAbstractSphinx (this);
    QCOMPARE (l_sphinx->start(), true);
    qDebug() << "Waiting" << SECONDS << "secs to permit recognition process from mic.";
    qWait ( (int) ( (SECONDS) * 1000));
    QCOMPARE (l_sphinx->stop(), true);
}

QTEST_MAIN (TestSphinx)
#include "sphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

