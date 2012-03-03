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
AcousticModel::AcousticModel ( const QString& p_path, QObject* p_parent ) : QObject ( p_parent ) {
    m_params.insert ( "path", p_path );
}

AcousticModel::AcousticModel ( QObject* p_parent ) : QObject ( p_parent ) {

}

AcousticModel::~AcousticModel() {
}

void AcousticModel::setParameter ( const QString &p_key, const QVariant &p_value ) {
    m_params.insert ( p_key, p_value );
}

void AcousticModel::setParameters ( QVariantMap const& p_params ) {
    m_params = p_params;
}

void AcousticModel::mergeParameters ( QVariantMap const& p_params ) {
    for ( QMap< QString, QVariant >::const_iterator it = p_params.constBegin();
            it != p_params.constEnd(); ++it ) {

        m_params.insert ( it.key(), it.value() );
    }
}

QVariant AcousticModel::parameter ( const QString &p_key ) const {
    return m_params.value ( p_key );
}

QVariantMap AcousticModel::parameters() const {
    return m_params;
}

quint16 AcousticModel::sampleRate() const {
    return 16000;
}

QString AcousticModel::path() const {
    return m_params.value ( "path" ).toString();
}

bool AcousticModel::isValid() const {
    QDir l_path ( m_params.value ( "path" ).toString() );
    return l_path.exists();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
#include "acousticmodel.moc"
