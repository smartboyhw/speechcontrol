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
#include <QList>
#include <QUuid>
#include <QObject>

#include <export.hpp>

class QDomElement;

namespace SpeechControl {
class Phrase;
class Corpus;
class Sentence;

/**
 * @brief Represents a @c QList of @c Sentence objects.
 **/
typedef QList<Sentence*> SentenceList;

/**
 * @brief Represents a @c QList of @c Phrase objects; components of a @c Sentence object.
 **/
typedef QList<Phrase*> PhraseList;


class SPCH_EXPORT Sentence : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( Sentence )
    friend class Corpus;
    friend class Phrase;

public:
    /**
     * @brief ...
     *
     **/
    virtual ~Sentence();
    /**
     * @brief ...
     *
     * @param p_corpus ...
     * @param p_text ...
     * @return :Sentence*
     **/
    static Sentence* create ( SpeechControl::Corpus* p_corpus, const QString& p_text );
    /**
     * @brief ...
     *
     * @return :Corpus*
     **/
    Corpus* parentSession() const;
    /**
     * @brief ...
     *
     * @return const QUuid
     **/
    const QUuid uuid() const;
    /**
     * @brief ...
     *
     * @return const QString
     **/
    const QString text() const;
    /**
     * @brief ...
     *
     * @return const QDir
     **/
    const QDir audioPath() const;
    /**
     * @brief ...
     *
     * @return :PhraseList
     **/
    const PhraseList phrases() const;
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool allPhrasesCompleted() const;
    /**
     * @brief ...
     *
     * @param  ...
     * @return bool
     **/
    bool isPhraseCompleted ( const int& ) const;
    /**
     * @brief ...
     *
     * @return double
     **/
    double completedProgress() const;
    /**
     * @brief ...
     *
     * @return int
     **/
    int index() const;
    /**
     * @brief ...
     *
     * @param  ...
     * @return :Phrase*
     **/
    Phrase* phrase ( const int& ) const;

private:
    explicit Sentence ( Corpus*, QDomElement* );
    QDomElement* getPhraseElement ( const int& ) const;
    QDomElement* m_elem;
    Corpus* m_corpus;
    PhraseList m_phrsLst;
};

}

#endif // SENTENCE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
