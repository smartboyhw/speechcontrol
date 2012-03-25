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

#ifndef SERVICES_ENGINE_HPP
#define SERVICES_ENGINE_HPP

#include <QList>
#include <QObject>
#include <QString>
#include <macros.hpp>

class QPixmap;
namespace SpeechControl
{
namespace Services
{
class AbstractModule;
class Engine;

typedef QList<AbstractModule*> AbstractModuleList;

class AbstractModule : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(AbstractModule)

public:
    QString name() const;
    QPixmap* pixmap() const;

protected:
    AbstractModule();
};

class Engine : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Engine)
    SC_SINGLETON(Engine)
    friend class AbstractModule;

public:
    static AbstractModule* findModule(const QString& p_id);

protected:
    static void
};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
