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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "config.hpp"
#include "content.hpp"
#include "core.hpp"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QUrl>

using SpeechControl::Core;
using SpeechControl::Content;
using SpeechControl::ContentList;
using SpeechControl::ContentMap;
using SpeechControl::AbstractContentSource;
using SpeechControl::TextContentSource;

Content::Content (const QUuid& p_uuid)
{
    load (p_uuid);
}

Content::Content (const Content& p_other) : QObject(),
    m_pages (p_other.m_pages), m_dom (p_other.m_dom), m_uuid (p_other.m_uuid)
{
}

Content* Content::obtain (const QUuid& p_uuid)
{
    qDebug() << "[Content::obtain()] Potential Content UUID:" << p_uuid;
//     SC_ASSERT (!p_uuid.isNull(), "Invalid UUID sent to Corpus instance for obtaining.");

//     if (p_uuid.isNull()) {
//         qWarning() << "[Content::obtain()] Null UUID passed.";
//         return 0;
//     }

    if (!s_lst.contains (p_uuid)) {
        Content* l_content = new Content (p_uuid);
        qDebug() << "[Content::obtain()] Is content valid? " << l_content->isValid();
        SC_ASSERT (l_content->isValid() , "Invalid Corpus was obtained.");

        s_lst.insert (p_uuid, (l_content));
        qDebug() << "[Content::obtain()] Content" << p_uuid << "rendered.";
        return l_content;
    }

    qDebug() << "[Content::obtain()] Pre-existing Content" << p_uuid << "obtained.";
    return s_lst.value (p_uuid);
}

void Content::erase()
{
    QFile* l_file = new QFile (getPath (m_uuid));

    if (l_file->remove()) {
        this->deleteLater();
        qDebug() << "[Content::erase()] Erased content" << m_uuid;
    }
    else {
        qWarning() << "[Content::erase()] Error erasing Content" << m_uuid << l_file->errorString();
    }
}

