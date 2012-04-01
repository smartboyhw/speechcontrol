/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#ifndef PHRASE_HPP
#define PHRASE_HPP

#include <QList>
#include <QString>
#include <QObject>

#include <lib/export.hpp>

class QFile;
class QDomElement;

namespace SpeechControl
{

class Corpus;
class Phrase;
class Corpus;

typedef QList<Phrase*> PhraseList;

class SPCH_EXPORT Phrase : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Phrase)
    friend class Corpus;

public:
    virtual ~Phrase();
    QFile* audio() const;
    const QString text() const;
    uint words() const;
    uint index() const;
    bool isCompleted() const;
    QString audioPath() const;
    static Phrase* create(Corpus* p_corpus, const QString& p_text);

private:
    explicit Phrase (Corpus* p_corpus, const QDomElement* p_elem);
    const QDomElement* m_elem;
    Corpus* m_corpus;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
