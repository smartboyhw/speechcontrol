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

#include "adaptionutility.hpp"
#include "session.hpp"
#include <acousticmodel.hpp>

#include <QDebug>
#include <QProcess>
#include <QDir>

using namespace SpeechControl;


AdaptationUtility::AdaptationUtility() : QObject(), m_session (0), m_modelBase (0), m_modelResult(0)
{
    qWarning() << "[AdaptationUtility] Initialized with null objects.";
}

AdaptationUtility::AdaptationUtility (Session* p_session, AcousticModel* p_model) : QObject(), m_session (p_session), m_modelBase (p_model), m_modelResult(0)
{

}

Session* AdaptationUtility::session()
{
    return m_session;
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

    copyAcousticModel();
    generateFeatures();
    generateMixtureWeights();
    convertModelDefinitions();
    collectAcousticStatistics();
    performAdaptation();
    generateSendmap();
    generateAccuracyReport();

    return m_modelResult;
}

void AdaptationUtility::copyAcousticModel()
{
    m_modelResult = m_modelBase->clone();
}

/*
 * <http://cmusphinx.sourceforge.net/wiki/tutorialadapt#generating_acoustic_feature_files>
 *
 * In order to run the adaptation tools, you must generate a set of acoustic
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
             -mswav no  \                          [defines input data as Microsoft WAV (tiff)]
             -raw yes                              [defines input data as raw binary data]
 *
 *
 */
void AdaptationUtility::generateFeatures()
{
    // Build argument values.
    QDir dirInput(m_session->corpus()->audioPath());
    QDir dirOutput = QDir::temp();
    QString suffixInput = ".raw";
    QString suffiXOutput = ".mfc";

    // Build control file.
    QFile* controlFile = m_session->corpus()->fileIds();

    QStringList args;
    args << "-argfile" << m_modelBase->parameterPath()
         << "-samprate" << QString::number(16000)
         << "-c" << controlFile->fileName()
         << "-di" << dirInput.absolutePath()
         << "-do" << dirOutput.absolutePath()
         << "-ei" << suffixInput
         << "-eo" << suffiXOutput;

    executeProcess("sphinx_fe",args);
}

/*
* Alternately, if you have installed the SphinxTrain Python modules, you can
* use sendump.py to convert the sendump file from the acoustic model to a
* mixture_weights file.
*/
* void AdaptationUtility::generateMixtureWeights()
{

}

/*
 * Converting 'mdef' to plain text:
 * pocketsphinx_mdef_convert -text hub4wsj_sc_8k/mdef hub4wsj_sc_8k/mdef.txt
 */
void AdaptationUtility::convertModelDefinitions()
{

}

/*
 * The next step in adaptation is to collect statistics from the adaptation data.
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
void AdaptationUtility::collectAcousticStatistics()
{

}

/*
 * Okay, this is a two-fold process here. According to a post on CMUSphinx's forums,
 * PocketSphinx can handle continuous models. If that's the case, then we can
 * use MLLR-style adaptations. Otherwise, MAP's the only right way.
 *
 *
 * Creating transformation with MLLR
 * ----------------------------------
 * MLLR transforms are supported by pocketsphinx and sphinx4. MLLR is a cheap
 * adaptation method that is suitable when amount of data is limited.
 * It's a good idea to use MLLR for online adaptation. MLLR works best for
 * continuous model. It's effect for semi-continuous models is very limited
 * since semi-continuous models mostly relies on mixture weights. If you want
 * best accuracy you can combine MLLR adaptation with MAP adaptation.
 *
 * Next we will generate an MLLR transformation which we will pass to the decoder
 * to adapt the acoustic model at run-time. This is done with the mllr_solve program:
 * ./mllr_solve \
            -meanfn hub4wsj_sc_8k/means \
            -varfn hub4wsj_sc_8k/variances \
            -outmllrfn mllr_matrix
            -accumdir . \

 * This command will create an adaptation data file called mllr_matrix.
 * Now, if you wish to decode with the adapted model,
 * simply add -mllr mllr_matrix (or whatever the path to the mllr_matrix file
 * you created is) to your pocketsphinx command line.
 *
 * Updating the acoustic model files with MAP
 * ------------------------------------------
 * MAP is different adaptation method. In this case unlike for MLLR we don't
 * create a generic transform but update each parameter in the model. We will
 * now copy the acoustic model directory and overwrite the newly created
 * directory with adapted model files:
  cp -a hub4wsj_sc_8k hub4wsj_sc_8kadapt
 *
 * To do adaptation, use the map_adapt program:
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

}

/*
 * And for all of the jazz at:
 * http://cmusphinx.sourceforge.net/wiki/tutorialadapt#testing_the_adaptation
 */
void AdaptationUtility::generateAccuracyReport()
{

}

void AdaptationUtility::executeProcess (const QString& p_program, const QStringList p_arguments)
{
    QProcess process(this);
    process.start(p_program,p_arguments);
}

AdaptationUtility::~AdaptationUtility()
{

}

#include "adaptionutility.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
