/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef ACOUSTICMODEL_HPP
#define ACOUSTICMODEL_HPP

#include <QObject>
#include <QVariant>

namespace SpeechControl
{

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

class AcousticModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY (const QVariantMap Parameters READ parameters WRITE setParameters)
    Q_PROPERTY (const quint16 SampleRate READ sampleRate WRITE setSampleRate)

    QVariantMap _parameters;
public:
    Q_DISABLE_COPY (AcousticModel)

    explicit AcousticModel(QObject* parent = 0);
    AcousticModel (QString const& path, QObject* parent = 0);
    virtual ~AcousticModel();

    void setParameter (const QString& key, const QVariant& value);
    void setParameters (const QVariantMap& parameters);

    void mergeParameters (const QVariantMap& parameters);

    QVariant parameter (const QString& key) const;
    QVariantMap parameters() const;

    quint16 sampleRate() const;
    void setSampleRate (const quint16 rate);
};
}


#endif // ACOUSTICMODEL_HPP
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
