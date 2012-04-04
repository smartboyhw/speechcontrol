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

#include "lib/audiosource/abstract.hpp"
#include "lib/audiosource/source.hpp"
#include "lib/sphinx/audiosource.hpp"

using namespace SpeechControl;

AudioSourceSphinx::AudioSourceSphinx (QObject* p_parent) : AbstractSphinx (p_parent), m_audioSrc (0), m_appSrc (0)
{
}

AudioSourceSphinx::AudioSourceSphinx (AbstractAudioSource* p_source, QObject* p_parent) : AbstractSphinx (p_parent), m_audioSrc(p_source) {
    setSource (p_source);
}


AudioSourceSphinx::AudioSourceSphinx (const AudioSourceSphinx& p_other) : AbstractSphinx (p_other.parent()), m_audioSrc (0) {
    setSource (p_other.m_audioSrc);
}


void AudioSourceSphinx::linkSource ()
{
    QString description = standardDescription();
    description = description.replace ("autoaudiosrc name=src", "appsrc name=src");
    buildPipeline (description);
    m_appSrc = new AudioSourceSphinxSource (this);
    qDebug() << "[AudioSourceSphinx::linkSource()] Linked up sources.";
}

void AudioSourceSphinx::setSource (AbstractAudioSource* p_source)
{
    m_audioSrc = p_source;
    linkSource();
}

AbstractAudioSource* AudioSourceSphinx::source()
{
    return m_audioSrc;
}

bool AudioSourceSphinx::start()
{
    m_audioSrc->start();
    return SpeechControl::AbstractSphinx::start();
}

bool AudioSourceSphinx::stop()
{
    m_audioSrc->stop();
    return SpeechControl::AbstractSphinx::stop();
}

void AudioSourceSphinx::applicationMessage (const QGst::MessagePtr& p_message)
{
    QString msgType    = p_message->internalStructure()->name();
    QString hypothesis = p_message->internalStructure()->value ("hyp").toString();
    QString uttid      = p_message->internalStructure()->value ("uttid").toString();

    if (msgType == "result") {
        qDebug() << "[AudioSourceSphinx::applicationMessage()] Obtained hypothesis" << hypothesis << "from user an utterance of " << uttid << ".";
        emit finished (hypothesis);
    }
}

AudioSourceSphinx::~AudioSourceSphinx()
{

}

#include "sphinx/audiosource.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
