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
 * @date 05/20/12 14:54:02 PM
 */

#ifndef SPEECHCONTROL_ADAPTIONUTILITY_HPP
#define SPEECHCONTROL_ADAPTIONUTILITY_HPP

#include <QDir>
#include <QObject>
#include <QProcess>

class QTemporaryFile;

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

    /**
     * @brief Represents the phases of which the adaption process takes place.
     **/
    typedef enum {
        Undefined = -1,                      ///< Represents an undefined Phase.
        Initialized,                         ///< Represents the initialized phase of the AdaptationUtility. No real activity has taken place as of yet.
        CopyAcousticModels,                  ///< Represents the phase that of which the copying of the base acoustic model to its new, cloned location ccurs.
        GenerateFeatures,                    ///< Represents the phase that of which that feature extraction from the base acoustic model takes place.
        GenerateMixtureWeights,              ///< Represents the phase that of which the generation or cloning (if existing) of mixture weights occurs.
        ConvertModelDefinitions,             ///< Represents the act of converting the binary format of the model definition file (mdef) into its text format (mdef.txt).
        CollectAcousticStatistics,           ///< Represents the phase where the obtaining of statistical information from the base acoustic model occurs.
        ProperAdaptation,                   ///< Represents the act of adaption upon the new templated model from the speech corpus (@c Session) and its parent corpus.
        GenerateSendump,                     ///< Represents the space-conversing phase of generating sendmap information data.
        GenerateAccuracyReportHypothesis,    ///< Represents the act of generating accuracy information of the newly adapted acoustic model.
        CompleteAdaption,                    ///< Represents the phase that handles the final tier of adaption, tweaking SpeechControl's internal listing and what-not to recognize the model.
        Deinitialized                        ///< Represents the de-initialized phase of the AdaptationUtility.
    } Phase;

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

    /**
     * @brief Returns the AcousticModel that was generated from the act of adaption.
     * @note This might return NULL until the model itself has been formed.
     **/
    AcousticModel* resultingModel();

    /**
     * @brief Obtains the current phase of the AdaptionUtility.
     **/
    Phase currentPhase();

    /**
     * @brief Invokes the adaption process.
     * @return A pointer to the new AcousticModel, or NULL if the operation failed.
     **/
    AcousticModel* adapt();

    /**
     * @brief Obtains a user-friendly string representing the specified phase.
     * @param p_phase The phase to be translated into text.
     **/
    QString obtainPhaseText (const SpeechControl::AdaptationUtility::Phase& p_phase) const;

    /**
     * @brief Obtains the temporary file used for rendering the hypothesis.
     **/
    QTemporaryFile* hypothesis();

signals:
    /**
     * @brief Emitted when a phase has begun.
     * @param p_phase The phase that begun.
     **/
    void phaseStarted (const Phase& p_phase);

    /**
     * @brief Emitted when a phase has ended.
     * @param p_phase The phase that ended.
     **/
    void phaseEnded (const Phase& p_phase);

    /**
     * @brief Emitted when a phase's encountered a run-time error.
     * @param p_phase The phase that the error occurred in.
     * @param p_message The message reported by the phase.
     **/
    void phaseError (const Phase& p_phase, const QString& p_message);

    /**
     * @brief Emitted when the AdaptionUtility has begun.
     **/
    void startedAdaptation();

    /**
     * @brief Emitted when the AdaptionUtility has ended.
     **/
    void completedAdaptation();

private slots:
    void on_process_finished (const int& p_exitCode, QProcess::ExitStatus p_exitStatus);

private:
    // Phase-related.
    void setPhase (const Phase& phase);
    void startPhase (Phase phase);
    void reportErrorInPhase (const QString& p_message);
    void cleanupPhase (const Phase& phase);
    void endCurrentPhase ();
    void nextPhase();
    void halt();

    // Procedural steps.
    void generateFeatures();
    void generateMixtureWeights();
    void convertModelDefinitions();
    void collectAcousticStatistics();
    void performAdaptation();
    void generateSendump();
    void generateAccuracyReportHypothesis();
    void copyAcousticModel();
    void completeAdaptation();

    // Miscellaneous.
    void executeProcess (const QString& p_program, const QStringList p_arguments);

    Session* m_session;
    AcousticModel* m_modelBase;
    AcousticModel* m_modelResult;
    QProcess* m_prcss;
    QTemporaryFile* m_fileTmpHyp;
    Phase current_phase;
    QDir m_dirAccum;
};

}

#endif // SPEECHCONTROL_ADAPTIONUTILITY_HPP
// kate: indent-mode cstyle; replace-tabs on;
