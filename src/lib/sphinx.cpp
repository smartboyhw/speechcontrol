/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

// stdc++ includes
#include <cstdio>

// Sphinx includes
#include <pocketsphinx.h>

// Qt includes
#include <QFile>

// local includes
#include "sphinx.hpp"
#include "microphone.hpp"
#include "acousticmodel.hpp"

#define MODELDIR "/usr/share/pocketsphinx/model"

using namespace SpeechControl;

Sphinx::Sphinx(const AcousticModel* p_mdl) : m_mic(Microphone::defaultMicrophone()),
    m_mdl(const_cast<AcousticModel*>(p_mdl)), m_decoder(0), m_config(0)
{
    initialize();
}

Sphinx::Sphinx(const Sphinx &p_sphnx) : m_hypothesis(p_sphnx.m_hypothesis),
    m_mic(p_sphnx.m_mic), m_mdl(p_sphnx.m_mdl), m_decoder(p_sphnx.m_decoder),
    m_config(p_sphnx.m_config)
{
    initialize();
}


/// @todo Fine-tune this method to properly recognize text from file.
void Sphinx::recognizeFromFile(const QFile *p_file)
{
    int score;
    const char* uttid;
    ps_decode_raw(m_decoder, fdopen(p_file->handle(),"r"), NULL, -1);
    m_hypothesis = ps_get_hyp(m_decoder, &score, &uttid);
    emit textRecognized(m_hypothesis);
}

/// @todo Fine-tune this method to continuously from the microphone.
void Sphinx::recognizeFromMicrophone(const Microphone *p_mic)
{
   emit textRecognized(m_hypothesis);
}

void Sphinx::startRecognizing(Microphone *p_mic)
{
    p_mic->startRecording();
}

void Sphinx::stopRecognizing(Microphone* p_mic)
{
    p_mic->stopRecording();
}


const bool SpeechControl::Sphinx::isListening() const
{
    return (m_mic && m_mic->active());
}

void Sphinx::initialize()
{
    m_config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", MODELDIR "hmm/en_US/hub4wsj_sc_8k", NULL);
    m_decoder = ps_init(m_config);
}

const QString Sphinx::text() const
{
    return m_hypothesis;
}

Sphinx::~Sphinx() {
}

SphinxResult::SphinxResult(Sphinx *p_sphnx) : QObject(p_sphnx),
    m_sphnx(p_sphnx) {

}

const QString SphinxResult::text() const
{
    return m_txt;
}

const double SphinxResult::confidence() const
{
    return m_cnfdnc;
}

SphinxResult::~SphinxResult() {

}

#ifdef WITH_PYTHON_BINDINGS

#include <boost/python.hpp>
#include <boost/noncopyable.hpp>

BOOST_PYTHON_MODULE(spchcntrl)
{
    using namespace boost::python;

    class_<Sphinx>("Sphinx", init<const AcousticModel*>("AcousticModel"))
    .add_property("text", &Sphinx::text)
    ;
}

#endif