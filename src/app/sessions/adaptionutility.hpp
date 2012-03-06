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

#ifndef SPEECHCONTROL_ADAPTIONUTILITY_HPP
#define SPEECHCONTROL_ADAPTIONUTILITY_HPP

#include <QObject>

namespace SpeechControl {
class Session;

/**
 * @brief ...
 **/
class AdaptionUtility : public QObject {
    Q_OBJECT

public:
    /**
     * @brief ...
     *
     * @param p_session ...
     **/
    explicit AdaptionUtility ( Session* p_session );

    /**
     * @brief ...
     *
     **/
    AdaptionUtility();

    /**
     * @brief ...
     *
     **/

    virtual ~AdaptionUtility();

    /**
     * @brief ...
     *
     * @param p_session ...
     * @return void
     **/
    void setSession(Session* p_session);

    /**
     * @brief ...
     *
     * @return :Session*
     **/
    Session* session();

    /**
     * @brief ...
     *
     * @return void
     **/
    void adapt();

private:
    enum AdaptionMode {
        MAP = 0,
        MLLR
    };

    void generateFeatures();
    void generateMixtureWeights();
    void convertModelDefinitions();
    void collectAcousticStatistics();
    void performAdaption(const AdaptionMode& p_mode = AdaptionMode::MAP);
    void generateSendmap();
    void generateAccuracyReport();

    Session* m_session;
};

}

#endif // SPEECHCONTROL_ADAPTIONUTILITY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
