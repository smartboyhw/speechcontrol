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

#include <stdio.h>

#include <QDir>
#include <QDebug>
#include <QTextStream>

#include <corpus.hpp>
#include <sentence.hpp>
#include "session.hpp"

#define CHUNK_SIZE 250

using namespace SpeechControl;

QMap<QUuid, QDomElement*> Session::s_elems;
QDomDocument* Session::s_dom = 0;
ContentMap Content::s_lst;

Session::Session ( const QUuid& p_uuid ) : m_corpus ( 0 ), m_content ( 0 ), m_elem ( 0 ) {
    load ( p_uuid );
}

Session::~Session() {
}

Corpus * Session::corpus() const {
    return m_corpus;
}

/// @todo Invoke the progress of this session when loaded.
void Session::setCorpus ( Corpus* l_corpus ) {
    if ( l_corpus ) {
        m_corpus = l_corpus;
        assessProgress();
    } else {
        qDebug() << "Null corpus not added.";
    }
}

Content* Session::content() const {
    return m_content;
}

void Session::setContent ( Content* p_content ) {
    m_content = p_content;
    assessProgress();
}

void Session::assessProgress() {
    double l_progress = 0.0;

    Q_FOREACH ( const Sentence* l_snt, corpus()->sentences() ) {
        l_progress += l_snt->completedProgress();
    }

    emit progressChanged ( l_progress / ( double ) ( corpus()->sentences().count() ) );
}

void Session::init() {
    qDebug() << "Loading sessions...";
    QFile* l_file = new QFile ( QDir::homePath() + "/.speechcontrol/sessions.xml" );
    s_elems.clear();

    if ( s_dom ) {
        s_dom->clear();
        delete s_dom;
    }

    s_dom = new QDomDocument ( "Sessions" );

    if ( l_file->exists() ) {
        l_file->open ( QIODevice::ReadOnly );
        s_dom->setContent ( l_file );

        const QDomElement l_domElem = s_dom->documentElement();
        const QDomNodeList l_domList = l_domElem.elementsByTagName ( "Session" );

        for ( int i = 0; i < l_domList.count(); i++ ) {
            QDomElement l_node = l_domList.at ( i ).toElement();
            const QUuid l_uuid ( l_node.attribute ( "uuid" ) );
            s_elems.insert ( l_uuid, new QDomElement ( l_domList.at ( i ).toElement() ) );
        }

        qDebug() << l_domList.count() << "sessions loaded.";
    } else {
        l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
        QDomElement l_elem = s_dom->createElement ( "Sessions" );
        s_dom->appendChild ( l_elem );
        l_file->write ( s_dom->toString ( 4 ).toLocal8Bit() );
        qDebug() << "Created session listing.";
    }

    l_file->close();
}

void Session::load ( const QUuid &p_uuid ) {
    m_elem = s_elems.value ( p_uuid );
    if ( m_elem && !m_elem->isNull() ) {
        setCorpus ( Corpus::obtain ( m_elem->attribute ( "corpus" ) ) );
        setContent ( Content::obtain ( m_elem->attribute ( "content" ) ) );
    } else {
        s_elems.remove ( p_uuid );
        m_content = 0;
        m_corpus = 0;
        m_elem = 0;
    }

    qDebug() << "Is Session" << p_uuid << "valid?" << isValid();
}

bool Session::isValid() const {
    const bool l_valid = (
                             m_elem && !m_elem->isNull()
                             && m_corpus && m_corpus->isValid()
                             && m_content && m_content->isValid()
                         );
    Q_ASSERT ( l_valid == true );
    return l_valid;
}

SessionList Session::allSessions() {
    SessionList l_lst;
    Q_FOREACH ( const QUuid l_uuid, s_elems.keys() ) {
        Session* l_session = Session::obtain ( l_uuid );
        if ( l_session && l_session->isValid() )
            l_lst << l_session;
    }

    return l_lst;
}

const QUuid Session::uuid() const {
    return m_elem->attribute ( "uuid" );
}

Session* Session::obtain ( const QUuid &p_uuid ) {
    //qDebug() << "Obtaining session" << p_uuid;
    return new Session ( p_uuid );
}

