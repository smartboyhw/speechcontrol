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

#include <lib/sphinx/abstract.hpp>

namespace SpeechControl
{

struct AbstractSphinxPrivate {
public:
    Q_DECLARE_PUBLIC (AbstractSphinx)
    explicit AbstractSphinxPrivate (AbstractSphinx* p_qPtr) :
        m_running (AbstractSphinx::NotPrepared),
        m_ready (AbstractSphinx::NotPrepared),
        q_ptr (p_qPtr) {
        clear();
    }

    virtual ~AbstractSphinxPrivate() {
        clear();
    }

    QGst::PipelinePtr      m_pipeline;
    QGst::ElementPtr       m_psphinx;
    QGst::ElementPtr       m_vader;
    QGst::BusPtr           m_bus;
    AbstractSphinx::States m_running;
    AbstractSphinx::States m_ready;
    AbstractSphinx* q_ptr;

private:
    void clear() {
        if (!m_pipeline.isNull()) {
            m_pipeline->setState (QGst::StateNull);
        }

        if (!m_psphinx.isNull()) {
            m_psphinx->setState (QGst::StateNull);
        }

        if (!m_vader.isNull()) {
            m_vader->setState (QGst::StateNull);
        }

        m_pipeline.clear();
        m_bus.clear();
        m_psphinx.clear();
        m_vader.clear();
    }
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
