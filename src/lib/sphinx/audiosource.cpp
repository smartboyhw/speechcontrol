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

#include "lib/sphinx/abstract.hpp"
#include "lib/sphinx/audiosource.hpp"
#include "lib/sphinx/audiosourceprivate.hpp"
#include "lib/audiosource/abstract.hpp"

using namespace SpeechControl;

AudioSourceSphinx::AudioSourceSphinx (QObject* p_parent) :
    AbstractSphinx (new AudioSourceSphinxPrivate (this), p_parent)
{
}

AudioSourceSphinx::AudioSourceSphinx (AbstractAudioSource* p_source, QObject* p_parent) :
    AbstractSphinx (new AudioSourceSphinxPrivate (this), p_parent)
{
    setSource (p_source);
}

AudioSourceSphinx::AudioSourceSphinx (const AudioSourceSphinx& p_other) :
    AbstractSphinx (const_cast<AbstractSphinxPrivate*> (p_other.d_func()), p_other.parent())
{
}

AudioSourceSphinx::AudioSourceSphinx (const AbstractSphinx& p_base) :
    AbstractSphinx (p_base)
{

}

void AudioSourceSphinx::setSource (AbstractAudioSource* p_source)
{
    Q_D (AudioSourceSphinx);
    d->m_audioSrc = p_source;
    d->linkSource();
}

AbstractAudioSource* AudioSourceSphinx::source()
{
    Q_D (AudioSourceSphinx);
    return d->m_audioSrc;
}

bool AudioSourceSphinx::start()
{
    Q_D (AudioSourceSphinx);
    d->m_audioSrc->start();

    if (!d->m_audioSrc->isActive())
        return false;

    return AbstractSphinx::start();
}

bool AudioSourceSphinx::stop()
{
    Q_D (AudioSourceSphinx);
    d->m_audioSrc->stop();

    return AbstractSphinx::stop();
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
// kate: indent-mode cstyle; replace-tabs on; 
