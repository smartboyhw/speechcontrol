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
 * @date 05/19/12 21:02:46 PM
 */

#include <QUuid>
#include <QDebug>
#include <QProcess>
#include <QTemporaryFile>

#include <lib/acousticmodel.hpp>
#include <lib/dictionary.hpp>

#include "sessions/session.hpp"
#include "sessions/session.hpp"
#include "sessions/corpus.hpp"
#include "sessions/adaptionutility.hpp"

using namespace SpeechControl;


AdaptationUtility::AdaptationUtility() : QObject(), m_session (0), m_modelBase (0), m_modelResult (0), m_prcss (0), m_fileTmpHyp (new QTemporaryFile), m_phase (Undefined)
{
    qWarning() << "[AdaptationUtility] Initialized with null objects.";
}

AdaptationUtility::AdaptationUtility (Session* p_session, AcousticModel* p_model) : QObject(), m_session (p_session), m_modelBase (p_model), m_modelResult (0), m_prcss (0), m_fileTmpHyp (new QTemporaryFile), m_phase (Undefined)
{

}

Session* AdaptationUtility::session()
{
    return m_session;
}

QTemporaryFile* AdaptationUtility::hypothesis()
{
    return m_fileTmpHyp;
}

AcousticModel* AdaptationUtility::baseModel()
{
    return m_modelBase;
}

AcousticModel* AdaptationUtility::resultingModel()
{
    return m_modelResult;
}

void AdaptationUtility::setSession (Session* p_session)
{
    m_session = p_session;
}

void AdaptationUtility::setAcousticModel (AcousticModel* p_model)
{
    m_modelBase = p_model;
}

AcousticModel* AdaptationUtility::adapt()
{
    if (!m_session || !m_modelBase)
        return 0;

    if (m_prcss)
        delete m_prcss;

    // set up the process.
    m_prcss = new QProcess (this);
    m_prcss->setProcessChannelMode (QProcess::MergedChannels);
    connect (m_prcss, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (on_process_finished (int, QProcess::ExitStatus)));

    // invoke the cycle.
    changePhase (Initialized);
    advanceNextPhase();

    return m_modelResult;
}

void AdaptationUtility::advanceNextPhase()
{
    switch (m_phase) {
    case Initialized:
        copyAcousticModel();
        break;

    case CopyAcousticModels:
        generateFeatures();
        break;

    case GenerateFeatures:
        generateMixtureWeights();
        break;

    case GenerateMixtureWeights:
        convertModelDefinitions();
        break;

    case ConvertModelDefinitions:
        collectAcousticStatistics();
        break;

    case CollectAcousticStatistics:
        performAdaptation();
        break;

    case PerformAdaptation:
        generateSendmap();
        break;

    case GenerateSendmap:
        generateAccuracyReportHypothesis();
        break;

    case GenerateAccuracyReportHypothesis:
        completeAdaptation();
        break;

    case CompleteAdaption:
        changePhase (Deinitialized);
        break;

    case Deinitialized:
        changePhase (Undefined);
        break;

    default:
    case Undefined:
        break;
    }
}

void AdaptationUtility::cleanupPhase (const Phase& phase = Undefined)
{
    if (phase == Undefined) {
        cleanupPhase (CopyAcousticModels);
        cleanupPhase (CollectAcousticStatistics);
        cleanupPhase (GenerateMixtureWeights);
        cleanupPhase (GenerateFeatures);
        cleanupPhase (GenerateSendmap);
        cleanupPhase (GenerateAccuracyReportHypothesis);
        cleanupPhase (PerformAdaptation);
    }
    else {
        switch (phase) {
        case CopyAcousticModels:
            break;

        case CollectAcousticStatistics:
            break;

        case ConvertModelDefinitions:
            break;

        case GenerateAccuracyReportHypothesis:
            break;

        case GenerateFeatures:
            break;

        case GenerateMixtureWeights:
            break;

        case GenerateSendmap:
            break;

        case PerformAdaptation:
            break;

        case CompleteAdaption:

        case Initialized:
        case Deinitialized:
        case Undefined:
            break;
        }
    }
}

