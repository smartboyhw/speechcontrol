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

#include "corpus.hpp"
#include "sentence.hpp"
#include "dictionary.hpp"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QDomDocument>

using SpeechControl::Corpus;
using SpeechControl::Sentence;
using SpeechControl::Dictionary;
using SpeechControl::CorpusList;
using SpeechControl::SentenceList;

Corpus::Corpus ( const QUuid& p_uuid ) : QObject(), m_dom ( new QDomDocument ) {
    load ( p_uuid );
}

/// @todo This list has to be ordered from '0' to 'n-1'.
/// @note Since this sorts the values by index, the value returned should be called once and stored into a variable, to prevent overhead.
/// @todo Cache the ordered listing and update it whenever it's modified.
SentenceList Corpus::sentences() const {
    return m_sntncLst;
}

/// @todo Add the Sentence to the XML document and then to the list.
Sentence* Corpus::addSentence ( Sentence* p_sentence ) {
    m_sntncLst << p_sentence;
    return p_sentence;
}

Sentence* Corpus::addSentence ( const QString& p_text, const QFile* p_audio ) {
    qDebug() << "Adding sentence" << p_text << "...";
    Sentence* l_sentence = Sentence::create ( this, p_text );

    if ( p_audio ) {
        l_sentence->m_elem->attribute ( QUrl::fromLocalFile ( p_audio->fileName() ).toString() );
    }

    return l_sentence;
}

Corpus & Corpus::operator << ( Sentence* p_sentence ) {
    this->addSentence ( p_sentence );
    return *this;
}

/// @todo Just invoke the above method.
Corpus & Corpus::operator << ( SentenceList& p_sentenceList ) {
    Q_FOREACH ( Sentence* l_phrs, p_sentenceList )
    this->addSentence ( l_phrs );

    return *this;
}

/// @todo Build a dictionary from the said text.
/// @todo Find a way to keep the text in an ordinal fashion.
Corpus * Corpus::create ( const QStringList& p_text ) {
    QUuid l_uuid = QUuid::createUuid();
    QDir l_dir;
    if ( !l_dir.mkpath ( getPath ( l_uuid ).toLocalFile() ) ) {
        qWarning() << "Can't make corpus" << l_uuid;
        return 0;
    }

    QDomDocument l_dom ( "Corpus" );
    QDomElement l_rootElem = l_dom.createElement ( "Corpuses" );
    QDomElement l_dateElem = l_dom.createElement ( "Timing" );
    QDomElement l_sentencesElem = l_dom.createElement ( "Sentences" );

    l_rootElem.setAttribute ( "Uuid",l_uuid.toString() );
    l_rootElem.setAttribute ( "Version",0.01 );
    l_dateElem.setAttribute ( "Created",QDateTime::currentDateTimeUtc().toString() );

    l_rootElem.appendChild ( l_dateElem );
    l_rootElem.appendChild ( l_sentencesElem );
    l_dom.appendChild ( l_rootElem );

    const QUrl l_path = getPath ( l_uuid );
    QFile* l_file = new QFile ( l_path.toLocalFile() + "/corpus.xml" );
    l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream l_strm ( l_file );
    l_dom.save ( l_strm,4 );

    //qDebug() << l_dom.toString();

    Corpus* l_corpus = Corpus::obtain ( l_uuid );
    Q_FOREACH ( QString l_str, p_text ) {
        l_str = l_str.simplified().trimmed();
        if ( l_str.isEmpty() )
            continue;

        Sentence* l_sent = l_corpus->addSentence ( l_str ,0 );
        l_corpus->m_dom->documentElement().namedItem ( "Sentences" ).appendChild ( *l_sent->m_elem );
        qDebug() << "Added sentence" << l_corpus->sentences().count() << l_str;
    }

    l_corpus->save();
    return l_corpus;
}

bool Corpus::exists ( const QUuid& p_uuid ) {
    return QFile::exists ( getPath ( p_uuid ).toLocalFile() );
}

QUrl Corpus::getPath ( const QUuid& p_uuid ) {
    return QDir::homePath() + "/.speechcontrol/corpus/" + p_uuid.toString();
}

QUrl Corpus::audioPath() const {
    return getPath ( this->uuid() ).toLocalFile() + "/audio";
}

const QUuid Corpus::uuid() const {
    return m_dom->documentElement().attribute ( "Uuid" );
}

Corpus* Corpus::obtain ( const QUuid &p_uuid ) {
    Corpus* l_crps = 0;
    const QString l_path = getPath ( p_uuid ).toLocalFile() + "/corpus.xml";
    //qDebug() << "Obtaining corpus" << p_uuid << l_path;

    if ( !QFile::exists ( l_path ) ) {
        qDebug() << "Corpus not found at" << l_path;
        return 0;
    }

    l_crps = new Corpus ( p_uuid );
    if ( !l_crps->isValid() ) {
        qDebug() << "Invalid corpus" << p_uuid;
        return 0;
    }

    return l_crps;
}