/// @todo Create a new Corpus with this Session.
Session* Session::create ( const Content* p_content ) {
    const QStringList l_lst = p_content->pages().join ( "\n" ).simplified().trimmed().replace ( ".", ".\n" ).split ( "\n", QString::SkipEmptyParts );
    qDebug () << "Session has " << l_lst.length() << "potential sentences.";
    const QUuid l_uuid = QUuid::createUuid();
    Corpus* l_corpus = Corpus::create ( l_lst );

    if ( !l_corpus ) {
        qDebug() << "Failed to create corpus for session.";
        return 0;
    }

    // create element data
    QDomElement l_sessElem = s_dom->createElement ( "Session" );
    QDomElement l_dateElem = s_dom->createElement ( "Date" );

    l_dateElem.setAttribute ( "created",QDateTime::currentDateTimeUtc().toString ( Qt::SystemLocaleDate ) );
    l_dateElem.setAttribute ( "completed","-1" );
    l_sessElem.setAttribute ( "uuid", l_uuid.toString() );
    l_sessElem.setAttribute ( "content", p_content->uuid().toString() );
    l_sessElem.setAttribute ( "corpus", l_corpus->uuid() );

    s_dom->documentElement().appendChild ( l_sessElem );
    s_dom->documentElement().appendChild ( l_dateElem );

    QFile* l_file = new QFile ( QDir::homePath() + "/.speechcontrol/sessions.xml" );
    l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream l_str ( l_file );
    s_dom->save ( l_str, 4 );
    l_file->close();

    init();

    return Session::obtain ( l_uuid );
}

Session* Session::Backup::session() {
    return 0;
}

Sentence* Session::firstIncompleteSentence() const {
    const SentenceList l_lst = m_corpus->sentences();

    for ( int i = 0; i < l_lst.count(); i++ ) {
        Sentence* l_sent = l_lst.at ( i );

        if ( !l_sent->allPhrasesCompleted() )
            return l_sent;
        else {
            qDebug() << l_sent->text() << "already completed @" << l_sent->audioPath().absolutePath();
            continue;
        }
    }

    return 0;
}

Sentence* Session::lastIncompleteSentence() const {
    const SentenceList l_lst = m_corpus->sentences();
    SentenceList::ConstIterator l_endItr = l_lst.begin();

    for ( SentenceList::ConstIterator l_itr = l_lst.end(); l_itr != l_endItr; l_itr-- ) {
        const Sentence* l_sent = ( *l_itr );

        if ( !l_sent->allPhrasesCompleted() )
            return *l_itr;
        else
            continue;
    }

    return 0;
}

SentenceList Session::incompletedSentences() const {
    SentenceList l_baseLst = m_corpus->sentences();
    SentenceList l_lst;

    for ( SentenceList::Iterator l_itr = l_baseLst.begin(); l_itr != l_baseLst.end(); l_itr++ ) {
        Sentence* l_sent = ( *l_itr );

        if ( !l_sent->allPhrasesCompleted() )
            l_lst << l_sent;

        continue;
    }

    //qDebug() << l_lst;

    return l_lst;
}

bool Session::isCompleted() const {
    return ( incompletedSentences().empty() );
}

Session::BackupList* Session::backups() const {
    return 0;
}

void Session::erase() const {
    QUuid l_uuid ( m_elem->attribute ( "uuid" ) );
    s_elems.remove ( l_uuid );
    m_corpus->erase();
    s_dom->documentElement().removeChild ( *m_elem );

    QFile* l_file = new QFile ( QDir::homePath() + "/.speechcontrol/sessions.xml" );
    l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream l_strm ( l_file );

    s_dom->save ( l_strm,4 );

    qDebug() << "Session" << l_uuid << "removed.";
}

Session::Backup* Session::createBackup() const {
    return Backup::generate ( *this );
}

void Session::setName ( const QString& p_name ) {
    if ( !p_name.isEmpty() && !p_name.isEmpty() )
        m_elem->setAttribute ( "Name",p_name );
}

const QString Session::name() const {
    if ( m_elem->hasAttribute ( "Name" ) )
        return m_elem->attribute ( "Name" );

    return content()->title();
}


Session* Session::clone() const {
    QUuid l_uuid = QUuid::createUuid();
    Corpus* l_corpus = m_corpus->clone();
    QDomElement l_elem = m_elem->cloneNode ( true ).toElement();
    l_elem.attribute ( "uuid",l_uuid.toString() );
    l_elem.attribute ( "corpus",l_corpus->uuid() );
    l_elem.namedItem ( "Date" ).toElement().setAttribute ( "created",QDateTime::currentDateTimeUtc().toString ( Qt::SystemLocaleDate ) );
    s_dom->documentElement().appendChild ( l_elem );
    s_elems.insert ( l_uuid,&l_elem );
    return Session::obtain ( l_uuid );
}

Session::Backup::Backup() : m_dom ( 0 ) {
}

Session::Backup::~Backup() {

}

