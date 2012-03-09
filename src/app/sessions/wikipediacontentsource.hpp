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


#ifndef WIKIPEDIACONTENTSOURCE_HPP
#define WIKIPEDIACONTENTSOURCE_HPP

#include <QObject>
#include <QString>
#include <QList>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

#include "content.hpp"

namespace SpeechControl
{

class WikipediaContentSource : public AbstractContentSource
{
    Q_OBJECT
    
public:
    explicit WikipediaContentSource (QObject* parent = 0);
    WikipediaContentSource(QString const& id, QObject* parent = 0);
    
    virtual ~WikipediaContentSource();
    
    bool ready() const;
    
    /**
     * @brief Order new sample of Wikipedia content.
     * 
     * This method enqueues fetch of new Wikipedia sample replacing the current one.
     */
    void order();
    
signals:
    void generateReady();
    
public slots:
    /**
     * @brief Generate a new Content using Wikipedia articles.
     * 
     * Use this method only after checking whether the object is ready to generate.
     * Usually you can connect @c generateReady() signal to this slot.
     * 
     * @todo Make random samples.
     */
    Content* generate();
    
private:
    QList<QWebPage *> portalPages;
    QList<QWebFrame *> portalFrames;
    int portalSuccess;
    int portalFail;
    
    QList<QWebPage *> wikiPages;
    QList<QWebFrame *> wikiFrames;
    int wikiSuccess;
    int wikiFail;
    
private slots:
    void portalPhase();
    void wikiPhase(bool ok);
    void parsingPhase(bool ok);
    
};

}
#endif // WIKIPEDIACONTENTSOURCE_HPP
