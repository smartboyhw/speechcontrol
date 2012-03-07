/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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

namespace SpeechControl
{
namespace DesktopControl
{

/**
 * @brief Represents the Sphinx instance for DesktopControl.
 *
 * @note This class is not meant for casual programming. It's solely used in DesktopControl::Agent.
 * @internal
 **/
class Sphinx : public AbstractSphinx
{
    Q_OBJECT
    Q_DISABLE_COPY (Sphinx)

protected:
    /**
     * @brief Null constructor.
     *
     * @param p_parent Defaults to NULL.
     **/
    explicit Sphinx (QObject* p_parent = 0);

    /**
     * @brief Constructor.
     *
     * @param p_description The description used to build the pipeline.
     * @param p_parent Defaults to 0.
     **/
    Sphinx (const QString& p_description, QObject* p_parent = 0);
    /**
     * @brief Constructor.
     *
     * @param p_pipeline The pipeline to be used.
     * @param p_parent Defaults to 0.
     **/
    Sphinx (QGst::PipelinePtr p_pipeline, QObject* p_parent = 0);

private slots:
    void applicationMessage (const QGst::MessagePtr& message);
};

}
}

#endif // DESKTOPASR_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
