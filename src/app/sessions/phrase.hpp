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

#include <QObject>
#include <QFile>

#include <export.hpp>

namespace SpeechControl
{
class Phrase;
class Sentence;

/**
 * @brief ...
 **/
typedef QList<Phrase*> PhraseList;

/**
 * @brief ...
 **/
class SPCH_EXPORT Phrase : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (Phrase)
    friend class Sentence;

public:
    /**
     * @brief ...
     *
     **/
    virtual ~Phrase();
    /**
     * @brief ...
     *
     * @return QFile*
     **/
    QFile* audio() const;
    /**
     * @brief ...
     *
     * @return const QString
     **/
    const QString text() const;

    /**
     * @brief ...
     *
     * @return int
     **/
    int words() const;

    /**
     * @brief ...
     *
     * @return int
     **/
    int index() const;
    /**
     * @brief ...
     *
     * @return bool
     **/
    bool isCompleted() const;

private:
    explicit Phrase (const Sentence*, const int&);
    const Sentence* m_sntnc;
    const int m_indx;
};

}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
