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
    _psphinx = _pipeline->getElementByName (_gstElements.value("pocketsphinx").toStdString().c_str());
    _vader   = _pipeline->getElementByName (_gstElements.value("vader").toStdString().c_str());
    
    QGlib::connect (_psphinx, "partial_result", this, &ASR::asrPartialResult);
    QGlib::connect (_psphinx, "result", this, &ASR::asrResult);
    _psphinx->setProperty ("configured", true);
    
    _bus = _pipeline->bus();
    _bus->addSignalWatch();
    QGlib::connect (_bus, "message::application", this, &ASR::applicationMessage);
    
    _pipeline->setState (QGst::StateReady);
    _state = Ready;
}

ASR::ASR (QObject* parent) : QObject (parent)
{
    _state = NotReady;
}

ASR::ASR (QGst::PipelinePtr pipeline, QObject* parent) : QObject (parent), _pipeline(pipeline)
{
    _state = NotReady;
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const char* description, QObject* parent) : QObject (parent)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description);
    _pipeline->add (bin);

    _gstElements.insert("pocketsphinx", "asr");
    _gstElements.insert("vader", "vad");
    
    _prepare();
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const QString& description, QObject* parent) : QObject (parent)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description.toStdString().c_str());
    _pipeline->add (bin);

    _gstElements.insert("pocketsphinx", "asr");
    _gstElements.insert("vader", "vad");
    
    _prepare();
}

ASR::ASR (const QMap< QString, QString >& elementMap, QObject* parent) : QObject (parent)
{
    _gstElements = elementMap;
    _pipeline = QGst::Pipeline::create();
    for (auto item = elementMap.constBegin(); item != elementMap.constEnd(); ++item) {
        if (item.key() == "pocketsphinx") {
                _psphinx = QGst::ElementFactory::make("pocketsphinx", item.value().toStdString().c_str());
                _pipeline->add(_psphinx);
        }
        else if (item.key() == "vader") {
                _vader = QGst::ElementFactory::make("vader", item.value().toStdString().c_str());
                _pipeline->add(_vader);
        }
        else {
            qWarning() << "[ASR] Unrecognized element passed; ignoring.";
        }
    }
    if (_psphinx && _vader)
        _prepare();
    else
        _state = NotReady;
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

const QGst::PipelinePtr ASR::getPipeline(
)const

{
    return _pipeline;
}

const QGst::ElementPtr ASR::getPocketSphinx(
)const

{
    return _psphinx;
}

const QGst::ElementPtr ASR::getVader(
)const

{
    return _vader;
}

const QGst::BusPtr ASR::getBus(
)const

{
    return _bus;
}

template<typename T>
void ASR::setPsProperty (const QString& property, const T& value)
{
    _psphinx->setProperty(property.toStdString().c_str(), value);
}

template<typename T>
void ASR::setVaderProperty (const QString& property, const T& value)
{
    _vader->setProperty(property.toStdString().c_str(), value);
}

bool ASR::ready() const
{
    return _state == Ready;
}

void ASR::run()
{
    if (ready())
        _pipeline->setState(QGst::StatePlaying);
    else
        qWarning() << "[ASR] Object is not ready to run.";
}

void ASR::pause()
{
    _vader->setProperty("silent", true);
}

void ASR::stop()
{
    _pipeline->setState(QGst::StatePaused);
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

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;
