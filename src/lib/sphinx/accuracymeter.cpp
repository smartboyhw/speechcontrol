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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 14:53:45 PM
 */

#include <QFile>
#include <QDebug>

#include "sessions/session.hpp"
#include "sessions/corpus.hpp"
#include "accuracymeter.hpp"

using namespace SpeechControl;

AccuracyMeter::AccuracyMeter (QObject* p_parent) : QObject (p_parent),
    m_prcss (0), m_model (0), m_session (0), m_status (Undefined)
{

}

AccuracyMeter::AccuracyMeter (AcousticModel* p_model) : QObject(),
    m_prcss (0), m_model (p_model), m_session (0), m_status (Undefined)
{

}

void AccuracyMeter::setSession (Session* p_session)
{
    if (p_session && p_session->isValid())
        m_session = p_session;
}

void AccuracyMeter::doAssessment (const QString& p_pathHyp)
{
    if (m_prcss && m_prcss->isOpen())
        return;

    if (m_session && m_session->isValid()) {
        QStringList args;
        args << "/usr/lib/sphinxtrain/scripts/decode/word_align.pl"
             << m_session->corpus()->transcription()->fileName()
             << p_pathHyp
             ;

        m_prcss = new QProcess (this);
        m_prcss->setProcessChannelMode (QProcess::MergedChannels);
        connect (m_prcss, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (on_mPrcss_finished (int, QProcess::ExitStatus)));
        m_prcss->start ("perl", args);
    }
    else {
        m_data["message"] = "No session was defined.";
        emit assessmentCompleted();
    }
}

void AccuracyMeter::on_mPrcss_finished (const int& p_exitCode , QProcess::ExitStatus p_status)
{
    m_data["exit-code"]   = p_exitCode;
    m_data["exit-status"] = p_status;
    parseOutput (m_prcss->readAllStandardOutput());
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
    qDebug() << "[AccuracyMeter::parseOutput()] Output:" << p_output;

    if (p_output.startsWith ("TOTAL ")) {
        QRegExp obtainWordCount ("Words: (\\d+)");
        QStringList lines = p_output.split ("\n");
        lines.replaceInStrings ("TOTAL ", "");

        qDebug() << obtainWordCount.indexIn (lines.at (0)) << obtainWordCount.capturedTexts() << lines.at (0);
        data["total-word-count"] = obtainWordCount.cap (1);

        m_status = Successful;
        m_data = data;
    }
    else {
        data["message"] = "Output provided was malformed.";
        data["output"] = p_output;
        m_status = Error;
        m_data = data;
    }

    emit assessmentCompleted();
}

AccuracyMeter::~AccuracyMeter()
{

}

#include "sessions/accuracymeter.moc"
// kate: indent-mode cstyle; replace-tabs on;
