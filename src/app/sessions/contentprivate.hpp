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
 * @date 05/16/12 22:43:21 PM
 */

#include <app/global.hpp>
#include <app/sessions/content.hpp>

SPCHCNTRL_BEGIN_NAMESPACE

class Content;
class AbstractContentSource;

struct ContentPrivate {
    Q_DECLARE_PUBLIC(Content)
    ContentPrivate(Content* p_Qptr);
    virtual ~ContentPrivate();
    static QString getPath (const QString& p_id);
    static ContentList findAllContents (QString p_path);
    void parseText (const QString& p_text);
    static ContentMap s_lst;
    QString path() const;
    QStringList pages;
    QStringList utterances;
    QList<int> freeUtts;
    QDomDocument* dom;
    QString id;
    Content* q_ptr;
};

struct AbstractContentSourcePrivate {
    Q_DECLARE_PUBLIC(AbstractContentSource)
    AbstractContentSourcePrivate(AbstractContentSource* p_Qptr);
    virtual ~AbstractContentSourcePrivate();
    QString id;
    QString author;
    QString text;
    QString title;
    AbstractContentSource* q_ptr;
};

SPCHCNTRL_END_NAMESPACE
