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

#include "sphinx.hpp"
#include <pocketsphinx.h>
#include <QGst/Pad>
#include <QGlib/Connect>

using namespace SpeechControl::Plugins::Transcriber;

Sphinx::Sphinx() : AbstractSphinx ( 0 ) {

}

void Sphinx::prepareForFile ( const QString& p_path ) {
    QString l_desc = standardDescription();
    l_desc.replace ( "autoaudiosrc name=audiosrc","filesrc name=audiosrc ! decodebin name=decoder" );
    buildPipeline ( l_desc );
    audioSrcElement()->setProperty<const char*> ( "location",p_path.toStdString().c_str() );
    QGst::ElementPtr l_decodebin = m_pipeline->getElementByName ( "decoder" );
    QGlib::connect ( l_decodebin,"unknown-type",this,&Sphinx::onUnknownTypeEncountered );
    qDebug() << audioSrcElement()->property ( "location" );
}

void Sphinx::applicationMessage ( const QGst::MessagePtr& p_message ) {
    QString l_msgType    = p_message->internalStructure()->name();
    QString l_hypothesis = p_message->internalStructure()->value ( "hyp" ).toString();
    QString l_uttid      = p_message->internalStructure()->value ( "uttid" ).toString();
    if ( l_msgType == "result" ) {
        qDebug() << "ASR result:" << l_hypothesis << l_uttid;
        emit finished ( l_hypothesis );
    }
}

void Sphinx::onUnknownTypeEncountered ( QGst::PadPtr p_pad, QGst::CapsPtr p_caps ) {
    qDebug() << "Unknown type encountered." << p_caps->toString()
             << p_pad->pathString();

    Q_FOREACH ( QGlib::ParamSpecPtr l_prop, p_pad->listProperties() ) {
        qDebug() << l_prop->name() << l_prop->description() << l_prop->nick() << p_pad->property ( l_prop->name().toStdString().c_str() );
    }

    Q_FOREACH ( QGlib::ParamSpecPtr l_prop, m_psphinx->listProperties() ) {
        qDebug() << l_prop->name() << l_prop->description() << l_prop->nick() << m_psphinx->property ( l_prop->name().toStdString().c_str() );
    }
}

Sphinx::~Sphinx() {

}

#include "sphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
