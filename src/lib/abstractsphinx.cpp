/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#include "abstractsphinx.hpp"
#include "acousticmodel.hpp"
#include "dictionary.hpp"
#include "microphone.hpp"
#include "languagemodel.hpp"

#include <QGlib/Connect>

#include <QGst/enums.h>
#include <QGst/ElementFactory>
#include <pocketsphinx.h>

using namespace SpeechControl;

AbstractSphinx::AbstractSphinx ( QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ) {
}

AbstractSphinx::AbstractSphinx ( QGst::PipelinePtr p_pipeline, QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ), m_pipeline ( p_pipeline ) {
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
AbstractSphinx::AbstractSphinx ( const QString& p_description, QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ) {
    buildPipeline ( p_description );
}

void AbstractSphinx::buildPipeline ( QString p_description ) {
    m_pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription ( p_description );
    m_pipeline->add ( bin );
    qDebug() << "Built pipeline for AbstractSphinx (" << p_description << ").";
    prepare();
}

void AbstractSphinx::prepare() {
    m_psphinx = m_pipeline->getElementByName ( "asr" );
    m_vader   = m_pipeline->getElementByName ( "vad" );

    QGlib::connect ( m_psphinx, "partial_result", this, &AbstractSphinx::formPartialResult );
    QGlib::connect ( m_psphinx, "result", this, &AbstractSphinx::formResult );
    //m_psphinx->setProperty ( "configured", true );

    m_bus = m_pipeline->bus();
    m_bus->addSignalWatch();
    QGlib::connect ( m_bus, "message::application", this, &AbstractSphinx::applicationMessage );

    m_pipeline->setState ( QGst::StateReady );
    //m_psphinx->setState ( QGst::StateReady );
    //m_vader->setState ( QGst::StateReady );
    m_ready = Ready;
}

QString AbstractSphinx::standardDescription() {
    return QString ( "autoaudiosrc name=audiosrc ! audioconvert"
                     " ! audioresample ! audiorate ! volume name=volume"
                     " ! vader name=vad auto_threshold=true"
                     " ! pocketsphinx name=asr"
                     " ! fakesink" );
}

/// @todo Determine how to pull the pointer of the held data from the QGlib::Value (or GValue) and use that as the ps_decoder_t.
QGlib::Value AbstractSphinx::decoder() const {
    QGlib::Value l_glibPs = m_psphinx->property ( "decoder" );
    return l_glibPs;
}

LanguageModel* AbstractSphinx::languageModel() const {
    QGlib::Value lm = m_psphinx->property ( "lm" );
    return LanguageModel::fromPath ( lm.toString() );
}

Dictionary* AbstractSphinx::dictionary() const {
    const QString l_dict = m_psphinx->property ( "dict" ).toString();
    return Dictionary::obtain ( l_dict );
}

AcousticModel* AbstractSphinx::acousticModel() const {
    const QString l_hmm = m_psphinx->property ( "hmm" ).toString();
    return new AcousticModel ( l_hmm , parent());
}

const QGst::PipelinePtr AbstractSphinx::pipeline() const {
    return m_pipeline;
}

const QGst::ElementPtr AbstractSphinx::volumeElement() const {
    return m_pipeline->getElementByName ( "volume" );
}

const QGst::ElementPtr AbstractSphinx::audioSrcElement() const {
    return m_pipeline->getElementByName ( "audiosrc" );
}

void AbstractSphinx::useMicrophone ( const Microphone* p_microphone ) {
    audioSrcElement()->setProperty<const char*> ( "device",p_microphone->name().toStdString().c_str() );
}

const QGst::ElementPtr AbstractSphinx::pocketSphinxElement() const {
    return m_psphinx;
}

const QGst::ElementPtr AbstractSphinx::vaderElement() const {
    return m_vader;
}

const QGst::BusPtr AbstractSphinx::busElement() const {
    return m_bus;
}

void AbstractSphinx::setVaderProperty ( const QString& p_property, const QVariant& p_value ) {
    m_vader->setProperty ( p_property.toStdString().c_str(), QGlib::Value ( p_value.toString() ) );
}

void AbstractSphinx::setPsProperty ( const QString& p_property, const QVariant& p_value ) {
    m_psphinx->setProperty ( p_property.toStdString().c_str(), QGlib::Value ( p_value.toString() ) );
}

void AbstractSphinx::setLanguageModel ( const QString& p_path ) {
    if ( QFile::exists ( p_path ) ) {
        setPsProperty ( "lm", p_path );
    } else {
        qWarning() << "[ASR] Given language model path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setLanguageModel ( const LanguageModel* p_languageModel ) {
    setPsProperty ( "lm",p_languageModel->path() );
}

void AbstractSphinx::setDictionary ( const QString& p_path ) {
    if ( QFile::exists ( p_path ) ) {
        setPsProperty ( "dict", p_path );
    } else {
        qWarning() << "[ASR] Given dictionary path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setDictionary ( const Dictionary* p_dictionary ) {
    return setDictionary ( p_dictionary->path() );
}

void AbstractSphinx::setAcousticModel ( const QString& p_path ) {

    if ( QDir ( p_path ).exists() ) {
        setPsProperty ( "hmm", p_path );
    } else {
        qWarning() << "[ASR] Given acoustic model path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setAcousticModel ( const AcousticModel* p_acousticModel ) {
    setAcousticModel(p_acousticModel->path());
}

bool AbstractSphinx::isReady() const {
    return m_ready == Ready;
}

bool AbstractSphinx::isRunning() const {
    return m_running == Running;
}

bool AbstractSphinx::start() {
    qDebug() << "[ASR start]";

    if ( isReady() ) {
        m_pipeline->setState ( QGst::StatePlaying );
        m_running = Running;
    } else {
        qWarning() << "[ASR] Object is not ready to run.";
    }

    return isRunning();
}

bool AbstractSphinx::stop() {
    if ( m_pipeline->setState ( QGst::StateNull ) == QGst::StateChangeSuccess )
        m_running = NotPrepared;

    return m_running == NotPrepared;
}

void AbstractSphinx::togglePause() {
    const bool l_silent = ! ( m_vader->property ( "silent" ).toBool() );
    m_vader->setProperty ( "silent", l_silent );
}

/// @todo Does this obtain the partial result from the internal PocketSphinx component?
void AbstractSphinx::formPartialResult ( QString& p_text, QString& p_uttid ) {
    QGst::Structure l_psStructure ( "partial_result" );
    l_psStructure.setValue ( "hyp", p_text );
    l_psStructure.setValue ( "uttid", p_uttid );
    QGst::MessagePtr l_message = QGst::ApplicationMessage::create ( m_psphinx, l_psStructure );
    m_bus->post ( l_message );
}

/// @todo Does this obtain the partial result from the internal PocketSphinx component?
void AbstractSphinx::formResult ( QString& p_text, QString& p_uttid ) {
    QGst::Structure l_psStructure ( "result" );
    l_psStructure.setValue ( "hyp", p_text );
    l_psStructure.setValue ( "uttid", p_uttid );
    QGst::MessagePtr l_message = QGst::ApplicationMessage::create ( m_psphinx, l_psStructure );
    m_bus->post ( l_message );
}

AbstractSphinx::~AbstractSphinx() {
    m_pipeline->setState ( QGst::StateNull );
}
#include "abstractsphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
