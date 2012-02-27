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

#ifndef SPHINX_HPP
#define SPHINX_HPP

// Qt includes
#include <QObject>
#include <QVariant>
#include <QVariantMap>

// local includes
#include "microphone.hpp"

// Sphinx includes
#include <pocketsphinx.h>

class QFile;

namespace SpeechControl {
class Sphinx;
class Trainer;
class AcousticModel;
class SphinxResult;

/**
  * @brief A wrapping utility class for the PocketSphinx utility.
  * @deprecated The ASR class will be used instead of this class for PocketSphinx communications.
  */
class Sphinx : public QObject {
    Q_OBJECT

signals:
    void textRecognized ( const QString& );

public:

    Q_DISABLE_COPY ( Sphinx )
    explicit Sphinx ( const AcousticModel* = 0 );
    virtual ~Sphinx();
    void setAcousticModel ( const AcousticModel* );
    AcousticModel* acousticModel() const;
    bool isListening() const;
    const QString text() const;

public slots:
    void recognizeFromFile ( const QFile* );
    void recognizeFromMicrophone ( const Microphone* = Microphone::defaultMicrophone() );
    void startRecognizing ( Microphone* = Microphone::defaultMicrophone() );
    void stopRecognizing ( Microphone* = Microphone::defaultMicrophone() );

private:
    void initialize();
    QString m_hypothesis;
    Microphone* m_mic;
    AcousticModel* m_mdl;
    ps_decoder_t *m_decoder;
    cmd_ln_t *m_config;
};

class SphinxResult : public QObject {
    Q_OBJECT
    Q_PROPERTY ( const double Confidence READ confidence )
    Q_PROPERTY ( const QString Text READ text )

public:
    SphinxResult ( Sphinx* );
    virtual ~SphinxResult();
    const QString text() const;
    double confidence() const;

private:
    double m_cnfdnc;
    QString m_txt;
    Sphinx* m_sphnx;
};
}

#endif // SPHINX_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
