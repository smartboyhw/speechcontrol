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

#include <QString>
#include <QStringList>

#include "acousticmodel.hpp"
#include <QDir>

using SpeechControl::AcousticModel;

AcousticModel::AcousticModel ( const AcousticModel &p_mdl ) :
    QObject ( p_mdl.parent() ) {
    }
AcousticModel::AcousticModel (const QString& path, QObject* parent) : QObject (parent)
{
    _parameters.insert("path", path);
}

AcousticModel::~AcousticModel() {
}

void AcousticModel::setParameter (const QString &key, const QVariant &value)
{
    _parameters.insert(key, value);
}

void AcousticModel::setParameters (QVariantMap const& parameters)
{
    _parameters = parameters;
}

void AcousticModel::mergeParameters (QVariantMap const& parameters)
{
    for (QMap< QString, QVariant >::const_iterator it = parameters.constBegin();
            it != parameters.constEnd(); ++it) {

        _parameters.insert(it.key(), it.value());
    }
}

QVariant AcousticModel::parameter (const QString &key) const
{
    return _parameters.value(key);
}

QVariantMap AcousticModel::parameters() const
{
    return _parameters;
}

quint16 AcousticModel::sampleRate() const
{
    return 16000;
}

void AcousticModel::setSampleRate ( const quint16& p_rate )
{
}

QString AcousticModel::path() const
{
    return _parameters.value("path").toString();
}

bool AcousticModel::isValid() const
{
    QDir path(_parameters.value("path").toString());
    return path.exists();
}

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;
#include "acousticmodel.moc"