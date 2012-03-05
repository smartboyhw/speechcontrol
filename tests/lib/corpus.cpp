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
#include "corpus.h"
#include <lib/corpus.hpp>
#include <lib/phrase.hpp>

using namespace SpeechControl;

QStringList TestCorpus::s_strlist = QStringList() << "Lorem ipsum dolor sit amet, nulla nec tortor."
                                    << "Donec id elit quis purus consectetur consequat."
                                    << "Nam congue semper tellus."
                                    << "Sed erat dolor, dapibus sit amet, venenatis ornare, ultrices ut, nisi."
                                    << "Aliquam ante.";

void TestCorpus::initTestCase() {
    m_crps = 0;
    generateCorpus();
}

void TestCorpus::generateCorpus() {
    m_crps = Corpus::create ( s_strlist );
}

void TestCorpus::createCorpus() {
    generateCorpus();
    QCOMPARE ( m_crps != 0,1 );
}

void TestCorpus::countSentences() {
    generateCorpus();
    QCOMPARE ( m_crps != 0,1 );
    QCOMPARE ( m_crps->sentences().length(), s_strlist.length() );
}

void TestCorpus::countPhrases() {
    generateCorpus();
    QCOMPARE ( m_crps != 0,1 );

    const int l_count = s_strlist.join ( " " ).split ( " " ).length();
    int l_genCount = 0;

    Q_FOREACH ( Sentence* l_snt, m_crps->sentences() ) {
        Q_FOREACH ( Phrase* l_phr, l_snt->phrases() ) {
            l_genCount += l_phr->words();
        }
    }

    QCOMPARE ( l_genCount,l_count );
}

void TestCorpus::cleanupTestCase() {
    m_crps->erase();
    m_crps->deleteLater();
}


QTEST_MAIN ( TestCorpus )

#include "corpus.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