void AdaptationUtility::halt()
{
    changePhase (Deinitialized);
    advanceNextPhase();
}

AdaptationUtility::Phase AdaptationUtility::currentPhase()
{
    return m_phase;
}

void AdaptationUtility::endPhase ()
{
    emit phaseEnded (m_phase);
    m_phase = Undefined;
}

void AdaptationUtility::startPhase (AdaptationUtility::Phase p_phase)
{
    m_phase = p_phase;
    qDebug() << "[AdaptationUtility::setPhase()] Set to phase" << obtainPhaseText (m_phase);
    emit phaseStarted (m_phase);

    if (m_phase == CopyAcousticModels)
        emit startedAdapting();
    else if (m_phase == CompleteAdaption)
        emit endedAdapting();
}

void AdaptationUtility::changePhase (const Phase& p_phase)
{
    endPhase ();
    startPhase (p_phase);
}

void AdaptationUtility::reportErrorInPhase (const QString& p_message)
{
    const Phase erredPhase = m_phase;
    emit phaseError (erredPhase, p_message);

}

void AdaptationUtility::executeProcess (const QString& p_program, const QStringList p_arguments)
{
    m_prcss->start (p_program, p_arguments);
    qDebug() << "[AdaptationUtility::executeProcess()] Waiting for start...";
    if (m_prcss->waitForStarted()) {
        qDebug() << "[AdaptationUtility::executeProcess()] Invoking" << p_program << "with" << p_arguments << "..";
    } else {
        emit phaseError(currentPhase(),"Failed to start process" + p_program + ".");
    }
}

QString AdaptationUtility::obtainPhaseText (const Phase& p_phase) const
{
    switch (p_phase) {
    case CollectAcousticStatistics:
        return "Collecting Acoustic Statistics";
        break;

    case ConvertModelDefinitions:
        return "Convert Model Definitions";
        break;

    case CopyAcousticModels:
        return "Copy Acoustic Model";
        break;

    case GenerateAccuracyReportHypothesis:
        return "Generate Accuracy Report Hypothesis";
        break;

    case GenerateFeatures:
        return "Generate Features";
        break;

    case GenerateMixtureWeights:
        return "Generate Mixture Weights";
        break;

    case CompleteAdaption:
        return "Complete Adaption";
        break;

    case Deinitialized:
        return "Deinitialized";
        break;

    case Initialized:
        return "Initialized";
        break;

    case GenerateSendmap:
        return "Generate sendmap";
        break;

    case PerformAdaptation:
        return "Perform Adaption";
        break;

    default:
        return "Invalid Phase";
        break;
    }

    return "Unknown Phase";
}

void AdaptationUtility::copyAcousticModel()
{
    changePhase (CopyAcousticModels);
    m_modelResult = m_modelBase->clone();
    advanceNextPhase();
}

/*
 * <http://cmusphinx.sourceforge.net/wiki/tutorialadapt#generating_acoustic_feature_files>
 *
 * In order to run the adaption tools, you must generate a set of acoustic
 * model feature files (AMFF) from these WAV audio recordings. This can be done
 * with the sphinx_fe tool from SphinxBase. It is imperative that you make sure
 * you are using the same acoustic parameters to extract these features as were
 * used to train the standard acoustic model. Since PocketSphinx 0.4, these are
 * stored in a file called `feat.params` in the acoustic model directory. You
 * can simply add it to the command line for `sphinx_fe`, like this:
 *
   sphinx_fe \
             -argfile hub4wsj_sc_8k/feat.params \  [Arguments for training]
             -samprate 16000 \                     [Sample recording rate]
             -c arctic20.fileids \                 [Control file for batch processing]
             -di . \                               [input directory]
             -do . \                               [output directory]
             -ei wav \                             [input file extension]
             -eo mfc \                             [output file extension]
             -mswav no                             [defines input data as Microsoft WAV (tiff)]
 *
 *
 * Typically, the recorded audio is in WAV format, straight from the mic.
 */
