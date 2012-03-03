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

#ifndef SPEECHCONTROL_NOISEDICTIONARY_HPP
#define SPEECHCONTROL_NOISEDICTIONARY_HPP

#include <QMap>
#include <QFile>
#include <QObject>
#include <QString>

namespace SpeechControl {
class NoiseDictionary;

typedef QMap<QString,QString> QStringMap;

/**
 * @brief ...
 **/
class NoiseDictionary : public QObject {
    Q_OBJECT

public:
    /**
     * @brief ...
     *
     * @param p_parent ... Defaults to 0.
     **/
    explicit NoiseDictionary ( QObject* p_parent = 0 );
    /**
     * @brief ...
     *
     * @param p_other ...
     **/
    NoiseDictionary ( const NoiseDictionary& p_other );
    /**
     * @brief ...
     *
     * @param p_file ...
     * @return :NoiseDictionary*
     **/
    static NoiseDictionary* fromFile ( QFile* p_file );
    /**
     * @brief ...
     *
     * @return :QStringMap
     **/
    QStringMap entries();
    /**
     * @brief ...
     *
     * @param p_entry ...
     * @return bool
     **/
    bool hasEntry ( const QString& p_entry );
    /**
     * @brief ...
     *
     * @param p_value ...
     * @return bool
     **/
    bool hasValue ( const QString& p_value );
    /**
     * @brief ...
     *
     * @param p_device ...
     * @return bool
     **/
    bool load ( QIODevice* p_device );
    /**
     * @brief ...
     *
     * @param p_entries ...
     * @return void
     **/
    void mergeEntries ( const QStringMap& p_entries );
    /**
     * @brief ...
     *
     * @param p_entry ...
     * @param p_value ...
     * @return void
     **/
    void addEntry ( const QString& p_entry, const QString& p_value );
    /**
     * @brief ...
     *
     * @return void
     **/
    void save();

private:
    QStringMap m_entries;       ///< Holds the entries.
    QIODevice* m_device;        ///< Holds the device for saving purposes.
};

}

#endif // SPEECHCONTROL_NOISEDICTIONARY_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
