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

#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include <QDir>
#include <QUuid>
#include <QObject>

class QDomElement;

namespace SpeechControl {
    class Corpus;
    class Phrase;
    class Sentence;

    typedef QList<Sentence*> SentenceList;
    typedef QList<Phrase*> PhraseList;

    class Phrase : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Phrase)
        friend class Sentence;

    public:
        virtual ~Phrase();
        QFile* audio() const;
        const QString text() const;
        int index() const;
        bool isCompleted() const;

    private:
        explicit Phrase(const Sentence*, const int& );
        const Sentence* m_sntnc;
        const int m_indx;
    };

    class Sentence : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Sentence)
        friend class Corpus;
        friend class Phrase;

    public:
        virtual ~Sentence();
        static Sentence* create(Corpus* , const QString& );
        Corpus* parentSession() const;
        const QUuid uuid() const;
        const QString text() const;
        const QDir audioPath() const;
        const PhraseList phrases() const;
        bool allPhrasesCompleted() const;
        bool isPhraseCompleted(const int& ) const;
        int index() const;
        Phrase* phrase(const int& ) const;

    private:
        explicit Sentence(Corpus*, QDomElement* );
        QDomElement* getPhraseElement(const int& ) const;
        QDomElement* m_elem;
        Corpus* m_corpus;
        PhraseList m_phrsLst;
    };

}

#endif // SENTENCE_HPP
