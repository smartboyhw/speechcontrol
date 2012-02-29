/**
 *   @file asr.cpp
 *   Copyright (C) 2012  Adrian Borucki <gentoolx@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "asr.hpp"

#include <QGlib/Connect>

#include <QGst/enums.h>
#include <QGst/ElementFactory>

using namespace SpeechControl;

void ASR::_prepare()
{
    _psphinx = _pipeline->getElementByName ("asr");
    _vader   = _pipeline->getElementByName ("vad");

    QGlib::connect (_psphinx, "partial_result", this, &ASR::asrPartialResult);
    QGlib::connect (_psphinx, "result", this, &ASR::asrResult);
//     _psphinx->setProperty ("configured", true);

    _bus = _pipeline->bus();
    _bus->addSignalWatch();
    QGlib::connect (_bus, "message::application", this, &ASR::applicationMessage);

    _pipeline->setState (QGst::StateReady);
    _state = Ready;
}

ASR::ASR (QObject* parent) : QObject (parent), _running(false)
{
    _state = NotReady;
}

ASR::ASR (QGst::PipelinePtr pipeline, QObject* parent) : QObject (parent), _pipeline(pipeline), _running(false)
{
    _state = NotReady;
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const char* description, QObject* parent) : QObject (parent), _running(false)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description);
    _pipeline->add (bin);

    _prepare();
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const QString& description, QObject* parent) : QObject (parent), _running(false)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description.toStdString().c_str());
    _pipeline->add (bin);

    _prepare();
}

ASR::~ASR()
{
    _pipeline->setState(QGst::StateNull);
}

QString ASR::getStandardDescription()
{
    QString desc ("autoaudiosrc name=audiosrc ! audioconvert");
    desc.append (" ! audioresample ! audiorate ! volume name=volume");
    desc.append (" ! vader name=vad auto_threshold=true");
    desc.append (" ! pocketsphinx name=asr");
    desc.append (" ! fakesink");
    return desc;
}

/// @todo How to deal with this decoder in GValue?
QGlib::Value ASR::getDecoder() const
{
    return _psphinx->property("decoder");
}

/// @todo Should we implement a class/struct to wrap these values more programatically?
QDir ASR::getLanguageModel() const
{
    QGlib::Value lm = _psphinx->property("lm");
    return QDir(lm.get<QString>());
}

/// @todo Rewrite the @c Dictionary class to be used here.
QDir ASR::getDictionary() const
{
    QGlib::Value dict = _psphinx->property("dict");
    return QDir(dict.get<QString>());
}

/// @todo Rewrite the @c AcousticModel class to be used here.
QDir ASR::getAcousticsModel() const
{
    QGlib::Value hmm = _psphinx->property("hmm");
    return QDir(hmm.get<QString>());
}

const QGst::PipelinePtr ASR::getPipeline() const
{
    return _pipeline;
}

const QGst::ElementPtr ASR::getPocketSphinx() const

{
    return _psphinx;
}

const QGst::ElementPtr ASR::getVader() const

{
    return _vader;
}

const QGst::BusPtr ASR::getBus() const

{
    return _bus;
}

// template<>
// void ASR::setPsProperty<QString>(const QString& property, const QString& value)
// {
//     _psphinx->setProperty(property.toStdString().c_str(), value.toStdString().c_str());
// }
//
// template<>
// void ASR::setVaderProperty<QString>(const QString& property, const QString& value)
// {
//     _vader->setProperty(property.toStdString().c_str(), value.toStdString().c_str());
// }

void ASR::setLanguageModel (const QString& path)
{
    if (QDir(path).exists())
        setPsProperty("lm", path);
    else
        qWarning() << "[ASR] Given language model path" << path << "does not exist.";
}

void ASR::setDictionary (const QString& path)
{
    if (QDir(path).exists())
        setPsProperty("dict", path);
    else
        qWarning() << "[ASR] Given dictionary path" << path << "does not exist.";
}

void ASR::setAcousticModel (const QString& path)
{

    if (QDir(path).exists())
        setPsProperty("hmm", path);
    else
        qWarning() << "[ASR] Given acoustic model path" << path << "does not exist.";
}

bool ASR::isReady() const
{
    return _state == Ready;
}

bool ASR::isRunning() const
{
    return _running;
}

bool ASR::run()
{
    qDebug() << "[ASR start]";
    if ( isReady()) {
        _pipeline->setState(QGst::StatePlaying);
        _running = true;
        return true;
    }
    else {
        qWarning() << "[ASR] Object is not ready to run.";
        return false;
    }
}

void ASR::pause()
{
    _vader->setProperty("silent", true);
}

void ASR::stop()
{
    _pipeline->setState(QGst::StatePaused);
    _running = false;
}

void ASR::asrPartialResult (const QString& text, const QString& uttid)
{
    QGst::Structure ps_structure("partial_result");
    ps_structure.setValue("hyp", text);
    ps_structure.setValue("uttid", uttid);
    QGst::MessagePtr message = QGst::ApplicationMessage::create(_psphinx, ps_structure);
    _bus->post(message);
}

void ASR::asrResult (const QString& text, const QString& uttid)
{
    QGst::Structure ps_structure("result");
    ps_structure.setValue("hyp", text);
    ps_structure.setValue("uttid", uttid);
    QGst::MessagePtr message = QGst::ApplicationMessage::create(_psphinx, ps_structure);
    _bus->post(message);
}

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;
