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

#include <QtTest/QtTest>

#include <sessions/content.hpp>
#include <sessions/corpus.hpp>
#include <sessions/phrase.hpp>

#include "corpus_test.h"

using namespace SpeechControl;

QStringList TestCorpus::s_strlist = QStringList() << "Lorem ipsum dolor sit amet, nulla nec tortor."
                                    << "Donec id elit quis purus consectetur consequat."
                                    << "Nam congue semper tellus."
                                    << "Sed erat dolor, dapibus sit amet, venenatis ornare, ultrices ut, nisi."
                                    << "Aliquam ante.";

void TestCorpus::initTestCase()
{
    m_crps = 0;
    generateCorpus();
}

void TestCorpus::generateCorpus()
{
    m_crps = Corpus::create (s_strlist);
}

void TestCorpus::benchmarkCreateCorpus()
{
    TextContentSource* src = new TextContentSource (this);
    qDebug() << TESTDATA;
    src->setFile (new QFile (TESTDATA));
    Content* cntt = src->generate();
    QStringList pages = cntt->pages();
    Corpus* corp = 0;

    QBENCHMARK {
        corp = Corpus::create (pages);
        corp->erase();
    }
}

void TestCorpus::createCorpus()
{
    generateCorpus();
    QCOMPARE (m_crps != 0, 1);
}
void TestCorpus::countPhrases()
{
    generateCorpus();
    QCOMPARE (m_crps != 0, 1);

    const int count = s_strlist.join (" ").split (" ").length();
    int genCount = 0;

    Q_FOREACH (Phrase * snt, m_crps->phrases()) {
        genCount += snt->words();
    }

    QCOMPARE (genCount, count);
}

void TestCorpus::cleanupTestCase()
{
    if (m_crps) {
        m_crps->erase();
        m_crps->deleteLater();
    }
}


QTEST_MAIN (TestCorpus)

#include "corpus_test.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
