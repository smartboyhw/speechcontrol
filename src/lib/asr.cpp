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

using namespace SpeechControl;

ASR::ASR (QObject* parent) : QObject (parent), _pipeline (NULL), _psphinx (NULL), _bus (NULL)
{

}

ASR::ASR (QGst::PipelinePtr pipeline, QObject* parent) : QObject (parent), _pipeline (pipeline), _psphinx (NULL), _bus (NULL)
{

}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const char* description, QObject* parent) : QObject (parent)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description);
    _pipeline->add (bin);
    _psphinx = _pipeline->getElementByName ("asr");

    QGlib::connect (_psphinx, "partial_result", this, &ASR::asrPartialResult);
    QGlib::connect (_psphinx, "result", this, &ASR::asrResult);
    _psphinx->setProperty ("configured", true);

    _bus = _pipeline->bus();
    _bus->addSignalWatch();
    QGlib::connect (_bus, "message::application", this, &ASR::applicationMessage);

    _pipeline->setState (QGst::StatePaused);
}

/// @todo Automatically extract 'pocketsphinx' element name from description.
ASR::ASR (const QString& description, QObject* parent) : QObject (parent)
{
    _pipeline = QGst::Pipeline::create();
    QGst::BinPtr bin = QGst::Bin::fromDescription (description.toStdString().c_str());
    _pipeline->add (bin);
    _psphinx = _pipeline->getElementByName ("asr");

    QGlib::connect (_psphinx, "partial_result", this, &ASR::asrPartialResult);
    QGlib::connect (_psphinx, "result", this, &ASR::asrResult);
    _psphinx->setProperty ("configured", true);

    _bus = _pipeline->bus();
    _bus->addSignalWatch();
    QGlib::connect (_bus, "message::application", this, &ASR::applicationMessage);

    _pipeline->setState (QGst::StatePaused);
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

const QGst::PipelinePtr ASR::getPipeline()
{
    return _pipeline;
}

const QGst::ElementPtr ASR::getPocketSphinx()
{
    return _psphinx;
}

const QGst::BusPtr ASR::getBus()
{
    return _bus;
}

void ASR::run()
{
    _pipeline->setState(QGst::StatePlaying);
}

void ASR::pause()
{
    _pipeline->setState(QGst::StatePaused);
}

void ASR::asrPartialResult (const QGlib::Value& text, const QGlib::Value& uttid)
{
    QGst::Structure ps_structure("partial_result");
    ps_structure.setValue("hyp", text.toString());
    ps_structure.setValue("uttid", uttid.toString());
    QGst::MessagePtr message = QGst::ApplicationMessage::create(_psphinx, ps_structure);
    _bus->post(message);
}

void ASR::asrResult (const QGlib::Value& text, const QGlib::Value& uttid)
{
    QGst::Structure ps_structure("result");
    ps_structure.setValue("hyp", text.toString());
    ps_structure.setValue("uttid", uttid.toString());
    QGst::MessagePtr message = QGst::ApplicationMessage::create(_psphinx, ps_structure);
    _bus->post(message);
}

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;