void Content::load (const QUuid& p_uuid)
{
    m_uuid = p_uuid;
    QFile* l_file = new QFile (getPath (m_uuid));
    QDomDocument* l_dom = new QDomDocument ("Content");

    if (!l_file->exists()) {
        qWarning() << "[Content::load()] Content" << m_uuid << "doesn't exist.";
        m_uuid = QUuid::createUuid();
        return;
    }

    if (!l_file->open (QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[Content::load()] Couldn't open Content" << m_uuid << ";" << l_file->errorString();
        m_uuid = QUuid();
        return;
    }

    QTextStream l_strm (l_file);
    l_strm.setCodec ("UTF-8");

    {
        QString l_errorMsg;
        int l_errorLine, l_errorColumn;

        if (!l_dom->setContent (l_strm.readAll() , &l_errorMsg , &l_errorLine , &l_errorColumn)) {
            qWarning() << "[Content::load()] Couldn't parse Content;" << l_errorMsg << "on l." << l_errorLine << "; col." << l_errorColumn;
            m_uuid = QUuid();
            return;
        }
    }

    qDebug() << "[Content::load()] DOM:" << l_dom->toString();
    QDomElement l_domElem = l_dom->documentElement();
    QDomElement l_textElem = l_domElem.namedItem ("Text").toElement();
    const QString l_textBase (l_textElem.text());
    parseText (l_textBase);

    if (m_pages.length() == 0) {
        m_uuid = QUuid();
        qWarning() << "[Content::load()] No text was found from" << l_textBase;
        return;
    }

    m_dom = l_dom;
    l_file->close();
    s_lst.insert (m_uuid, this);
}

/// @todo Implement a proper means of segmenting text into chunks.
void Content::parseText (const QString& p_text)
{
    QString l_tmpText;
    uint l_count = 0;

    Q_FOREACH (const QChar l_chr, p_text) {
        if (l_count == CONTENT_CHUNK_SIZE) {
            if (l_chr.isLetterOrNumber()) {
                l_count -= 1;
            }
            else {
                m_pages << l_tmpText;
                l_tmpText.clear();
                l_count = -1;
            }
        }

        l_tmpText.append (l_chr);

        ++l_count;
    }
}

bool Content::isValid() const
{
    if (m_uuid.isNull()) {
        qDebug() << "[Content::isValid()] UUID of Content is null.";
        return false;
    }
    else if (!QFile::exists (getPath (m_uuid))) {
        qDebug() << "[Content::isValid()] Content's data file doesn't exists." << getPath (m_uuid);
        return false;
    }
    else if (m_pages.isEmpty()) {
        qDebug() << "[Content::isValid()] Content has no pages." << m_uuid;
        return false;
    }
    else if (!m_dom || m_dom->isNull()) {
        qDebug() << "[Content::isValid()] DOM element of Corpus is null." << m_uuid;
        return false;
    }

    return true;
}

/// @todo Determine a means of finding the page count.
/// @todo Define the size of one page.
uint Content::pageCount() const
{
    return m_pages.count();
}

uint Content::words() const
{
    return m_pages.join ("\n").split (" ").count();
}

uint Content::uniqueWords() const
{
    QStringList words = m_pages.join ("\n").split (" ");
    words.removeDuplicates();
    return words.count();
}

uint Content::length() const
{
    return m_pages.join ("\n").length();
}

/// @todo Determine a means of finding the count of alpha-numeric characters.
uint Content::characters() const
{
    return m_pages.join ("\n").length();
}

QString Content::getPath (const QUuid& p_uuid)
{
    return Core::configurationPath().path() + "/contents/" + p_uuid.toString().remove (QRegExp ("[{}]")) + ".xml";
}

QString Content::title() const
{
    if (isValid()) {
        QDomElement l_domElem = m_dom->documentElement();
        QDomElement l_bilboElem = l_domElem.namedItem ("Bibliography").toElement();
        return l_bilboElem.attribute ("Title");
    }
    else {
        qWarning() << "[Content::title()] Core DOM element is null.";
        return QString::null;
    }
}

QString Content::author() const
{
    if (isValid()) {
        QDomElement l_domElem = m_dom->documentElement();
        QDomElement l_bilboElem = l_domElem.namedItem ("Bibliography").toElement();
        return l_bilboElem.attribute ("Author");
    }
    else {
        qWarning() << "[Content::title()] Core DOM element is null.";
        return QString::null;
    }

}

Content::~Content()
{
}

QUuid Content::id() const
{
    return m_uuid;
}

ContentList Content::allContents()
{
    ContentList lst;
    lst.append(findAllContents(Core::configurationPath().path() + "/contents/"));
    lst.append(findAllContents(SPCHCNTRL_SYSTEM_CONTENT_DIR));

    return lst;
}

ContentList Content::findAllContents(QString p_path){
    ContentList lst;
    QDir dir (p_path);
    dir.setFilter (QDir::Files);
    QStringList results = dir.entryList (QStringList() << "*");

    qDebug () << "[Content::allContents()] Iterating over " << results.length() << "results...";

    Q_FOREACH (const QUuid uuid, results) {
        qDebug () << "[Content::allContents()] Parsing potential Content " << uuid;

        if (uuid.isNull()) {
            qDebug () << "[Content::allContents()] Invliad Content found at" << uuid;
            continue;
        }

        Content* l_content = Content::obtain (uuid);
        qDebug () << "[Content::allContents()] Is content null?" << (l_content == 0);
        qDebug () << "[Content::allContents()] Is content" << uuid << "valid?" << l_content->isValid();

        if (l_content && l_content->isValid()) {
            lst << l_content;
        }
        else {
            QFile::remove (dir.absoluteFilePath (uuid));
        }
    }

    qDebug () << "[Content::allContents()]" << lst.length() << " Content objects discovered.";
    return lst;
}

QStringList Content::pages() const
{
    return m_pages;
}

QString Content::pageAt (const int& p_index) const
{
    if (p_index < m_pages.count()) {
        return m_pages.at (p_index).trimmed();
    }

    return QString::null;
}

Content* Content::create (const QString& p_author, const QString& p_title, const QString& p_content)
{
    QUuid l_uuid = QUuid::createUuid();
    qDebug() << "[Content::create()] Creating content with UUID" << l_uuid << "...";

    QDomDocument l_dom ("Content");
    QDomElement l_domElem = l_dom.createElement ("Content");
    l_domElem.setAttribute ("Uuid", l_uuid);
    l_dom.appendChild (l_domElem);

    QDomElement l_bilboElem = l_dom.createElement ("Bibliography");
    l_bilboElem.setAttribute ("Author", p_author);
    l_bilboElem.setAttribute ("Title", p_title);
    l_domElem.appendChild (l_bilboElem);

    QDomElement l_textElem = l_dom.createElement ("Text");
    QDomText l_textNode = l_dom.createTextNode (p_content);
    l_textElem.appendChild (l_textNode);
    l_domElem.appendChild (l_textElem);

    QFile l_file (Content::getPath (l_uuid));

    if (!l_file.open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "[Content::create()] Failed to open file for new Content creation.";
        return 0;
    }

    QTextStream l_strm (&l_file);
    l_strm.setCodec ("UTF-8");
    l_dom.save (l_strm, 4);
    l_file.close();

    qDebug() << "[Content::create()] Content XML:" << l_dom.toString();
    Content* l_nabbedContent = Content::obtain (l_uuid);
    SC_ASSERT (l_nabbedContent != 0, "The generated Content doesn't exist!");

    return l_nabbedContent;
}

AbstractContentSource::AbstractContentSource (QString p_id, QObject* p_parent) : QObject (p_parent),
    m_id (p_id)
{

}

AbstractContentSource::AbstractContentSource (const AbstractContentSource& p_other) : QObject (p_other.parent()),
    m_id (p_other.id()), m_author (p_other.author()), m_text (p_other.text()), m_title (p_other.title())
{

}

QString AbstractContentSource::id() const
{
    return m_id;
}

QString AbstractContentSource::author() const
{
    return m_author;
}

QString AbstractContentSource::text() const
{
    return m_text;
}

QString AbstractContentSource::title() const
{
    return m_title;
}

void AbstractContentSource::setAuthor (const QString p_author)
{
    m_author = p_author;
}

void AbstractContentSource::setText (const QString p_text)
{
    m_text = p_text;
}

void AbstractContentSource::setTitle (const QString p_title)
{
    m_title = p_title;
}

Content* AbstractContentSource::generate()
{
    Content* l_content = Content::create (m_author, m_title, m_text);
    SC_ASSERT (l_content != 0, "Failed to generate Content from AbstractContentSource.");

    return l_content;
}

bool AbstractContentSource::isValid()
{
    return (!m_author.isNull() && !m_author.isEmpty()) &&
           (!m_text.isNull() && !m_text.isEmpty()) &&
           (!m_title.isNull() && !m_title.isEmpty());
}
AbstractContentSource::~AbstractContentSource()
{
}

TextContentSource::TextContentSource (QObject* p_parent) : AbstractContentSource ("text", p_parent)
{

}

TextContentSource::~TextContentSource()
{

}

/// @todo Should make a schema for this file and check it against the file.
bool TextContentSource::setFile (QFile& p_file)
{
    if (!p_file.isOpen()) {
        if (!p_file.open (QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "[TextContentSource::setFile()] Unable to open file" << p_file.fileName() << p_file.errorString();
            return false;
        }

        if (!p_file.isReadable()) {
            qDebug() << "[TextContentSource::setFile()] Unable to read file" << p_file.fileName() << p_file.errorString();
            return false;
        }
    }

    QDomDocument document ("Content");
    QString errMsg;
    int errLn, errCol;

    if (!document.setContent (&p_file, &errMsg, &errLn, &errCol)) {
        // Not a standard content file - prepare the text.
        /// @todo (Veles) Port the Python script here.
        QByteArray rawText = p_file.readAll();
        QString text (rawText);

        setText(text);
        setTitle("Unknown Title");
        setAuthor("Unknown Author");
    } else {

        const QDomElement book = document.documentElement().namedItem ("Book").toElement();

        const QString author = book.attribute ("author");

        const QString title = book.attribute ("title");

        const QString text = document.documentElement().namedItem ("Text").toElement().text();

        setText (text);

        setTitle (title);

        setAuthor (author);
    }

    return isValid();
}

/// @todo Implement support for pulling information from a HTTP(S) server (hosted file).
/// @todo Implement support for pulling information from a FTP server (hosted file).
bool TextContentSource::setUrl (const QUrl& p_url)
{
    if (!p_url.isValid()) {
        qDebug() << "[TextContentSource::setUrl()] Invalid URL" << p_url;
        return false;
    }
    else {
        if (p_url.scheme() == "http" || p_url.scheme() == "https") {
            qDebug() << "[TextContentSource::setUrl()] HTTP(S) addresses not yet supported." << p_url;
            return false;
        }
        else {
            QFile l_file (p_url.toLocalFile());
            return setFile (l_file);
        }
    }

    // Won't be reached.
    qDebug() << "[TextContentSource::setUrl()] Unknown failure.";
    return false;
}

#include "content.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
