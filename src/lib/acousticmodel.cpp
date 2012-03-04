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

#include <QDir>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QStringList>

#include "noisedictionary.hpp"
#include "acousticmodel.hpp"

using SpeechControl::AcousticModel;

AcousticModel::AcousticModel ( const AcousticModel &p_mdl ) :
    QObject ( p_mdl.parent() ), m_params ( p_mdl.m_params ), m_path ( p_mdl.m_path ) {
}
AcousticModel::AcousticModel ( const QString& p_path, QObject* p_parent ) : QObject ( p_parent ) {
    load ( p_path );
}

AcousticModel::AcousticModel ( QObject* p_parent ) : QObject ( p_parent ) {

}

AcousticModel::~AcousticModel() {
}

void AcousticModel::load ( QString p_path ) {
    QDir l_dir ( p_path );
    if ( !l_dir.exists() )
        return;

    m_path = p_path;

    loadFeatureParameters();
    loadNoiseDictionary();
}

void AcousticModel::loadFeatureParameters() {
    QFile* l_file = new QFile ( m_path + "/feat.params" );
    l_file->open ( QIODevice::ReadOnly | QIODevice::Text );

    QTextStream l_strm ( l_file );

    while ( !l_strm.atEnd() ) {
        const QStringList l_tokens = l_strm.readLine().split ( " " );
        qDebug() << "Parsing parameter" << l_tokens[0] << "=" << l_tokens[1];
        setParameter ( l_tokens[0],l_tokens[1] );
    }

    l_file->close();
}

void AcousticModel::loadNoiseDictionary() {
    QFile* l_noiseDictFile = new QFile ( m_path +  "/noisedict" );
    m_noisedict = NoiseDictionary::fromFile ( l_noiseDictFile );
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
    return m_path;
}

bool AcousticModel::isValid() const {
    return ( QDir ( m_path ) ).exists();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
#include "acousticmodel.moc"
