/***
 *  This file is part of the SpeechControl project.
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
 *  You should have received a copy of the GNU Library General Public
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 22:47:48 PM
 */

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDomDocument>

#include "app/core.hpp"
#include "app/global.hpp"
#include "content.hpp"
#include "contentprivate.hpp"

using namespace SpeechControl;
ContentMap ContentPrivate::s_lst;

ContentPrivate::ContentPrivate(Content* p_Qptr) : q_ptr(p_Qptr) {

}

ContentPrivate::~ContentPrivate() {

}

QString ContentPrivate::getPath (const QString& p_id) {
    return Core::configurationPath().path() + "/contents/" + p_id + ".spch";
}

ContentList ContentPrivate::findAllContents (QString p_path)
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

void ContentPrivate::parseText (const QString& p_text)
{
    QString tmpText;
    uint count = 0;

    // This method of breaking every X letters are so seemed like a good idea
    // at the time. If you have a more efficient way to determine a single
    // chunk of text that'd be remotely uniform in size between other Content
    // objects, be my guest.
    Q_FOREACH (const QChar chr, p_text) {
        if (count == CONTENT_CHUNK_SIZE) {
            if (chr.isLetterOrNumber()) {
                count -= 1;
            }
            else {
                pages << tmpText;
                tmpText.clear();
                count = -1;
            }
        }

        tmpText.append (chr);

        ++count;
    }

    // While the above algorithm divides text into chunks ("pages"),
    // this one generates a list of sentences.
    QString fakeText;
    QStringList lines = p_text.split('\n');
    Q_FOREACH (QString line, lines) {
        line = line.remove(QRegExp("\\s+$"));
        line = line.remove("\\[[0-9]+\\]");

        if (line.isEmpty())
            continue;

        if (line.at(line.length() - 1) != '.')
            fakeText.append(line + ' ');
        else
            fakeText.append(line + '\n');
    }
    fakeText = fakeText.replace("([a-zA-Z0-9]{3}\\. )", "\\1\n");
    utterances = fakeText.split('\n');
}

QString ContentPrivate::path() const
{
    // I chose to use the q-pointer here just in case a future implementation
    // changes how IDs are loaded for corpus objects.

    Q_Q(const Content);
    return ContentPrivate::getPath(q->id());
}

Content::Content (const QString& p_id) : d_ptr(new ContentPrivate(this))
{
    load (p_id);
}

Content::Content (const Content& p_other) : QObject(),
    d_ptr(new ContentPrivate(this))
{
    Q_D(Content);
    d->dom = p_other.d_func()->dom;
    d->id = p_other.d_func()->id;
}

Content* Content::obtain (const QString& p_id)
{
    qDebug() << "[Content::obtain()] Potential Content ID:" << p_id;

    if (!ContentPrivate::s_lst.contains (p_id)) {
        Content* content = new Content (p_id);
        qDebug() << "[Content::obtain()] Is content valid? " << content->isValid();
        if (!content->isValid())
            return 0;

        ContentPrivate::s_lst.insert (p_id, (content));
        qDebug() << "[Content::obtain()] Content" << p_id << "rendered.";
        return content;
    }

    qDebug() << "[Content::obtain()] Pre-existing Content" << p_id << "obtained.";
    return ContentPrivate::s_lst.value (p_id);
}