///@todo Add a means of validating the files generated by Sphinx here.
void AdaptationUtility::generateFeatures()
{
    changePhase (GenerateFeatures);

    // Build argument values.
    QDir dirInput (m_session->corpus()->audioPath());
    QDir dirOutput (QDir::tempPath() + "/" + m_session->id() + "-" + QString::number (qrand()));
    dirOutput.mkpath (dirOutput.path());
    QString suffixInput = "wav";
    QString suffiXOutput = "mfc";
    QFile* controlFile = m_session->corpus()->fileIds();

    QStringList args;
    args << "-argfile"  << m_modelResult->parameterPath()
         << "-samprate" << QString::number (16000)
         << "-c"        << controlFile->fileName()
         << "-di"       << dirInput.absolutePath()
         << "-do"       << dirOutput.absolutePath()
         << "-ei"       << suffixInput
         << "-eo"       << suffiXOutput
         ;

    executeProcess ("sphinx_fe", args);
}

/*
* Alternately, if you have installed the SphinxTrain Python modules, you can
* use sendump.py to convert the sendump file from the acoustic model to a
* mixture_weights file.
*/
/// @todo Determine the path to 'sendump.py' from SphinxTrain.
void AdaptationUtility::generateMixtureWeights()
{
    changePhase (GenerateMixtureWeights);

    QStringList args;

    // Locate sendump.py
    /// @todo Do that more elegantly
    QFile loc1("/usr/lib/sphinxtrain/python/cmusphinx/sendump.py");
    QFile loc2("/usr/lib/python2.7/dist-packages/cmusphinx/sendump.py");
    QFile loc3("/usr/local/lib/python2.7/dist-packages/cmusphinx/sendump.py");
    QString sendumpLoc;
    if (loc1.exists())
        sendumpLoc = loc1.fileName();
    else if (loc2.exists())
        sendumpLoc = loc2.fileName();
    else
        sendumpLoc = loc3.fileName();

    args << sendumpLoc
         << m_modelBase->senDump()->fileName()
         << m_modelResult->mixtureWeights()->fileName()
         ;

    executeProcess ("python", args);
}

/*
 * Converting 'mdef' to plain text:
 * pocketsphinx_mdef_convert -text hub4wsj_sc_8k/mdef hub4wsj_sc_8k/mdef.txt
 */
void AdaptationUtility::convertModelDefinitions()
{
    changePhase (ConvertModelDefinitions);

    QFile* fileMdef = m_modelResult->modelDefinitions();

    QStringList args;
    args << "-text"     << fileMdef->fileName()
         << (fileMdef->fileName() + ".txt")
         ;

    executeProcess ("pocketsphinx_mdef_convert", args);
}

/*
 * The next step in adaption is to collect statistics from the adaption data.
 * This is done using the bw program from SphinxTrain.
 * Copy it to the working directory along with the map_adapt and mk_s2sendump
 * programs from the same directory. Now, to collect statistics, run:
   bw \
    -hmmdir hub4wsj_sc_8k \
    -moddeffn hub4wsj_sc_8k/mdef.txt \
    -ts2cbfn .semi. \
    -feat 1s_c_d_dd \
    -svspec 0-12/13-25/26-38 \
    -cmn current \
    -agc none \
    -dictfn arctic20.dic \
    -ctlfn arctic20.fileids \
    -lsnfn arctic20.transcription \
    -accumdir .

 * Make sure the arguments in bw command should match the parameters in feat.params
 * file inside the acoustic model folder. Please note that not all the parameters
 * from feat.param are supported by bw, only a few of them. bw for example doesn't
 * suppport upperf or other feature extraction params. You only need to use parameters
 * which are accepted, other parameters from feat.params should be skipped.
 *
 * For example, for most continuous models (like the ones used by Sphinx4) you
 * don't need to include the svspec option. Instead, you need to use just -
 * ts2cbfn .cont. For PTM models use -ts2cbfn .ptm.
 *
 * Sometimes if you're missing the file noisedict you also need an extra step,
 * copy the fillerdict file into the directory that you choose in the hmmdir
 * parameter, renaming it to noisedict.
 */
