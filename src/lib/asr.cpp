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


#include "asr.hpp"
#include "acousticmodel.hpp"
#include "dictionary.hpp"

#include <QGlib/Connect>

#include <QGst/enums.h>
#include <QGst/ElementFactory>

using namespace SpeechControl;

ASR::ASR ( QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ) {
}

ASR::ASR ( QGst::PipelinePtr p_pipeline, QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ), m_pipeline ( p_pipeline ) {
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR ( const QString& p_description, QObject* p_parent ) : QObject ( p_parent ),
    m_running ( NotPrepared ), m_ready ( NotPrepared ) {
    buildPipeline ( p_description );
    prepare();
}

void ASR::buildPipeline ( QString p_description ) {
    m_pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription ( p_description );
    m_pipeline->add ( bin );
}

void ASR::prepare() {
    m_psphinx = m_pipeline->getElementByName ( "asr" );
    m_vader   = m_pipeline->getElementByName ( "vad" );

    QGlib::connect ( m_psphinx, "partial_result", this, &ASR::formPartialResult );
    QGlib::connect ( m_psphinx, "result", this, &ASR::formResult );
    //     _psphinx->setProperty ("configured", true);

    m_bus = m_pipeline->bus();
    m_bus->addSignalWatch();
    QGlib::connect ( m_bus, "message::application", this, &ASR::applicationMessage );

    m_pipeline->setState ( QGst::StateReady );
    m_ready = Ready;
}

ASR::~ASR() {
    m_pipeline->setState ( QGst::StateNull );
}

QString ASR::standardDescription() {
    return QString ( "autoaudiosrc name=audiosrc ! audioconvert"
                     " ! audioresample ! audiorate ! volume name=volume"
                     " ! vader name=vad auto_threshold=true"
                     " ! pocketsphinx name=asr"
                     " ! fakesink" );
}

/// @todo How to deal with this decoder in GValue?
QGlib::Value ASR::decoder() const {
    return m_psphinx->property ( "decoder" );
}

/// @todo Should we implement a class/struct to wrap these values more programatically?
QDir ASR::languageModel() const {
    QGlib::Value lm = m_psphinx->property ( "lm" );
    return QDir ( lm.get<QString>() );
}

Dictionary* ASR::dictionary() const {
    const QString l_dict = m_psphinx->property ( "dict" ).toString();
    return Dictionary::obtain ( l_dict );
}

AcousticModel* ASR::acousticModel() const {
    const QString l_hmm = m_psphinx->property ( "hmm" ).toString();
    return new AcousticModel ( l_hmm );
}

const QGst::PipelinePtr ASR::pipeline() const {
    return m_pipeline;
}

const QGst::ElementPtr ASR::pocketSphinxElement() const {
    return m_psphinx;
}

const QGst::ElementPtr ASR::vaderElement() const {
    return m_vader;
}

const QGst::BusPtr ASR::busElement() const {
    return m_bus;
}


void ASR::setVaderProperty ( const QString& p_property, const QVariant& p_value ) {
    m_vader->setProperty ( p_property.toStdString().c_str(), QGlib::Value ( p_value.toString() ) );
}

void ASR::setPsProperty ( const QString& p_property, const QVariant& p_value ) {
    m_psphinx->setProperty ( p_property.toStdString().c_str(), QGlib::Value ( p_value.toString() ) );
}

void ASR::setLanguageModel ( const QString& p_path ) {
    if ( QFile::exists(p_path) ) {
        setPsProperty ( "lm", p_path );
    } else {
        qWarning() << "[ASR] Given language model path" << p_path << "does not exist.";
    }
}

void ASR::setDictionary ( const QString& p_path ) {
    if ( QFile::exists( p_path ) ) {
        setPsProperty ( "dict", p_path );
    } else {
        qWarning() << "[ASR] Given dictionary path" << p_path << "does not exist.";
    }
}

void ASR::setAcousticModel ( const QString& p_path ) {

    if ( QDir ( p_path ).exists() ) {
        setPsProperty ( "hmm", p_path );
    } else {
        qWarning() << "[ASR] Given acoustic model path" << p_path << "does not exist.";
    }
}

bool ASR::isReady() const {
    return m_ready == Ready;
}

bool ASR::isRunning() const {
    return m_running == Running;
}

bool ASR::start() {
    qDebug() << "[ASR start]";

    if ( isReady() ) {
        m_pipeline->setState ( QGst::StatePlaying );
        m_running = Running;
    } else {
        qWarning() << "[ASR] Object is not ready to run.";
    }

    return isRunning();
}

void ASR::stop() {
    m_pipeline->setState ( QGst::StatePaused );
}

void ASR::togglePause() {
    const bool l_silent = ! ( m_vader->property ( "silent" ).toBool() );
    m_vader->setProperty ( "silent", l_silent );
}

void ASR::formPartialResult ( QString& p_text, QString& p_uttid ) {
    QGst::Structure l_psStructure ( "partial_result" );
    l_psStructure.setValue ( "hyp", p_text );
    l_psStructure.setValue ( "uttid", p_uttid );
    QGst::MessagePtr l_message = QGst::ApplicationMessage::create ( m_psphinx, l_psStructure );
    m_bus->post ( l_message );
}

void ASR::formResult ( QString& p_text, QString& p_uttid ) {
    QGst::Structure l_psStructure ( "result" );
    l_psStructure.setValue ( "hyp", p_text );
    l_psStructure.setValue ( "uttid", p_uttid );
    QGst::MessagePtr l_message = QGst::ApplicationMessage::create ( m_psphinx, l_psStructure );
    m_bus->post ( l_message );
}

#include "asr.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
