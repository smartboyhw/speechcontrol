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

#include <QFile>
#include <QDebug>

#include "app/sessions/session.hpp"
#include "app/sessions/corpus.hpp"
#include "accuracymeter.hpp"

using namespace SpeechControl;

AccuracyMeter::AccuracyMeter (QObject* parent) : QObject (parent), m_prcss (0), m_model (0), m_session (0)
{

}

AccuracyMeter::AccuracyMeter (AcousticModel* p_model) : QObject(), m_prcss (0), m_model (p_model), m_session (0)
{

}

Session* AccuracyMeter::session() const
{
    return m_session;
}

void AccuracyMeter::setSession (Session* p_session)
{
    m_session = p_session;
}

void AccuracyMeter::doAssessment (const QString& p_pathHyp)
{
    qDebug() << ! (m_prcss == 0);

    if (m_prcss && m_prcss->isOpen())
        return;

    QStringList args;
    args << "/usr/lib/sphinxtrain/scripts/decode/word_align.pl"
         << m_session->corpus()->transcription()->fileName()
         << p_pathHyp
         ;

    qDebug() << args << (QFile (p_pathHyp)).readAll();
    m_prcss = new QProcess (this);
    m_prcss->setProcessChannelMode (QProcess::MergedChannels);
    connect (m_prcss, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (on_mPrcss_finished (int, QProcess::ExitStatus)));
    m_prcss->start ("perl", args);
}

void AccuracyMeter::on_mPrcss_finished (const int& p_exitCode , QProcess::ExitStatus p_status)
{
    switch (p_status) {
    case QProcess::NormalExit:
        parseOutput (m_prcss->readAllStandardOutput());
        break;

    default:
        break;
    }
}

/*
 * This method has to do some splitting of text and expression matching
 * since the output of word_align.pl isn't in a machine-readable format.
 */
void AccuracyMeter::parseOutput (const QString& p_output)
{
    /*
     * TEMPLATE FOR OUTPUT:
     *
     * TOTAL Words: %d Correct: %d Errors: %d\n
     * TOTAL Percent correct = %.2f%% Error = %.2f%% Accuracy = %.2f%%\n
     * TOTAL Insertions: $total_ins Deletions: $total_del Substitutions: $total_subst\n
     *
     */

    QVariantMap data;
    qDebug() << p_output;

    if (p_output.startsWith ("TOTAL ")) {
        QRegExp obtainWordCount ("Words: (\\d+)");
        QStringList lines = p_output.split ("\n");
        lines.replaceInStrings ("TOTAL ", "");

        qDebug() << obtainWordCount.indexIn (lines.at (0)) << obtainWordCount.capturedTexts() << lines.at (0);
        data["total-word-count"] = obtainWordCount.cap (1);

        emit assessmentCompleted (Successful, data);
    } else {
        data["message"] = "Output provided was malformed.";
        data["output"] = p_output;
        emit assessmentCompleted(Error,data);
    }
}

AccuracyMeter::~AccuracyMeter()
{

}

#include "accuracymeter.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