/// @todo Use silence deliminators from noise dictionary for transcription.
void AdaptationUtility::collectAcousticStatistics()
{
    changePhase (CollectAcousticStatistics);

    m_dirAccum.setPath (QDir::tempPath() + QString ("/speechcontrol-") + m_session->id());
    m_dirAccum.mkpath (m_dirAccum.path());

    QStringList args;
    args << "-hmmdir"    << m_modelResult->path()
         << "-moddeffn"  << (m_modelResult->modelDefinitions()->fileName() + ".txt")
         << "-ts2cbfn"    << ".semi."
         << "-feat"      << "ls_c_d_dd"
         << "-svspec"    << "0-12/13-25/26-38"
         << "-cmn"       << "current"
         << "-agc"       << "none"
         << "-dictfn"    << m_session->corpus()->dictionary()->path()
         << "-ctlfn"     << m_session->corpus()->fileIds()->fileName()
         << "-lsnfn"     << m_session->corpus()->transcription ("<s>", "</s>")->fileName()
         << "-accumdir"  << m_dirAccum.absolutePath();
    ;

    executeProcess ("bw", args);
}

/*
 * Okay, this is a two-fold process here. According to a post on CMUSphinx's forums,
 * PocketSphinx can handle continuous models. If that's the case, then we can
 * use MLLR-style adaptions. Otherwise, MAP's the only right way.
 *
 *
 * Creating transformation with MLLR
 * ----------------------------------
 * MLLR transforms are supported by pocketsphinx and sphinx4. MLLR is a cheap
 * adaption method that is suitable when amount of data is limited.
 * It's a good idea to use MLLR for online adaption. MLLR works best for
 * continuous model. It's effect for semi-continuous models is very limited
 * since semi-continuous models mostly relies on mixture weights. If you want
 * best accuracy you can combine MLLR adaption with MAP adaption.
 *
 * Next we will generate an MLLR transformation which we will pass to the decoder
 * to adapt the acoustic model at run-time. This is done with the mllr_solve program:
 * ./mllr_solve \
            -meanfn hub4wsj_sc_8k/means \
            -varfn hub4wsj_sc_8k/variances \
            -outmllrfn mllr_matrix
            -accumdir . \

 * This command will create an adaption data file called mllr_matrix.
 * Now, if you wish to decode with the adapted model,
 * simply add -mllr mllr_matrix (or whatever the path to the mllr_matrix file
 * you created is) to your pocketsphinx command line.
 *
 * Updating the acoustic model files with MAP
 * ------------------------------------------
 * MAP is different adaption method. In this case unlike for MLLR we don't
 * create a generic transform but update each parameter in the model. We will
 * now copy the acoustic model directory and overwrite the newly created
 * directory with adapted model files:
  cp -a hub4wsj_sc_8k hub4wsj_sc_8kadapt
 *
 * To do adaption, use the map_adapt program:
    map_adapt \
    -meanfn hub4wsj_sc_8k/means \
    -varfn hub4wsj_sc_8k/variances \
    -mixwfn hub4wsj_sc_8k/mixture_weights \
    -tmatfn hub4wsj_sc_8k/transition_matrices \
    -accumdir . \
    -mapmeanfn hub4wsj_sc_8kadapt/means \
    -mapvarfn hub4wsj_sc_8kadapt/variances \
    -mapmixwfn hub4wsj_sc_8kadapt/mixture_weights \
    -maptmatfn hub4wsj_sc_8kadapt/transition_matrices
 */
void AdaptationUtility::performAdaptation ()
{
    changePhase (PerformAdaptation);

    QStringList args;
    args << "-meanfn"    << m_modelBase->means()->fileName()
         << "-varfn"     << m_modelBase->variances()->fileName()
         << "-mixwfn"    << m_modelBase->mixtureWeights()->fileName()
         << "-tmatfn"    << m_modelBase->transitionMatrices()->fileName()
         << "-accumdir"  << "."
         << "-mapmeanfn" << m_modelResult->means()->fileName()
         << "-mapvarfn"  << m_modelResult->variances()->fileName()
         << "-mapmixwfn" << m_modelResult->mixtureWeights()->fileName()
         << "-maptmatfn" << m_modelResult->transitionMatrices()->fileName()
         ;

    executeProcess ("map_adapt", args);
}

