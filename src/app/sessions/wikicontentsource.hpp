/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Adrian Borucki <gentoolx@gmail.com>
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
#ifndef WIKICONTENTSOURCE_HPP
#define WIKICONTENTSOURCE_HPP

#include <QList>
#include <QObject>

#include "content.hpp"

class QWebFrame;
class QWebPage;
class QString;

namespace SpeechControl
{

class WikiContentSource : public AbstractContentSource
{
    Q_OBJECT

public:
    explicit WikiContentSource (QObject* parent = 0);
    WikiContentSource (QString const& id, QObject* parent = 0);

    virtual ~WikiContentSource();

    bool ready() const;

    /**
     * @brief Order new sample of Wiki content.
     *
     * This method enqueues fetch of new Wiki sample replacing the current one.
     */
    void order();

signals:
    void generateReady();

public slots:
    /**
     * @brief Generate a new Content using Wiki articles.
     *
     * Use this method only after checking whether the object is ready to generate.
     * Usually you can connect @c generateReady() signal to this slot.
     *
     * @todo Make random samples.
     */
    Content* generate();

private:
    QList<QWebPage*> portalPages;
    QList<QWebFrame*> portalFrames;
    int portalSuccess;
    int portalFail;

    QList<QWebPage*> wikiPages;
    QList<QWebFrame*> wikiFrames;
    int wikiSuccess;
    int wikiFail;

private slots:
    void portalPhase();
    void wikiPhase (bool ok);
    void parsingPhase (bool ok);

};

}
#endif // WIKICONTENTSOURCE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
