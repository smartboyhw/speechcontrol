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
#include <acousticmodel.hpp>

#include <QDebug>
#include <QProcess>

using namespace SpeechControl;


AdaptationUtility::AdaptationUtility() : QObject(), m_session (0), m_modelBase (0), m_modelResult(0)
{
    qWarning() << "[AdaptationUtility] Initialized with null objects.";
}

AdaptationUtility::AdaptationUtility (Session* p_session, AcousticModel* p_model) : QObject(), m_session (p_session), m_modelBase (p_model), m_modelResult(0)
{

}

Session* AdaptationUtility::session()
{
    return m_session;
}

AcousticModel* AdaptationUtility::baseModel()
{
    return m_modelBase;
}

AcousticModel* AdaptationUtility::resultingModel()
{
    return m_modelResult;
}

void AdaptationUtility::setSession (Session* p_session)
{
    m_session = p_session;
}

void AdaptationUtility::setAcousticModel (AcousticModel* p_model)
{
    m_modelBase = p_model;
}

AcousticModel* AdaptationUtility::adapt()
{
    if (!m_session || !m_modelBase)
        return 0;

    copyAcousticModel();
    generateFeatures();
    generateMixtureWeights();
    convertModelDefinitions();
    collectAcousticStatistics();
    performAdaptation();
    generateSendmap();
    generateAccuracyReport();

    return 0;
}

void AdaptationUtility::copyAcousticModel()
{
    m_modelResult = m_modelBase->clone();
}

void AdaptationUtility::generateFeatures()
{

}

void AdaptationUtility::generateMixtureWeights()
{

}

void AdaptationUtility::convertModelDefinitions()
{

}

void AdaptationUtility::collectAcousticStatistics()
{

}

void AdaptationUtility::performAdaptation ()
{

}

void AdaptationUtility::generateSendmap()
{

}

void AdaptationUtility::generateAccuracyReport()
{

}

AdaptationUtility::~AdaptationUtility()
{

}

#include "adaptionutility.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
