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

#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include <QObject>
#include <QString>

#include <qindicateindicator.h>

class QImage;

namespace SpeechControl {
class Indicator;

/**
 * @brief ...
 **/
class Indicator : public QObject {
    Q_OBJECT

public:
    /**
     * @brief ...
     *
     **/
    virtual ~Indicator();
    /**
     * @brief ...
     *
     * @return void
     **/
    static void show();
    /**
     * @brief ...
     *
     * @return void
     **/
    static void hide();
    /**
     * @brief ...
     *
     * @param p_message ...
     * @return void
     **/
    static void showMessage ( const QString& p_message );
    /**
     * @brief ...
     *
     * @return :Indicator*
     **/
    static Indicator* instance();

private:
    /**
     * @brief ...
     *
     * @param parent ... Defaults to 0.
     **/
    explicit Indicator ( QObject* parent = 0 );
    QIndicate::Indicator* m_indctr;     ///< Indicator instance.
    static Indicator* s_inst;           ///< Singleton instance.
};
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