Content* Content::obtainFromFile (const QUrl& p_url)
{
    qDebug() << "[Content::obtainFromFile()] Potential Content URL:" << p_url;

    Content* content = new Content (QString::null);
    QFile* file = new QFile (p_url.toString());

    if (file->open (QIODevice::ReadOnly | QIODevice::Text)) {
        content->load (file);
        qDebug() << "[Content::obtainFromFile()] Is content valid? " << content->isValid();
        Q_ASSERT (content->isValid());
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
    Q_D(Content);
    QFile* file = new QFile (d->path());

    if (file->remove()) {
        this->deleteLater();
        qDebug() << "[Content::erase()] Erased content" << id();
    }
    else {
        qWarning() << "[Content::erase()] Error erasing Content" << id() << file->errorString();
    }
}

void Content::load (const QString& p_id)
{
    Q_D(Content);
    d->id = p_id;
    QFile* file = new QFile (d->path());

    if (!file->exists()) {
        qWarning() << "[Content::load()] Content" << id() << "doesn't exist.";
        d->id = QString();
        d->dom = 0;
        d->pages = QStringList();
        return;
    }

    if (!file->open (QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[Content::load()] Couldn't open Content" << id() << ";" << file->errorString();
        d->id = QString();
        d->dom = 0;
        d->pages = QStringList();
        return;
    }

    load (file);
}

void Content::load (QFile* p_file)
{
    Q_D(Content);
    QDomDocument* dom = new QDomDocument ("Content");
    QTextStream strm (p_file);
    strm.setCodec ("UTF-8");

    {
        QString errorMsg;
        int errorLine, errorColumn;

        if (!dom->setContent (strm.readAll() , &errorMsg , &errorLine , &errorColumn)) {
            qWarning() << "[Content::load()] Couldn't parse Content;" << errorMsg << "on l." << errorLine << "; col." << errorColumn;
            d->id = QString();
            return;
        }
    }

    qDebug() << "[Content::load()] DOM:" << dom->toString();
    QDomElement domElem = dom->documentElement();
    QDomElement textElem = domElem.namedItem ("Text").toElement();
    const QString textBase (textElem.text());
    d->parseText (textBase);

    if (pages().length() == 0) {
        d->id = QString();
        qWarning() << "[Content::load()] No text was found from" << textBase;
        return;
    }

    d->dom = dom;
    p_file->close();
    d->id = QFileInfo (*p_file).baseName();
    ContentPrivate::s_lst.insert (id(), this);
}

bool Content::isValid() const
{
    Q_D(const Content);
    if (id().isNull() || id().isEmpty()) {
        qDebug() << "[Content::isValid()] ID of Content is null." << id();
        return false;
    }
    else if (d->pages.isEmpty()) {
        qDebug() << "[Content::isValid()] Content has no pages." << id();
        return false;
    }
    else if (!d->dom || d->dom->isNull()) {
        qDebug() << "[Content::isValid()] DOM element of Corpus is null." << id();
        return false;
    }

    return true;
}

uint Content::pageCount() const
{
    Q_D(const Content);
    return d->pages.count();
}

uint Content::words() const
{
    return pages().join ("\n").split (" ").count();
}

uint Content::uniqueWords() const
{
    QStringList words = pages().join ("\n").split (" ");
    words.removeDuplicates();
    return words.count();
}

uint Content::length() const
{
    return pages().join ("\n").length();
}

uint Content::characters() const
{
    /// @todo Provide a more accurate calculation; from a cached value.
    return pages().join ("\n").length();
}

QString Content::title() const
{
    Q_D(const Content);
    if (isValid()) {
        QDomElement domElem = d->dom->documentElement();
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
    Q_D(const Content);
    if (isValid()) {
        QDomElement domElem = d->dom->documentElement();
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
    Q_D(const Content);
    return d->id;
}

ContentList Content::allContents()
{
    ContentList lst;
    lst.append (ContentPrivate::findAllContents (Core::configurationPath().path() + "/contents/"));
    lst.append (ContentPrivate::findAllContents (SPCHCNTRL_SYSTEM_CONTENT_DIR));

    return lst;
}

QStringList Content::pages() const
{
    Q_D(const Content);
    return d->pages;
}

int Content::uttNumber() const
{
    Q_D(const Content);
    return d->utterances.length();
}

QString Content::getUtterance(int id) const
{
    Q_D(const Content);
    return d->utterances.at(id);
}

QStringList Content::getUtteranceSeq(int begin, int end) const
{
    Q_D(const Content);
    QStringList seq;
    for (int i = begin; i <= end; ++begin)
        seq << d->utterances.at(i);
    return seq;
}

QString Content::pageAt (const int& p_index) const
{
    if (p_index < pages().count()) {
        return pages().at (p_index).trimmed();
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

    QFile file (ContentPrivate::getPath (id));

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
    Q_ASSERT (nabbedContent != 0);

    return nabbedContent;
}

AbstractContentSourcePrivate::AbstractContentSourcePrivate(AbstractContentSource* p_Qptr) : id(), author(),

    text(), title(), q_ptr(p_Qptr) {
}

AbstractContentSourcePrivate::~AbstractContentSourcePrivate() {

}

AbstractContentSource::AbstractContentSource (QObject* p_parent) :
    QObject (p_parent), d_ptr(new AbstractContentSourcePrivate(this))
{

}

AbstractContentSource::AbstractContentSource (QString p_id, QObject* p_parent) :
    QObject (p_parent), d_ptr(new AbstractContentSourcePrivate(this))
{

}

/// @todo Implement copy constructor.
AbstractContentSource::AbstractContentSource (const AbstractContentSource& p_other) :
    QObject (p_other.parent()), d_ptr(new AbstractContentSourcePrivate(this))
{
}

QString AbstractContentSource::id() const
{
    Q_D(const AbstractContentSource);
    return d->id;
}

QString AbstractContentSource::author() const
{
    Q_D(const AbstractContentSource);
    return d->author;
}

QString AbstractContentSource::text() const
{
    Q_D(const AbstractContentSource);
    return d->text;
}

QString AbstractContentSource::title() const
{
    Q_D(const AbstractContentSource);
    return d->title;
}

void AbstractContentSource::setAuthor (const QString& p_author)
{
    Q_D(AbstractContentSource);
    d->author = p_author;
}

void AbstractContentSource::setText (const QString& p_text)
{
    Q_D(AbstractContentSource);
    d->text = p_text;
}

void AbstractContentSource::setTitle (const QString& p_title)
{
    Q_D(AbstractContentSource);
    d->title = p_title;
}

Content* AbstractContentSource::generate()
{
    Content* content = Content::create (author(), title(), text());
    Q_ASSERT (content != 0);

    return content;
}

bool AbstractContentSource::isValid()
{
    return (!author().isNull() && !author().isEmpty()) &&
           (!text().isNull() && !text().isEmpty()) &&
           (!title().isNull() && !title().isEmpty());
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
// kate: indent-mode cstyle; replace-tabs on;