QDateTime Session::Backup::created() {
    QString l_time = m_dom->documentElement().attribute ( "DateCreated" );
    return QDateTime::fromString ( l_time );
}

const QString Session::Backup::getPath ( const QString& p_id ) {
    return QDir::homePath() + "/.speechcontrol/backups/" + p_id + ".bckp";
}

/// @todo Implement a means of backing up a session's data to a compressed document.
Session::Backup* Session::Backup::generate ( const Session& p_sssn ) {
    QDomDocument l_dom ( "Backup" );
    QDateTime l_tm = QDateTime::currentDateTimeUtc();
    QString l_id = p_sssn.uuid().toString() + "_" + QString::number ( l_tm.toMSecsSinceEpoch() );
    QDomElement l_domElem = l_dom.appendChild ( l_dom.createElement ( "Backup" ) ).toElement();
    QFile* l_file = new QFile ( getPath ( l_id ) );
    l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );

    // Obtain session data.
    const QString l_sssnStr = p_sssn.m_elem->text();
    const QByteArray l_sssnData = qCompress ( l_sssnStr.toUtf8() );

    // Compress corpus data.
    const Corpus* l_corpus = p_sssn.corpus();
    QFile* l_corpusFile = new QFile ( Corpus::getPath ( l_corpus->uuid() ).toLocalFile() );
    QByteArray l_corpusData;
    l_corpusData = qCompress ( l_corpusFile->readAll() );

    QDomElement l_sssnElem = l_dom.createElement ( "Session" );
    l_domElem.appendChild ( l_sssnElem );
    l_sssnElem.appendChild ( l_dom.createTextNode ( l_sssnData.toBase64() ) );

    QDomElement l_crpsElem = l_dom.createElement ( "Corpus" );
    l_domElem.appendChild ( l_crpsElem );
    l_crpsElem.appendChild ( l_dom.createTextNode ( l_corpusData.toBase64() ) );

    //qDebug() << l_domElem.text();
    return 0;
}

Content::Content ( const QUuid& p_uuid ) {
    load ( p_uuid );
}

Content::Content ( const Content& p_other ) : QObject(),
    m_pages ( p_other.m_pages ), m_dom ( p_other.m_dom ), m_uuid ( p_other.m_uuid ) {
}

Content* Content::obtain ( const QUuid &p_uuid ) {
    qDebug() << p_uuid;
    Q_ASSERT ( !p_uuid.isNull() );
    if ( p_uuid.isNull() ) {
        qDebug() << "Null UUID passed.";
        return 0;
    }

    if ( !s_lst.contains ( p_uuid ) ) {
        Content* l_content = new Content ( p_uuid );
        qDebug() << "Is content valid? " << l_content->isValid();
        if ( !l_content->isValid() )
            return 0;

        s_lst.insert ( p_uuid, ( l_content ) );
    }

    return s_lst.value ( p_uuid );
}

void Content::erase() {
    QFile* l_file = new QFile ( getPath ( m_uuid ) );
    if ( l_file->remove() )
        this->deleteLater();
}

void Content::load ( const QUuid &p_uuid ) {
    m_uuid = p_uuid;
    QFile* l_file = new QFile ( getPath ( m_uuid ) );
    QDomDocument* l_dom = new QDomDocument ( "Content" );

    if ( !l_file->exists() ) {
        qDebug() << "Content" << m_uuid << "doesn't exist.";
        m_uuid = QUuid();
        return;
    }

    if ( !l_file->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << "Couldn't open Content" << m_uuid << ";" << l_file->errorString();
        m_uuid = QUuid();
        return;
    }

    {
        QString l_errorMsg;
        int l_errorLine, l_errorColumn;
        if ( !l_dom->setContent ( l_file, &l_errorMsg ,&l_errorLine ,&l_errorColumn ) ) {
            qDebug() << "Couldn't parse Content;" << l_errorMsg << "on l." << l_errorLine << "; col." << l_errorColumn;
            m_uuid = QUuid();
            return;
        }
    }

    //qDebug() << "DOM:" << l_dom->toString();
    QDomElement l_domElem = l_dom->documentElement();
    QDomElement l_textElem = l_domElem.namedItem ( "Text" ).toElement();
    const QString l_textBase ( QByteArray::fromBase64 ( l_textElem.text().toUtf8() ) );
    parseText ( l_textBase );

    if ( m_pages.length() == 0 ) {
        m_uuid = QUuid();
        qDebug() << "No text was found from" << l_textBase;
        return;
    }

    m_dom = l_dom;
}

