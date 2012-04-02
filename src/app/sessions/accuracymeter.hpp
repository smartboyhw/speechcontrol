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

#ifndef ACCURACYMETER_HPP
#define ACCURACYMETER_HPP

#include <QObject>
#include <QProcess>
#include <QVariantMap>

namespace SpeechControl
{

class AcousticModel;
class Session;

class AccuracyMeter : public QObject
{
    Q_OBJECT
    Q_ENUMS (Status)

signals:
    void assessmentCompleted ();

public:
    enum Status {
        Undefined = -1,
        Successful,
        Error
    };

private:
    void parseOutput (const QString& p_output);
    QProcess* m_prcss;
    AcousticModel* m_model;
    Session* m_session;
    Status m_status;
    QVariantMap m_data;

public:
    explicit AccuracyMeter (QObject* parent = 0);
    AccuracyMeter (AcousticModel* p_model);
    void setSession (Session* p_session);
    void doAssessment (const QString& p_pathHyp);
    virtual ~AccuracyMeter();

    inline Session* session() const {
        return m_session;
    }

    inline AccuracyMeter::Status status() {
        return m_status;
    }
    inline QVariantMap data() {
        return m_data;
    }

public slots:
    void on_mPrcss_finished (const int& p_exitCode, QProcess::ExitStatus p_status);
};
}

#endif // ACCURACYMETER_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
