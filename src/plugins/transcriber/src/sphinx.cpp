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

#include <QGst/Pad>
#include <QGlib/Connect>

#include <core.hpp>
#include "sphinx.hpp"

using namespace SpeechControl::Plugins::Transcriber;

Sphinx::Sphinx() : AbstractSphinx (SpeechControl::Core::instance())
{

}

void Sphinx::prepareForFile (const QString& p_path)
{
    QString desc = standardDescription();
    desc.replace ("autoaudiosrc name=audiosrc ! audioconvert"
                    " ! audioresample ! audiorate ",
                    "filesrc name=audiosrc ! decodebin name=decoder ");
    buildPipeline (desc);
    audioSrcElement()->setProperty<const char*> ("location", p_path.toStdString().c_str());
    QGst::ElementPtr decodebin = pipeline()->getElementByName ("decoder");
    QGlib::connect (decodebin, "unknown-type", this, &Sphinx::onUnknownTypeEncountered);
}

void Sphinx::applicationMessage (const QGst::MessagePtr& p_message)
{
    QString msgType    = p_message->internalStructure()->name();
    QString hypothesis = p_message->internalStructure()->value ("hyp").toString();
    QString uttid      = p_message->internalStructure()->value ("uttid").toString();

    if (msgType == "result") {
        qDebug() << "ASR result:" << hypothesis << uttid;
        emit finished (hypothesis);
    }
}

void Sphinx::onUnknownTypeEncountered (QGst::PadPtr p_pad, QGst::CapsPtr p_caps)
{
    qDebug() << "Unknown type encountered." << p_caps->toString()
             << p_pad->pathString();

    Q_FOREACH (QGlib::ParamSpecPtr prop, p_pad->listProperties()) {
        qDebug() << prop->name() << prop->description() << prop->nick() << p_pad->property (prop->name().toStdString().c_str());
    }

    Q_FOREACH (QGlib::ParamSpecPtr prop, pocketSphinxElement()->listProperties()) {
        qDebug() << prop->name() << prop->description() << prop->nick() << pocketSphinxElement()->property (prop->name().toStdString().c_str());
    }
}

Sphinx::~Sphinx()
{

}

#include "sphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
