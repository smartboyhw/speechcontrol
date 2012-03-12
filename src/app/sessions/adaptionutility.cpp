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

#include "adaptionutility.hpp"

using namespace SpeechControl;

AdaptionUtility::AdaptionUtility() : QObject(), m_session (0), m_model (0)
{

}

AdaptionUtility::AdaptionUtility (Session* p_session, AcousticModel* p_model) : QObject(), m_session (p_session), m_model (p_model)
{

}

Session* AdaptionUtility::session()
{
    return m_session;
}

AcousticModel* AdaptionUtility::model()
{
    return m_model;
}

void AdaptionUtility::setSession (Session* p_session)
{
    m_session = p_session;
}

void AdaptionUtility::setAcousticModel (AcousticModel* p_model)
{
    m_model = p_model;
}

AcousticModel* AdaptionUtility::adapt()
{
    if (!m_session || !m_model)
        return 0;

    generateFeatures();
    generateMixtureWeights();
    convertModelDefinitions();
    collectAcousticStatistics();
    performAdaption();
    generateSendmap();
    generateAccuracyReport();

    return 0;
}

void AdaptionUtility::generateFeatures()
{

}

void AdaptionUtility::generateMixtureWeights()
{

}

void AdaptionUtility::convertModelDefinitions()
{

}

void AdaptionUtility::collectAcousticStatistics()
{

}

void AdaptionUtility::performAdaption ()
{

}

void AdaptionUtility::generateSendmap()
{

}

void AdaptionUtility::generateAccuracyReport()
{

}

AdaptionUtility::~AdaptionUtility()
{

}

#include "adaptionutility.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
