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

#ifndef SPEECHCONTROL_ADAPTIONUTILITY_HPP
#define SPEECHCONTROL_ADAPTIONUTILITY_HPP

#include <QObject>
#include <QProcess>

namespace SpeechControl
{

class AcousticModel;
class Session;

/**
 * @brief Provides SpeechControl with the ability to adapt sessions into existing acoustic models.
 *
 * AdaptationUtility performs most of the actions required to perform an adaption as
 * prescribed by <http://cmusphinx.sourceforge.net/wiki/tutorialadapt>. The process is
 * typically as follows:
 *
 * @li Obtain the feature parameters of the acoustic model to be adapted. ( @c generateFeatures() )
 * @li Convert the model definitions from its binary format into a text format. ( @c convertModelDefinitions() )
 * @li Obtain statistical information about the acoustic model. ( @c collectAcousticStatistics() )
 * @li Enacts the adaption process, MAP-style. ( @c performAdaptation() )
 * @li Re-generate the sendmap file (to save disk space) ( @c generateSendmap() )
 * @li Generate statistical information about the accuracy and efficiency of the newly adopted model. ( @c generateAccuracyReport() )
 *
 * More information about adaption in SpeechControl can be found at <http://wiki.thesii.org/SpeechControl/AdaptingAcousticModels>.
 **/
class AdaptationUtility : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        PhaseUndefined = -1,
        PhaseInitialized,
        PhaseCopyAcousticModels,
        PhaseGenerateFeatures,
        PhaseGenerateMixtureWeights,
        PhaseConvertModelDefinitions,
        PhaseCollectAcousticStatistics,
        PhasePerformAdaptation,
        PhaseGenerateSendmap,
        PhaseGenerateAccuracyReport,
        PhaseCompleteAdaption,
        PhaseDeinitialized
    } Phases;

    /**
     * @brief Default constructor.
     * @param p_session The Session to be adapted with.
     * @param p_model The AcousticModel to be adapted.
     **/
    explicit AdaptationUtility (Session* p_session, AcousticModel* p_model);

    /**
     * @brief Null constructor.
     **/
    AdaptationUtility();

    /**
     * @brief Destructor.
     **/

    virtual ~AdaptationUtility();

    /**
     * @brief Changes the Session to be merged with the acoustic model.
     * @param p_session The Session to use.
     **/
    void setSession (Session* p_session);

    /**
     * @brief Changes the AcousticModel to be adapted.
     * @param p_model The new model to adapt.
     **/
    void setAcousticModel (AcousticModel* p_model);

    /**
     * @brief Returns the Session that'll be used for adaption.
     **/
    Session* session();

    /**
     * @brief Returns the AcousticModel that'll be used for the base of adaption.
     **/
    AcousticModel* baseModel();

    AcousticModel* resultingModel();

    Phases currentPhase();

    /**
     * @brief Invokes the adaption process.
     * @return A pointer to the new AcousticModel, or NULL if the operation failed.
     **/
    AcousticModel* adapt();
    QString obtainPhaseText (const SpeechControl::AdaptationUtility::Phases& p_phase) const;

signals:
    void phaseStarted(const Phases& p_phase);
    void phaseEnded(const Phases& p_phase);

private:
    void changePhase (const Phases& p_phase);
    void generateFeatures();
    void generateMixtureWeights();
    void convertModelDefinitions();
    void collectAcousticStatistics();
    void performAdaptation();
    void generateSendmap();
    void generateAccuracyReport();
    void copyAcousticModel();
    void executeProcess (const QString& p_program, const QStringList p_arguments);
    void advanceNextPhase();
    void cleanupPhases(const SpeechControl::AdaptationUtility::Phases& phase);
    void haltPhasing();
    void completeAdaptation();

    Session* m_session;
    AcousticModel* m_modelBase;
    AcousticModel* m_modelResult;
    QProcess* m_prcss;
    Phases m_phase;
public slots:
    void on_mPrcss_finished (const int& p_exitCode, QProcess::ExitStatus p_exitStatus);
};

}

#endif // SPEECHCONTROL_ADAPTIONUTILITY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
