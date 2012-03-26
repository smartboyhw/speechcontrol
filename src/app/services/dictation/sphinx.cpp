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

#include "sphinx.hpp"

using namespace SpeechControl::Dictation;

Sphinx::Sphinx (QObject* p_parent) : AbstractSphinx (p_parent)
{

}

Sphinx::Sphinx (const QString& p_description, QObject* p_parent) : AbstractSphinx (p_description, p_parent)
{

}

Sphinx::Sphinx (QGst::PipelinePtr p_pipeline, QObject* p_parent) : AbstractSphinx (p_pipeline, p_parent)
{

}

void Sphinx::applicationMessage (const QGst::MessagePtr& p_message)
{
    QString msgType    = p_message->internalStructure()->name();
    QString hypothesis = p_message->internalStructure()->value ("hyp").toString();
    QString uttid      = p_message->internalStructure()->value ("uttid").toString();

    if (msgType == "result") {
        qDebug() << "[Dictation::Sphinx::applicationMessage()] Result:" << hypothesis;
        emit finished (hypothesis);
    }
}

#include "services/dictation/sphinx.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
