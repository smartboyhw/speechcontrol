/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "wikipediacontentsource.hpp"

#include <QDebug>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

using namespace SpeechControl;

void WikipediaContentSource::_makeNewSample()
{
    // Let's use featured articles from Technology, Society and History portals.
    QUrl tech("http://en.wikipedia.org/wiki/Portal:Technology"),
         society("http://en.wikipedia.org/wiki/Portal:Society"),
         history("http://en.wikipedia.org/wiki/Portal:History");
         
    QList<QUrl> portalUrls;
    portalUrls.append(tech);
    portalUrls.append(society);
    portalUrls.append(history);
         
    QList<QWebPage *> pages;
    QList<QWebFrame *> portals;
    Q_FOREACH (QUrl url, portalUrls) {
        QWebPage *page = new QWebPage;
        QWebFrame *fr = page->mainFrame();
        fr->load(url);
        
        pages.append(page);
        portals.append(fr);
    }
    
    Q_FOREACH (QWebFrame *portal, portals) {
        QWebElement document = portal->documentElement();
        QWebElementCollection links = document.findAll("p");
        
        qDebug() << "We have" << links.count() << "links.";
        
        QUrl featuredUrl;
        QWebElementCollection::const_iterator link = links.constBegin();
        for (; link != links.constEnd(); ++link) {
            if ((*link).toPlainText() == "Read more...") {
                featuredUrl = QUrl((*link).attribute("href"));
                _urls.append(featuredUrl);
                break;
            }
        }
    }
}

WikipediaContentSource::WikipediaContentSource (QObject* parent) : AbstractContentSource(parent)
{

}

WikipediaContentSource::WikipediaContentSource (const QString& id, QObject* parent) : AbstractContentSource (id, parent)
{

}

WikipediaContentSource::~WikipediaContentSource()
{

}

bool WikipediaContentSource::ready() const
{
    return !_urls.empty();
}

Content* WikipediaContentSource::generate()
{
    if (!ready())
        _makeNewSample();
    
    qDebug() << "We have" << _urls.size() << "urls.";
    
    QList<QWebFrame *> frames;
    Q_FOREACH (QUrl url, _urls) {
        QWebFrame *frame = QWebPage().mainFrame();
        frame->load(url);
        frames.append(frame);
    }
    
    qDebug() << "We have" << frames.size() << "frames.";
    Q_FOREACH (QWebFrame *frame, frames) {
        qDebug() << "Text of the featured frame is:" << frame->toPlainText();
    }
    
    return Content::create("Unimplemented yet", "Unimplemented yet", "Unimplemented yet");
}
