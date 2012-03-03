/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#ifndef ACOUSTICMODEL_HPP
#define ACOUSTICMODEL_HPP

#include <QObject>
#include <QVariant>

#include <export.hpp>

namespace SpeechControl {
class AcousticModel;

typedef QList<AcousticModel*> AcousticModelList;

/**
 * @brief Represents an acoustic model for Sphinx.
 *
 * This class provides API for setting and getting meta-data of
 * acoustic models. The main property is a path where the model is stored.
 * This and all other properties are contained in the QVariantMap, inside the object.
 *
 * Currently supported parameters are:
 * @li path - Path where the model is stored.
 *
 * @note Should we use enum rather than QString for keys?
 * @todo Add more properties to use.
 */
class SPCH_EXPORT AcousticModel : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY ( AcousticModel )
    Q_PROPERTY ( const QVariantMap Parameters READ parameters WRITE setParameters )
    Q_PROPERTY ( const quint16 SampleRate READ sampleRate )

private:
    QVariantMap m_params;   ///< Holds the properties of the model.

public:
    /**
     * @brief Destructor.
     **/
    virtual ~AcousticModel();

    /**
     * @brief Constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit AcousticModel ( QObject* p_parent = 0 );

    /**
     * @brief Constructor.
     * @param p_path The path to the acoustic model.
     * @param p_parent Defaults to 0.
     **/
    AcousticModel ( QString const& p_path, QObject* p_parent = 0 );

    /**
     * @brief Sets a paramater within the acoustic model.
     *
     * @param p_key The key to change.
     * @param p_value The new value of the key to change.
     **/
    void setParameter ( const QString& p_key , const QVariant& p_value );

    /**
     * @brief Sets an array of parameters within the acoustic model.
     *
     * @param p_values The values and their corresponding keys to change.
     **/
    void setParameters ( const QVariantMap& p_values );

    /**
     * @brief Merges the passed parameters p_params with the one of this model.
     *
     * @param p_params The parameters to merge.
     **/
    void mergeParameters ( const QVariantMap& p_params );

    /**
     * @brief Obtains the value of the key.
     *
     * @param p_key The key of the desired value.
     * @return A QVariant of the value. If the key wasn't found, then the returned QVariant is invalid.
     **/
    QVariant parameter ( const QString& p_key ) const;

    /**
     * @brief Obtains all of the parameters of the acoustic model.
     *
     * @return A QVariantMap of all of the values.
     **/
    QVariantMap parameters() const;

    /**
     * @brief Obtains the sample rate of the acoustic model.
     *
     * @note This always returns 16 KHz (16,000).
     * @return 16,000 KHz (kilohertz) as an quint16.
     **/
    quint16 sampleRate() const;

    /**
     * @brief Obtains the path to the acoustic model.
     * @return The path to the acoustic model's directory.
     **/
    QString path() const;

    /**
     * @brief Determines if the acoustic model is valid.
     *
     * @return boolean
     **/
    bool isValid() const;

};

/**
 * @brief Represents a list of acoustic models.
 **/
typedef QList<AcousticModel*> AcousticModelList;

}

#endif // ACOUSTICMODEL_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