void Corpus::load ( const QUuid &p_uuid ) {
    const QUrl l_path = getPath ( p_uuid );
    QFile* l_file = new QFile ( l_path.toLocalFile() + "/corpus.xml" );

    if ( l_file->exists() && l_file->open ( QIODevice::ReadOnly ) ) {
        if ( !m_dom ) {
            m_dom = new QDomDocument ( "Corpus" );
        }

        if ( !m_dom->setContent ( l_file ) ) {
            qDebug() << "Failed to load corpus.";
            return;
        }

        QDomNodeList l_elems = m_dom->documentElement().firstChildElement ( "Sentences" ).childNodes();

        for ( int i = 0; i < l_elems.count(); ++i ) {
            QDomElement l_elem = l_elems.at ( i ).toElement();
            qDebug() << "Loading sentence:" << l_elem.attribute ( "uuid" );

            if ( l_elem.isNull() ) {
                continue;
            }

            Sentence* l_sntc = new Sentence ( this, ( new QDomElement ( l_elems.at ( i ).toElement() ) ) );
            qDebug() << "Loaded sentence:" << l_sntc->text();
            addSentence ( l_sntc );
        }

        m_uuid = p_uuid;
    } else {
        m_dom = 0;
        m_dict = 0;
        m_sntncLst = SentenceList();
        m_uuid = QUuid ( QString::null );
        qDebug() << "Failed to open corpus XML file.";
    }
}

/// @todo What happens if this corpus has no sentences?
/// @todo The dictionary isn't checked at the moment, but should be in the future when the class is more defined.
bool Corpus::isValid() const {
    const bool l_valid = m_dom && !m_uuid.isNull();
    Q_ASSERT ( l_valid == true );
    qDebug() << "Is corpus valid?" << l_valid;
    return l_valid;
}

void Corpus::save() {
    const QUrl l_path = getPath ( this->uuid() );
    QFile* l_file = new QFile ( l_path.toLocalFile() + "/corpus.xml" );
    if ( l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        QTextStream l_strm ( l_file );
        m_dom->save ( l_strm,4 );
    } else {
        qWarning() << "Can't write to" << l_file->fileName() << ":" << l_file->errorString();
    }
}

CorpusList Corpus::allCorpuses() {
    CorpusList l_lst;
    QDir l_dir ( QDir::homePath() + "/.speechcontrol/corpus/" );
    l_dir.setFilter ( QDir::Dirs );
    QStringList l_results = l_dir.entryList ( QStringList() << "*" );
    Q_FOREACH ( const QString& l_uuid, l_results ) {
        l_lst << Corpus::obtain ( QUuid ( l_uuid ) );
    }

    return l_lst;
}

void Corpus::erase() {
    const QUrl l_path = getPath ( m_uuid );
    QDir* l_dir = new QDir ( l_path.toLocalFile() );
    l_dir->rmdir ( l_dir->absolutePath() );
}

Corpus* Corpus::clone() const {
    QUuid l_uuid = QUuid::createUuid();
    QDir l_thisDir ( QDir::homePath() + "./speechcontrol/corpus/" + m_uuid.toString() );
    QDir l_newDir ( QDir::homePath() + "./speechcontrol/corpus/" + l_uuid.toString() );
    l_newDir.mkpath ( l_newDir.absolutePath() );
    QStringList l_lst = l_newDir.entryList ( ( QStringList() << "*" ),QDir::NoDotAndDotDot | QDir::Files,QDir::NoSort );

    Q_FOREACH ( QString l_pth, l_lst ) {
        QFile* l_file = new QFile ( l_pth );
        const QString l_newPth = l_pth.replace ( m_uuid.toString(),l_uuid.toString() );
        l_file->copy ( l_newPth );
    }

    return Corpus::obtain ( l_uuid );
}

Sentence* Corpus::sentenceAt ( const int& p_index ) const {
    return m_sntncLst.at ( p_index );
}

Dictionary * Corpus::dictionary() const {
    return m_dict;
}

const QDateTime Corpus::timeStarted() const {
    return QDateTime::fromString ( m_dom->elementsByTagName ( "Date" ).at ( 0 ).toElement().attribute ( "Started" ) );
}

const QDateTime Corpus::timeLastModified() const {
    return QDateTime::fromString ( m_dom->elementsByTagName ( "Date" ).at ( 0 ).toElement().attribute ( "LastModified" ) );
}

const QDateTime Corpus::timeCompleted() const {
    return QDateTime::fromString ( m_dom->elementsByTagName ( "Date" ).at ( 0 ).toElement().attribute ( "Completed" ) );
}

/// @todo What to clean-up?
Corpus::~Corpus() {

}

#include "corpus.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
