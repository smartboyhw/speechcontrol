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

#include <QGst/Bus>
#include <QGst/Element>
#include <QGst/Pipeline>

namespace SpeechControl
{

class AbstractSphinx;

class AbstractSphinxPrivate;

class AbstractSphinxPrivate
{
public:
    Q_DECLARE_PUBLIC (AbstractSphinx)
    explicit AbstractSphinxPrivate (AbstractSphinx* p_qPtr) : q_ptr (p_qPtr) {
        clear();
    }

    virtual ~AbstractSphinxPrivate() {
        clear();
    }

    QGst::PipelinePtr   m_pipeline;     ///< Holds the pipeline for GStreamer.
    QGst::ElementPtr    m_psphinx;      ///< Holds our lucky PocketSphinx object.
    QGst::ElementPtr    m_vader;        ///< Holds the Vader element.
    QGst::BusPtr        m_bus;          ///< Holds the executing bus for GStreamer.
    AbstractSphinx* q_ptr;

private:
    void clear() {
        m_pipeline.clear();
        m_bus.clear();
        m_psphinx.clear();
        m_vader.clear();
    }
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
