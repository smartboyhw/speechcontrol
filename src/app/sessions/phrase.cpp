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

#include "phrase.hpp"
#include "corpus.hpp"

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
    qDebug() << p_corpus->m_dom->toString();
    return p_corpus->addPhrase (new Phrase (p_corpus, elem));
}

bool Phrase::isCompleted() const
{
    //qDebug() << "[Phrase::isCompleted()] Audio exists?" << audio()->exists() << audio()->fileName();
    return audio()->exists();
}

const QString Phrase::text() const
{
    return m_elem->attribute("text").toLocal8Bit();
}

QFile* Phrase::audio() const
{
    const QString fileName = m_corpus->audioPath() + QString::number (index()) + ".raw";
    //qDebug() << "[Phrase::audio()]" << fileName;
    return new QFile (fileName);
}

uint Phrase::index() const
{
    return m_elem->attribute ("index").toInt();
}

uint Phrase::words() const
{
    return text().split (" ").length();
}

Phrase::~Phrase()
{

}

#include "phrase.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
