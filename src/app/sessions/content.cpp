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

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDomDocument>

#include "core.hpp"
#include "content.hpp"

using SpeechControl::Core;

using SpeechControl::Content;
using SpeechControl::ContentMap;
using SpeechControl::ContentList;

using SpeechControl::AbstractContentSource;
using SpeechControl::TextContentSource;

Content::Content (const QString& p_id)
{
    load (p_id);
}

Content::Content (const Content& p_other) : QObject(),
    m_pages (p_other.m_pages), m_dom (p_other.m_dom), m_id (p_other.m_id)
{
}

Content* Content::obtain (const QString& p_id)
{
    qDebug() << "[Content::obtain()] Potential Content ID:" << p_id;

    if (!s_lst.contains (p_id)) {
        Content* content = new Content (p_id);
        qDebug() << "[Content::obtain()] Is content valid? " << content->isValid();
        SC_ASSERT (content->isValid() , "Invalid Corpus was obtained.");

        s_lst.insert (p_id, (content));
        qDebug() << "[Content::obtain()] Content" << p_id << "rendered.";
        return content;
    }

    qDebug() << "[Content::obtain()] Pre-existing Content" << p_id << "obtained.";
    return s_lst.value (p_id);
}

Content* Content::obtainFromFile (const QUrl& p_url)
{
    qDebug() << "[Content::obtainFromFile()] Potential Content URL:" << p_url;

    Content* content = new Content (QString::null);
    QFile* file = new QFile (p_url.toString());

    if (file->open (QIODevice::ReadOnly | QIODevice::Text)) {
        content->load (file);
        qDebug() << "[Content::obtainFromFile()] Is content valid? " << content->isValid();
        SC_ASSERT (content->isValid() , "Invalid Corpus was obtained.");
        qDebug() << "[Content::obtainFromFile()] Content" << p_url << "rendered.";
        return content;
    }
    else {
        qDebug() << "[Content::obtainFromFile()] File could not be read." << file->errorString();
        return 0;
    }

    return content;
}

void Content::erase()
{
    QFile* file = new QFile (getPath (m_id));

    if (file->remove()) {
        this->deleteLater();
        qDebug() << "[Content::erase()] Erased content" << m_id;
    }
    else {
        qWarning() << "[Content::erase()] Error erasing Content" << m_id << file->errorString();
    }
}

