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

#include "phrase.hpp"
#include "sentence.hpp"

#include <QFile>
#include <QDomElement>

using namespace SpeechControl;

Phrase * Sentence::phrase ( const int &p_indx ) const {
    return m_phrsLst.at ( p_indx );
}

const PhraseList Sentence::phrases() const {
    return m_phrsLst;
}

Phrase::Phrase ( const Sentence *p_sntnct, const int &p_index ) :
    m_sntnc ( p_sntnct ), m_indx ( p_index ) {
    //qDebug() << "Phrase" << this->text() << "rendered.";
}

bool Phrase::isCompleted() const {
    return audio()->exists();
}

const QString Phrase::text() const {
    QDomElement* l_elem = m_sntnc->getPhraseElement ( m_indx );
    return QByteArray::fromBase64 ( l_elem->text().toAscii() );
}

QFile* Phrase::audio() const {
    const QString l_fileName = m_sntnc->getPhraseElement ( m_indx )->attribute ( "uuid" ) + ".wav";
    const QString l_pth = m_sntnc->audioPath().path();
    return new QFile ( l_pth + "/" + l_fileName );
}

int Phrase::index() const {
    return m_indx;
}

Phrase::~Phrase() {

}

#include "phrase.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
