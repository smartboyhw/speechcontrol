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

AdaptionUtility::AdaptionUtility() : QObject(), m_session ( 0 ) {

}

AdaptionUtility::AdaptionUtility ( Session* p_session ) : QObject(), m_session ( p_session ) {

}

Session* AdaptionUtility::session() {
    return m_session;
}

void AdaptionUtility::setSession ( Session* p_session ) {
    m_session = p_session;
}

void AdaptionUtility::adapt() {
    if ( !m_session )
        return;

    generateFeatures();
    generateMixtureWeights();
    convertModelDefinitions();
    collectAcousticStatistics();
    performAdaption();
    generateSendmap();
    generateAccuracyReport();
}

void AdaptionUtility::generateFeatures() {

}

void AdaptionUtility::generateMixtureWeights() {

}

void AdaptionUtility::convertModelDefinitions() {

}

void AdaptionUtility::collectAcousticStatistics() {

}

void AdaptionUtility::performAdaption ( ) {

}

void AdaptionUtility::generateSendmap() {

}

void AdaptionUtility::generateAccuracyReport() {

}

AdaptionUtility::~AdaptionUtility() {

}

#include "adaptionutility.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