void Content::load (const QString& p_id)
{
    m_id = p_id;
    QFile* file = new QFile (getPath (m_id));

    if (!file->exists()) {
        qWarning() << "[Content::load()] Content" << m_id << "doesn't exist.";
        m_id = QString();
        m_dom = 0;
        m_pages = QStringList();
        return;
    }

    if (!file->open (QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[Content::load()] Couldn't open Content" << m_id << ";" << file->errorString();
        m_id = QString();
        m_dom = 0;
        m_pages = QStringList();
        return;
    }

    load (file);
}

void Content::load (QFile* p_file)
{
    QDomDocument* dom = new QDomDocument ("Content");
    QTextStream strm (p_file);
    strm.setCodec ("UTF-8");

    {
        QString errorMsg;
        int errorLine, errorColumn;

        if (!dom->setContent (strm.readAll() , &errorMsg , &errorLine , &errorColumn)) {
            qWarning() << "[Content::load()] Couldn't parse Content;" << errorMsg << "on l." << errorLine << "; col." << errorColumn;
            m_id = QString();
            return;
        }
    }

    qDebug() << "[Content::load()] DOM:" << dom->toString();
    QDomElement domElem = dom->documentElement();
    QDomElement textElem = domElem.namedItem ("Text").toElement();
    const QString textBase (textElem.text());
    parseText (textBase);

    if (m_pages.length() == 0) {
        m_id = QString();
        qWarning() << "[Content::load()] No text was found from" << textBase;
        return;
    }

    m_dom = dom;
    p_file->close();
    m_id = QFileInfo (*p_file).baseName();
    s_lst.insert (m_id, this);
}

void Content::parseText (const QString& p_text)
{
    QString tmpText;
    uint count = 0;

    Q_FOREACH (const QChar chr, p_text) {
        if (count == CONTENT_CHUNK_SIZE) {
            if (chr.isLetterOrNumber()) {
                count -= 1;
            }
            else {
                m_pages << tmpText;
                tmpText.clear();
                count = -1;
            }
        }

        tmpText.append (chr);

        ++count;
    }
}

bool Content::isValid() const
{
    if (m_id.isNull() || m_id.isEmpty()) {
        qDebug() << "[Content::isValid()] ID of Content is null." << m_id;
        return false;
    }
    else if (m_pages.isEmpty()) {
        qDebug() << "[Content::isValid()] Content has no pages." << m_id;
        return false;
    }
    else if (!m_dom || m_dom->isNull()) {
        qDebug() << "[Content::isValid()] DOM element of Corpus is null." << m_id;
        return false;
    }

    return true;
}

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

uint Content::characters() const
{
    return m_pages.join ("\n").length();
}

QString Content::getPath (const QString& p_id)
{
    return Core::configurationPath().path() + "/contents/" + p_id + ".xml";
}

QString Content::title() const
{
    if (isValid()) {
        QDomElement domElem = m_dom->documentElement();
        QDomElement bilboElem = domElem.namedItem ("Book").toElement();
        return bilboElem.attribute ("title");
    }
    else {
        qWarning() << "[Content::title()] Core DOM element is null.";
        return QString::null;
    }
}

QString Content::author() const
{
    if (isValid()) {
        QDomElement domElem = m_dom->documentElement();
        QDomElement bilboElem = domElem.namedItem ("Book").toElement();
        return bilboElem.attribute ("author");
    }
    else {
        qWarning() << "[Content::title()] Core DOM element is null.";
        return QString::null;
    }

}

Content::~Content()
{
}

QString Content::id() const
{
    return m_id;
}

ContentList Content::allContents()
{
    ContentList lst;
    lst.append (findAllContents (Core::configurationPath().path() + "/contents/"));
    lst.append (findAllContents (SPCHCNTRL_SYSTEM_CONTENT_DIR));

    return lst;
}

ContentList Content::findAllContents (QString p_path)
{
    ContentList lst;
    QDir dir (p_path);
    dir.setFilter (QDir::Files);
    QStringList results = dir.entryList (QStringList() << "*");

    qDebug () << "[Content::allContents()] Iterating over " << results.length() << "results in " << p_path;

    Q_FOREACH (const QString id, results) {
        qDebug () << "[Content::allContents()] Parsing potential Content " << id;

        if (id.isNull() || id.isEmpty()) {
            qDebug () << "[Content::allContents()] Invalid Content found at" << id;
            continue;
        }

        Content* content = Content::obtainFromFile (dir.absoluteFilePath (id));
        qDebug () << "[Content::allContents()] Is content null?" << (content == 0);
        qDebug () << "[Content::allContents()] Is content" << id << "valid?" << content->isValid();

        if (content && content->isValid()) {
            lst << content;
        }
        else {
            QFile::remove (dir.absoluteFilePath (id));
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
    QString id = QString::number (qrand());
    qDebug() << "[Content::create()] Creating content with ID" << id << "...";

    QDomDocument dom ("Content");
    QDomElement domElem = dom.createElement ("Content");
    domElem.setAttribute ("Id", id);
    dom.appendChild (domElem);

    QDomElement bilboElem = dom.createElement ("Book");
    bilboElem.setAttribute ("author", p_author);
    bilboElem.setAttribute ("title", p_title);
    domElem.appendChild (bilboElem);

    QDomElement textElem = dom.createElement ("Text");
    QDomText textNode = dom.createTextNode (p_content);
    textElem.appendChild (textNode);
    domElem.appendChild (textElem);

    QFile file (Content::getPath (id));

    if (!file.open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "[Content::create()] Failed to open file for new Content creation.";
        return 0;
    }

    QTextStream strm (&file);
    strm.setCodec ("UTF-8");
    dom.save (strm, 4);
    file.close();

    qDebug() << "[Content::create()] Content XML:" << dom.toString();
    Content* nabbedContent = Content::obtain (id);
    SC_ASSERT (nabbedContent != 0, "The generated Content doesn't exist!");

    return nabbedContent;
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

void AbstractContentSource::setAuthor (const QString& p_author)
{
    m_author = p_author;
}

void AbstractContentSource::setText (const QString& p_text)
{
    m_text = p_text;
}

void AbstractContentSource::setTitle (const QString& p_title)
{
    m_title = p_title;
}

Content* AbstractContentSource::generate()
{
    Content* content = Content::create (m_author, m_title, m_text);
    SC_ASSERT (content != 0, "Failed to generate Content from AbstractContentSource.");

    return content;
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
bool TextContentSource::setFile (QFile* p_file)
{
    if (!p_file->isOpen()) {
        if (!p_file->open (QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "[TextContentSource::setFile()] Unable to open file" << p_file->fileName() << p_file->errorString();
            return false;
        }

        if (!p_file->isReadable()) {
            qDebug() << "[TextContentSource::setFile()] Unable to read file" << p_file->fileName() << p_file->errorString();
            return false;
        }
    }

    QDomDocument document ("Content");
    QString errMsg;
    int errLn, errCol;

    if (!document.setContent (p_file, &errMsg, &errLn, &errCol)) {
        // Not a standard content file - prepare the text.
        /// @todo (Veles) Port the Python script here.
        QByteArray rawText = p_file->readAll();
        QString text (rawText);

        setText (text);
        setTitle ("Unknown Title");
        setAuthor ("Unknown Author");
    }
    else {

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
            return setFile (new QFile (p_url.toLocalFile()));
        }
    }

    // Won't be reached.
    qDebug() << "[TextContentSource::setUrl()] Unknown failure.";
    return false;
}

#include "sessions/content.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
