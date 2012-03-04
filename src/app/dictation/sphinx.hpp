/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
 *  Copyright (C) 2012 Jacky Alcine <jacky.alcine@thesii.org>
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

#ifndef DESKTOPASR_HPP
#define DESKTOPASR_HPP

#include <lib/abstractsphinx.hpp>

namespace SpeechControl {
namespace Dictation {

/**
 * @brief Represents the AbstractSphinx object for Dictation.
 *
 * This AbstractSphinx instance is specifically tailored for the purpose of
 * dictation.
 *
 * @note This class is not meant to be used for typical programming. It's used exclusively by Dictation::Agent.
 * @see Dictation::Agent
 **/
class Sphinx : public AbstractSphinx {
    Q_OBJECT
    Q_DISABLE_COPY ( Sphinx )
    friend class Agent;

protected:
    /**
     * @brief Null constructor.
     *
     * @param p_parent ... Defaults to 0.
     **/
    explicit Sphinx ( QObject* p_parent = 0 );

    /**
     * @brief Constructor.
     *
     * @param p_description The description for the pipeline to build.
     * @param p_parent Defaults to 0.
     **/
    Sphinx ( const QString& p_description, QObject* p_parent = 0 );

    /**
     * @brief Constructor.
     *
     * @param p_pipeline The pipeline to use.
     * @param p_parent Defaults to 0.
     **/
    Sphinx ( QGst::PipelinePtr p_pipeline, QObject* p_parent = 0 );

public slots:
    void applicationMessage ( const QGst::MessagePtr& p_message );
};

}
}

#endif // DESKTOPASR_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