/*
 *
 * Apparently, this is pretty nifty for the space freaks (like jalcine)
 *
 * Recreating the adapted sendump file
 * -----------------------------------
 * If you want to save space for the model you can use sendump file supported by
 * pocketsphinx. To recreate the sendump file from the updated mixture_weights file:

  mk_s2sendump \
        -pocketsphinx yes \
        -moddeffn hub4wsj_sc_8kadapt/mdef.txt \
        -mixwfn hub4wsj_sc_8kadapt/mixture_weights \
        -sendumpfn hub4wsj_sc_8kadapt/sendump

 * Congratulations! You now have an adapted acoustic model! You can delete the
 * files wsj1adapt/mixture_weights and wsj1adapt/mdef.txt to save space if you
 * like, because they are not used by the decoder.
 */
void AdaptationUtility::generateSendmap()
{
    changePhase (GenerateSendmap);

    QStringList args;
    args << "-pocketsphinx" << "yes"
         << "-modddefn"     << (m_modelResult->modelDefinitions()->fileName() + "txt")
         << "-mixwfn"       << (m_modelResult->mixtureWeights()->fileName())
         << "-sendumpfn"    << (m_modelResult->senDump()->fileName())
         ;

    executeProcess ("mk_s2sendump", args);
}

/*
 * And for all of the jazz at:
 * http://cmusphinx.sourceforge.net/wiki/tutorialadapt#testing_the_adaption
 *
 * A two step process, generating a hypothesis file using pocketsphinx_batch.
 * And then using a Perl script to determine accuracy.
 */
void AdaptationUtility::generateAccuracyReportHypothesis()
{
    changePhase (GenerateAccuracyReportHypothesis);

    // Render the temporary file.
    hypothesis()->setAutoRemove (false);
    hypothesis()->open();

    QStringList args;

    args << "-adcin"     << "yes"
         << "-cepdir"    << m_session->corpus()->audioPath()
         << "-cepext"    << ".wav"
         << "-ctl"       << m_session->corpus()->fileIds()->fileName()
         << "-lm"        << "/usr/share/pocketsphinx/model/lm/wsj/wlist5o.3e-7.vp.tg.lm.DMP" /// @todo Define a language model to be used here.
         << "-dict"      << "/usr/share/pocketsphinx/model/lm/wsj/wlist5o.dic"
         //m_session->corpus()->dictionary()->path()
         /// @bug Pocketsphinx crashes, which is probably associated with dictionaries.
         << "-hmm"       << m_modelResult->path()
         << "-hyp"       << hypothesis()->fileName()
         ;

    executeProcess ("pocketsphinx_batch", args);
}

void AdaptationUtility::completeAdaptation()
{
    changePhase (CompleteAdaption);
    advanceNextPhase();
}

void AdaptationUtility::on_process_finished (const int& p_exitCode, QProcess::ExitStatus p_exitStatus)
{
    qDebug() << "[AdaptationUtility::on_mPrcss_finished()] Exit code" << p_exitCode;
    qDebug() << "[AdaptationUtility::on_mPrcss_finished()] Output:" << m_prcss->readAll();

    if (p_exitCode != 0) {
        qWarning("[AdaptationUtility::on_proc_finished()] Error during adaptation!");
        cleanupPhase();
        halt();
        return;
    }

    switch (p_exitStatus) {
    case QProcess::NormalExit:
        qDebug() << "[AdaptationUtility::on_mPrcss_finished()] Normal exit experienced.";
        advanceNextPhase();
        break;

    case QProcess::CrashExit:
        qDebug() << "[AdaptationUtility::on_mPrcss_finished()] Crash exit experienced!";
        Phase curPhase = currentPhase();
        cleanupPhase();
        halt();
        emit phaseError(curPhase,"Crashed");
        break;
    }
}

AdaptationUtility::~AdaptationUtility()
{
    m_prcss->close();
    m_prcss->deleteLater();
}

#include "sessions/adaptionutility.moc"
// kate: indent-mode cstyle; replace-tabs on;
