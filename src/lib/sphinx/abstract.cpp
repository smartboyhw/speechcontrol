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

#include <QGlib/Connect>

#include <QGst/enums.h>
#include <QGst/ElementFactory>
#include <pocketsphinx.h>

#include "acousticmodel.hpp"
#include "dictionary.hpp"
#include "languagemodel.hpp"

#include "sphinx/abstract.hxx"
#include "sphinx/abstract.hpp"
#include "audiosource/abstract.hpp"

using namespace SpeechControl;

AbstractSphinx::AbstractSphinx (QObject* p_parent) : QObject (p_parent),
    m_running (NotPrepared), m_ready (NotPrepared), d_ptr (new AbstractSphinxPrivate (this))
{
}

AbstractSphinx::AbstractSphinx (QGst::PipelinePtr p_pipeline, QObject* p_parent) : QObject (p_parent),
    m_running (NotPrepared), m_ready (NotPrepared), d_ptr (new AbstractSphinxPrivate (this))
{
    Q_D (AbstractSphinx);
    d->m_pipeline = p_pipeline;
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
AbstractSphinx::AbstractSphinx (const QString& p_description, QObject* p_parent) : QObject (p_parent),
    m_running (NotPrepared), m_ready (NotPrepared), d_ptr (new AbstractSphinxPrivate (this))
{
    buildPipeline (p_description);
}

AbstractSphinx::AbstractSphinx (AbstractSphinxPrivate* p_private, QObject* p_parent) :
    QObject (p_parent), d_ptr (p_private)
{

}

void AbstractSphinx::buildPipeline (QString p_description)
{
    Q_D (AbstractSphinx);
    d->m_pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (p_description);
    d->m_pipeline->add (bin);
    qDebug() << "[AbstractSphinx::buildPipeline()] Built pipeline for AbstractSphinx (" << p_description << ").";
    prepare();
}

void AbstractSphinx::prepare()
{
    Q_D (AbstractSphinx);
    d->m_psphinx = d->m_pipeline->getElementByName ("asr");
    d->m_vader   = d->m_pipeline->getElementByName ("vad");

    QGlib::connect (d->m_psphinx, "partial_result", this, &AbstractSphinx::formPartialResult);
    QGlib::connect (d->m_psphinx, "result", this, &AbstractSphinx::formResult);
    d->m_psphinx->setProperty ("configured", true);

    d->m_pipeline->setState (QGst::StateReady);
    d->m_psphinx->setState (QGst::StateReady);
    d->m_vader->setState (QGst::StateReady);
    qDebug() << "[AbstractSphinx::prepare()] Prepared pipeline.";
    m_ready = Ready;
}

QString AbstractSphinx::standardDescription()
{
    return QString ("autoaudiosrc name=src ! audioconvert"
                    " ! audioresample ! audiorate ! volume name=volume"
                    " ! vader name=vad auto_threshold=true"
                    " ! pocketsphinx name=asr"
                    " ! fakesink");
}

/// @todo Determine how to pull the pointer of the held data from the QGlib::Value (or GValue) and use that as the ps_decoder_t.
QGlib::Value AbstractSphinx::decoder() const
{
    Q_D (const AbstractSphinx);
    QGlib::Value glibPs = d->m_psphinx->property ("decoder");
    bool ok;
    void* pointer = glibPs.get<void*> (&ok);
    qDebug() << "[AbstractSphinx::decoder()] Obtained successfully? " << ok << pointer;
    return glibPs;
}

const QGst::PipelinePtr AbstractSphinx::pipeline() const
{
    Q_D (const AbstractSphinx);
    return d->m_pipeline;
}

const QGst::ElementPtr AbstractSphinx::audioSrcElement() const
{
    return pipeline()->getElementByName ("src");
}

const QGst::ElementPtr AbstractSphinx::pocketSphinxElement() const
{
    Q_D (const AbstractSphinx);
    return d->m_psphinx;
}

const QGst::ElementPtr AbstractSphinx::vaderElement() const
{
    Q_D (const AbstractSphinx);
    return d->m_vader;
}

const QGst::BusPtr AbstractSphinx::busElement() const
{
    Q_D (const AbstractSphinx);
    return d->m_bus;
}

LanguageModel* AbstractSphinx::languageModel() const
{
    QGlib::Value lm = pocketSphinxElement()->property ("lm");
    return LanguageModel::fromDirectory (lm.toString());
}

Dictionary* AbstractSphinx::dictionary() const
{
    const QString dict = pocketSphinxElement()->property ("dict").toString();
    return Dictionary::obtain (dict);
}

AcousticModel* AbstractSphinx::acousticModel() const
{
    const QString hmm = pocketSphinxElement()->property ("hmm").toString();
    return new AcousticModel (hmm , parent());
}

const QGst::ElementPtr AbstractSphinx::volumeElement() const
{
    return pipeline()->getElementByName ("volume");
}

void AbstractSphinx::setVaderProperty (const QString& p_property, const QVariant& p_value)
{
    vaderElement()->setProperty (p_property.toStdString().c_str(), QGlib::Value (p_value.toString()));
}

void AbstractSphinx::setPsProperty (const QString& p_property, const QVariant& p_value)
{
    pocketSphinxElement()->setProperty (p_property.toStdString().c_str(), QGlib::Value (p_value.toString()));
}

void AbstractSphinx::setLanguageModel (const QString& p_path)
{
    if (QFile::exists (p_path)) {
        setPsProperty ("lm", p_path);
    }
    else {
        qWarning() << "[AbstractSphinx] Given language model path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setLanguageModel (const LanguageModel* p_languageModel)
{
    if (p_languageModel) {
        setLanguageModel (p_languageModel->path());
    }
}

void AbstractSphinx::setDictionary (const QString& p_path)
{
    if (QFile::exists (p_path)) {
        setPsProperty ("dict", p_path);
    }
    else {
        qWarning() << "[AbstractSphinx] Given dictionary path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setDictionary (const Dictionary* p_dictionary)
{
    if (p_dictionary) {
        setDictionary (p_dictionary->path());
    }
}

void AbstractSphinx::setAcousticModel (const QString& p_path)
{
    if (QDir (p_path).exists()) {
        setPsProperty ("hmm", p_path);
    }
    else {
        qWarning() << "[AbstractSphinx] Given acoustic model path" << p_path << "does not exist.";
    }
}

void AbstractSphinx::setAcousticModel (const AcousticModel* p_acousticModel)
{
    if (p_acousticModel && p_acousticModel->isValid()) {
        setAcousticModel (p_acousticModel->path());
    }
}

bool AbstractSphinx::isReady() const
{
    return m_ready == Ready;
}

bool AbstractSphinx::isRunning() const
{
    return m_running == Running;
}

bool AbstractSphinx::start()
{
    Q_D (AbstractSphinx);
    qDebug() << "[AbstractSphinx::start()] Starting...";

    if (isRunning()) {
        qDebug() << "[AbstractSphinx::start()] Already started.";
        return true;
    }

    if (isReady()) {
        d->m_bus = d->m_pipeline->bus();
        d->m_bus->addSignalWatch();
        QGlib::connect (d->m_bus, "message::application", this, &AbstractSphinx::applicationMessage);

        d->m_pipeline->setState (QGst::StatePlaying);
        m_running = Running;
        qDebug() << "[AbstractSphinx::start()] PocketSphinx started.";
    }
    else {
        qWarning() << "[AbstractSphinx::start()] Object is not ready to run.";
    }

    return isRunning();
}

bool AbstractSphinx::stop()
{
    Q_D (AbstractSphinx);

    if (d->m_pipeline->setState (QGst::StateNull) == QGst::StateChangeSuccess) {
        m_running = NotPrepared;
        d->m_bus.clear();
    }

    qDebug() << "[AbstractSphinx::stop()] Has PocketSphinx halted?" << (m_running == NotPrepared);
    return m_running == NotPrepared;
}

void AbstractSphinx::togglePause()
{
    Q_D (AbstractSphinx);
    const bool silent = ! (d->m_vader->property ("silent").toBool());
    d->m_vader->setProperty ("silent", silent);
}

void AbstractSphinx::formPartialResult (QString& p_text, QString& p_uttid)
{
    Q_D (AbstractSphinx);
    QGst::Structure psStructure ("partial_result");
    psStructure.setValue ("hyp", p_text);
    psStructure.setValue ("uttid", p_uttid);
    QGst::MessagePtr message = QGst::ApplicationMessage::create (d->m_psphinx, psStructure);
    d->m_bus->post (message);
}

/// @todo Does this obtain the partial result from the internal PocketSphinx component?
void AbstractSphinx::formResult (QString& p_text, QString& p_uttid)
{
    Q_D (AbstractSphinx);
    QGst::Structure psStructure ("result");
    psStructure.setValue ("hyp", p_text);
    psStructure.setValue ("uttid", p_uttid);
    QGst::MessagePtr l_message = QGst::ApplicationMessage::create (d->m_psphinx, psStructure);
    d->m_bus->post (l_message);
}

AbstractSphinx::~AbstractSphinx()
{
}

#include "sphinx/abstract.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