/// @todo Implement a proper means of segmenting text into chunks.
void Content::parseText ( const QString& p_text ) {
    QString l_tmpText;
    uint l_count = 0;

    Q_FOREACH ( const QChar l_chr, p_text ) {
        if ( l_count == CHUNK_SIZE ) {
            if ( l_chr.isLetterOrNumber() )
                l_count -= 1;
            else {
                m_pages << l_tmpText;
                l_tmpText.clear();
                l_count = 0;
            }
        }

        l_tmpText.append ( l_chr );

        ++l_count;
    }
}

bool Content::isValid() const {
    if ( !QFile::exists ( getPath ( m_uuid ) ) ) {
        qDebug() << "Content's data file doesn't exists." << getPath ( m_uuid );
        Q_ASSERT ( QFile::exists ( getPath ( m_uuid ) ) == true );
        return false;
    }

    if ( m_pages.isEmpty() ) {
        qDebug() << "Content has no pages." << m_uuid;
        Q_ASSERT ( m_pages.isEmpty() == true );
        return false;
    }

    return true;
}

/// @todo Determine a means of finding the page count.
/// @todo Define the size of one page.
uint Content::pageCount() const {
    return m_pages.count();
}

/// @todo Determine a means of finding the words in the text.
uint Content::words() const {
    return m_pages.join ( "\n" ).split ( " " ).count();
}

uint Content::length() const {
    return m_pages.join ( "\n" ).length();
}

/// @todo Determine a means of finding the count of alpha-numeric characters.
uint Content::characters() const {
    return m_pages.join ( "\n" ).length();
}

QString Content::getPath ( const QUuid &p_uuid ) {
    return QDir::homePath() + "/.speechcontrol/contents/" + p_uuid.toString() + ".xml";
}

const QString Content::title() const {
    QDomElement l_domElem = m_dom->documentElement();
    QDomElement l_bilboElem = l_domElem.namedItem ( "Bibliography" ).toElement();
    return l_bilboElem.attribute ( "Title" );
}

const QString Content::author() const {
    QDomElement l_domElem = m_dom->documentElement();
    QDomElement l_bilboElem = l_domElem.namedItem ( "Bibliography" ).toElement();
    return l_bilboElem.attribute ( "Author" );
}

Content::~Content() {
}

const QUuid Content::uuid() const {
    return m_uuid;
}

ContentList Content::allContents() {
    ContentList l_lst;
    QDir l_dir ( QDir::homePath() + "/.speechcontrol/contents/" );
    l_dir.setFilter ( QDir::Files );
    QStringList l_results = l_dir.entryList ( QStringList() << "*" );

    Q_FOREACH ( const QString l_uuid, l_results ) {
        Content* l_content = Content::obtain ( QUuid ( l_uuid ) );
        qDebug () << "Is content null?" << ( l_content == 0 );
        qDebug () << "Is content" << l_uuid << "valid?" << l_content->isValid();

        if ( l_content && l_content->isValid() )
            l_lst << l_content;
        else
            QFile::remove ( l_dir.absoluteFilePath ( l_uuid ) );
    }

    return l_lst;
}

const QStringList Content::pages() const {
    return m_pages;
}

const QString Content::pageAt ( const int &l_index ) const {
    if ( l_index < m_pages.count() )
        return m_pages.at ( l_index );

    return QString::null;
}

Content * Content::create ( const QString &p_author, const QString &p_title, const QString &p_content ) {
    QUuid l_uuid = QUuid::createUuid();
    qDebug() << "Creating content with UUID " << l_uuid << "...";

    QDomDocument* l_dom = new QDomDocument ( "Content" );
    QDomElement l_domElem = l_dom->createElement ( "Content" );
    l_domElem.setAttribute ( "Uuid", l_uuid );
    l_dom->appendChild ( l_domElem );

    QDomElement l_bilboElem = l_dom->createElement ( "Bibliography" );
    l_bilboElem.setAttribute ( "Author", p_author );
    l_bilboElem.setAttribute ( "Title", p_title );
    l_domElem.appendChild ( l_bilboElem );

    QDomElement l_textElem = l_dom->createElement ( "Text" );
    QDomText l_textNode = l_dom->createTextNode ( p_content.toUtf8().toBase64() );
    l_textElem.appendChild ( l_textNode );
    l_domElem.appendChild ( l_textElem );

    QFile* l_file = new QFile ( Content::getPath ( l_uuid ) );
    l_file->open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream l_strm ( l_file );
    l_dom->save ( l_strm, 4 );
    l_file->close();

    //qDebug() << "Content XML:" << l_dom->toString();
    return Content::obtain ( l_uuid );
}

#include "session.moc"

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

