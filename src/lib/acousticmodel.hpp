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

#include <lib/global.hpp>
#include <lib/export.hpp>

class QFile;

namespace SpeechControl {

class AcousticModel;
struct AcousticModelPrivate;
class NoiseDictionary;

/**
 * @brief Represents a list of acoustic models.
 **/
typedef QList<AcousticModel*> AcousticModelList;

/**
 * @brief Represents an acoustic model for Sphinx.
 *
 * This class provides API for setting and getting meta-data of
 * acoustic models. The main property is a path where the model is stored.
 * This and all other properties are contained in the QVariantMap, inside the object.
 *
 * @section params Parameters
 *
 * The parameters here are represented in <b>feat.params</b> of the acoustic model's
 * directory. An example dump of such a file would be the following:
 *
 * @code
- lowerf 1 *
-upperf 4000
-nfilt 20
-transform dct
-round_filters no
-remove_dc yes
-feat s2_4x
 * @endcode
 *
 * Each of these properties can be set by their key (the value to the left)
 * and update its value (the value to the right). The properties supported are
 * all of the ones recognized by PocketSphinx.
 */
class SPCH_EXPORT AcousticModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (AcousticModel)
    Q_DECLARE_PRIVATE (AcousticModel)
    Q_PROPERTY (const QVariantMap Parameters READ parameters WRITE setParameters)
    Q_PROPERTY (const quint16 SampleRate READ sampleRate)

public:
    /**
     * @brief Destructor.
     **/
    virtual ~AcousticModel();

    /**
     * @brief Constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit AcousticModel (QObject* p_parent = 0);

    /**
     * @brief Constructor.
     * @param p_path The path to the acoustic model.
     * @param p_parent Defaults to 0.
     **/
    AcousticModel (QString const& p_path, QObject* p_parent = 0);

    /**
     * @brief Sets a paramater within the acoustic model.
     *
     * @param p_key The key to change.
     * @param p_value The new value of the key to change.
     **/
    void setParameter (const QString& p_key , const QVariant& p_value);

    /**
     * @brief Sets an array of parameters within the acoustic model.
     *
     * @param p_values The values and their corresponding keys to change.
     **/
    void setParameters (const QVariantMap& p_values);

    /**
     * @brief Merges the passed parameters p_params with the one of this model.
     *
     * @param p_params The parameters to merge.
     **/
    void mergeParameters (const QVariantMap& p_params);

    /**
     * @brief Obtains the value of the key.
     *
     * @param p_key The key of the desired value.
     * @return A QVariant of the value. If the key wasn't found, then the returned QVariant is invalid.
     **/
    QVariant parameter (const QString& p_key) const;

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
     **/
    bool isValid() const;

    /**
     * @brief Determines if this AcousticModel is a system-wide model.
     **/
    bool isSystem() const;

    /**
     * @brief Determines if this AcousticModel is a user-wide model.
     **/
    bool isUser() const;

    /**
     * @brief Obtains the name of this AcousticModel.
     **/
    QString name() const;

    /**
     * @brief Loads an AcousticModel from a specific path, p_path.
     * @param p_path The path in question.
     **/
    void load (QString p_path);

    /**
     * @brief Duplicates the data of this AcousticModel.
     **/
    AcousticModel* newModel();

    /**
     * @brief Obtains a listing of all AcousticModels.
     *
     * Obtains a list of all of the system-wide and user-wide AcousticModels
     * known to SpeechControl.
     **/
    static AcousticModelList allModels();

    /**
     * @brief Obtains a file to the NoiseDictionary of this AcousticModel.
     **/
    NoiseDictionary* noiseDictionary() const;

    /**
     * @brief Obtains the path to this AcousticModel's parameters (feat.params).
     **/
    QString parameterPath() const;

    /**
     * @brief Obtains the QFile representing the binary model definitions.
     * @note This is typically found at path() + "/mdef".
     **/
    QFile* modelDefinitions() const;

    /**
     * @brief Obtains the QFile representing the mixture weights.
     * @note This is typically found at path() + "/mixture_weights".
     **/
    QFile* mixtureWeights();

    /**
     * @brief Obtains the QFile representing the sentence dump of the AcousticModel.
     * @note This is typically found at path() + "/sendump".
     **/
    QFile* senDump();

    /**
     * @brief Obtains the QFile representing the variances of the AcousticModel.
     * @note This is typically found at path() + "/variances".
     **/
    QFile* variances();
    /**
     * @brief Obtains the QFile representing the transition matrices of the AcousticModel.
     * @note This is typically found at path() + "/transition_matrices".
     **/
    QFile* transitionMatrices();

    /**
     * @brief Obtains the QFile representing the mean data of the AcousticModel.
     * @note This is typically found at path() + "/means".
     **/
    QFile* means();

    /**
     * @brief Erases this AcousticModel from disk.
     **/
    void erase();

private:
    QScopedPointer<AcousticModelPrivate> d_ptr;
};

}

#endif // ACOUSTICMODEL_HPP
// kate: indent-mode cstyle; replace-tabs on;
