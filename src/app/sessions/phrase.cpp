/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 14:54:17 PM
 */

#include "phrase.hpp"
#include "corpus.hpp"
#include <lib/audio/devicemanager.hpp>

#include <QFile>
#include <QDebug>
#include <QDomElement>

using namespace SpeechControl;

Phrase::Phrase (Corpus* p_corpus, const QDomElement* p_elem) : QObject (p_corpus), m_elem (p_elem), m_corpus (p_corpus)
{
    //qDebug() << "[Phrase::{constructor}] Phrase" << this->text() << "rendered.";
}

Phrase* Phrase::create (Corpus* p_corpus, const QString& p_text)
{
    QDomElement* elem = new QDomElement (p_corpus->m_dom->createElement ("Phrase"));
    elem->setAttribute ("index", p_corpus->phrases().count());
    elem->setAttribute ("text", p_text);
    p_corpus->m_dom->documentElement().namedItem ("Phrases").appendChild (*elem);
    return p_corpus->addPhrase (new Phrase (p_corpus, elem));
}

bool Phrase::recorded() const
{
    return audioFile()->exists();
}

const QString Phrase::text() const
{
    return m_elem->attribute ("text").toLocal8Bit();
}

QFile* Phrase::audioFile() const
{
    const QString fileName = m_corpus->audioPath() + "/phrase" + QString::number (index()) + ".wav";
    return new QFile (fileName);
}

QString Phrase::filePath() const
{
    QString pathPrefix = m_corpus->audioPath() + "/phrase" + QString::number (index());
    if (Audio::DeviceManager::audioFormat() == "Wav")
        pathPrefix.append(".wav");
    else if (Audio::DeviceManager::audioFormat() == "Ogg")
        pathPrefix.append(".ogg");
    else
        pathPrefix.append(".avi");
    return pathPrefix;
}

quint8 Phrase::index() const
{
    return m_elem->attribute ("index").toInt();
}

quint8 Phrase::words() const
{
    return text().split (" ").length();
}

Phrase::~Phrase()
{

}

#include "sessions/phrase.moc"
// kate: indent-mode cstyle; replace-tabs on;
