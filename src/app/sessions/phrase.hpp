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

#include <export.hpp>

class QFile;
class QDomElement;

namespace SpeechControl
{

class Corpus;
class Phrase;
class Corpus;

/**
 * @brief Represents a list of Phrase objects.
 **/
typedef QList<Phrase*> PhraseList;

/**
 * @brief Represents a phrase of text, as part of a speech corpus.
 **/
class SPCH_EXPORT Phrase : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Phrase)
    friend class Corpus;

public:
    /**
     * @brief Destructor.
     **/
    virtual ~Phrase();

    /**
     * @brief Obtains the audio QFile of this phrase.
     **/
    QFile* audio() const;

    /**
     * @brief Obtains the text of this Phrase.
     **/
    const QString text() const;

    /**
     * @brief Obtains the number of words in this Phrase.
     **/
    quint8 words() const;

    /**
     * @brief Obtains the index of this Phrase.
     **/
    quint8 index() const;

    /**
     * @brief Determines if this Phrase has been recorded.
     **/
    bool isCompleted() const;

    /**
     * @brief Obtains the path to the audio file.
     * @see audio()
     **/
    QString audioPath() const;

    /**
     * @brief Creates a new Phrase for a corpus.
     *
     * Creates a new Phrase with text p_text for a specific corpus, p_corpus.
     *
     * @param p_corpus The corpus obtaining this new Phrase.
     * @param p_text The QString rendering this Phrase.
     **/
    static Phrase* create (Corpus* p_corpus, const QString& p_text);

private:
    explicit Phrase (Corpus* p_corpus, const QDomElement* p_elem);
    const QDomElement* m_elem;         ///< The QDomElement of the Phrase.
    Corpus* m_corpus;                  ///< The Corpus of this Phrase.
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
