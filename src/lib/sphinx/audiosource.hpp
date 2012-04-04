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

#ifndef SPCHCNTRL_LIB_SPHINX_AUDIOSOURCE_HPP
#define SPCHCNTRL_LIB_SPHINX_AUDIOSOURCE_HPP

#include <QObject>
#include <QGst/Message>

#include <lib/sphinx/abstract.hpp>

namespace SpeechControl
{
class AudioSourceSphinx;
class AudioSourceSphinxSource;
class AudioSourceSphinxPrivate;
class AbstractAudioSource;

class SPCH_EXPORT AudioSourceSphinx : public AbstractSphinx
{
    Q_OBJECT
    Q_DISABLE_COPY (AudioSourceSphinx)
    Q_DECLARE_PRIVATE (AudioSourceSphinx)
    friend class AudioSourceSphinxSource;

public:
    explicit AudioSourceSphinx (QObject* p_parent = 0);
    AudioSourceSphinx (AbstractAudioSource* p_source, QObject* p_parent = 0);
    virtual ~AudioSourceSphinx();
    AbstractAudioSource* source();
    void setSource (AbstractAudioSource* p_source);
    virtual bool start();
    virtual bool stop();

protected slots:
    virtual void applicationMessage (const QGst::MessagePtr& p_message);
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
