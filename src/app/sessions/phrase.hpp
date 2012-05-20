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

/**
 * @brief A list of Phrase objects.
 **/
typedef QList<Phrase*> PhraseList;

/**
 * @brief A phrase of text, as part of a speech corpus.
 *
 * The text representation uses DOM document which is written to
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
     * @brief Obtain the audio QFile of this phrase.
     **/
    QFile* audioFile() const;

    /**
     * @brief Obtain the path to the audio file.
     **/
    QString filePath() const;

    /**
     * @brief Obtain the text of this Phrase.
     **/
    const QString text() const;

    /**
     * @brief Obtain the number of words in this Phrase.
     **/
    quint8 words() const;

    /**
     * @brief Obtain the index of this Phrase.
     **/
    quint8 index() const;

    /**
     * @brief Determine if this Phrase has been recorded.
     **/
    bool recorded() const;

    /**
     * @brief Create a new Phrase for a corpus.
     *
     * Create a new Phrase located in some Corpus with given text.
     *
     * @param p_corpus The Corpus for this new Phrase.
     * @param p_text The text of this Phrase.
     **/
    static Phrase* create (Corpus* p_corpus, const QString& p_text);

private:
    explicit Phrase (Corpus* p_corpus, const QDomElement* p_elem);
    const QDomElement* m_elem;         ///< The QDomElement of the Phrase.
    Corpus* m_corpus;                  ///< The Corpus of this Phrase.
};

}

#endif
// kate: indent-mode cstyle; replace-tabs on; 
