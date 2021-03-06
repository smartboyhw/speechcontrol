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
 * @date 05/20/12 14:53:55 PM
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

/**
 * @brief Determines the accuracy of an acoustic model.
 *
 * AccuracyMeter provides a simple means of determining the accuracy of an acoustic
 * model. In order to do so, AccuracyMeter needs a speech corpus to represent the
 * data to be checked against.
 **/
class AccuracyMeter : public QObject
{
    Q_OBJECT
    Q_ENUMS (Status)

signals:
    /**
     * @brief Emitted when the assessment process has completed.
     **/
    void assessmentCompleted ();

public:
    /**
     * @brief Represents the status codes of the AccuracyMeter.
     **/
    enum Status {
        Undefined = -1,         ///< An undefined state, only for nullifying values.
        Successful,             ///< Represents success determining of the accuracy rate.
        Error                   ///< Represents an error occurring while determining of the accuracy rate.
    };

private:
    void parseOutput (const QString& p_output);
    QProcess* m_prcss;
    AcousticModel* m_model;
    Session* m_session;
    Status m_status;
    QVariantMap m_data;

public:
    /**
     * @brief Null constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit AccuracyMeter (QObject* p_parent = 0);

    /**
     * @brief Constructor.
     *
     * Creates a AccuracyMeter with an acoustic model, p_model, as its basis.
     *
     * @param p_model The model to be tested.
     **/
    AccuracyMeter (AcousticModel* p_model);

    /**
     * @brief Sets the session to use.
     *
     * @param p_session The Session to be used.
     **/
    void setSession (Session* p_session);

    /**
     * @brief Invokes the assessment.
     *
     * @see AdaptionUtility::hypothesis()
     * @param p_pathHyp The path to the file representing a hypothesis.
     **/
    void doAssessment (const QString& p_pathHyp);

    /**
     * @brief Destructor.
     **/
    virtual ~AccuracyMeter();

    /**
     * @brief Obtains the Session used by this AccuracyMeter.
     **/
    inline Session* session() const {
        return m_session;
    }

    /**
     * @brief Obtains the status of this AccuracyMeter.
     **/
    inline AccuracyMeter::Status status() {
        return m_status;
    }

    /**
     * @brief Obtains the data generated by this AccuracyMeter.
     **/
    inline QVariantMap data() {
        return m_data;
    }

private slots:
    void on_mPrcss_finished (const int& p_exitCode, QProcess::ExitStatus p_status);
};
}

#endif // ACCURACYMETER_HPP
// kate: indent-mode cstyle; replace-tabs on;
