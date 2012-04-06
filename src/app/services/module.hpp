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
#ifndef SPEECHCONTROL_SERVICES_MODULE_HPP
#define SPEECHCONTROL_SERVICES_MODULE_HPP

#include <QObject>
#include <QList>
#include <QPixmap>

namespace SpeechControl
{

namespace Services
{

class AbstractModule;
class AbstractModulePrivate;
/**
 * @brief Represents a list of Modules.
 **/
typedef QList<AbstractModule*> ModuleList;

/**
 * @brief Represents  a service module.
 *
 * Modules serve as wrappers to the interfaces that internal and external components
 * of SpeechControl can provide.
 **/
class AbstractModule : public QObject
{
    Q_OBJECT

signals:
    /**
     * @brief Emitted when this AbstractModule's started.
     **/
    void started();

    /**
     * @brief Emitted when this AbstractModule's stopped.
     **/
    void stopped();

public:
    /**
     * @brief Obtains the friendly name of this AbstractModule.
     **/
    virtual QString name() const = 0;

    /**
     * @brief Obtains the ID of the AbstractModule.
     **/
    virtual QString id() const = 0;

    /**
     * @brief Obtains the QPixmap to use with this AbstractModule.
     **/
    virtual QPixmap pixmap() const = 0;

    /**
     * @brief Determines if this AbstractModule is enabled.
     **/
    virtual bool isEnabled() const = 0;

    /**
     * @brief Determines if this AbstractModule is active.
     **/
    virtual bool isActive() const = 0;

public slots:
    /**
     * @brief Starts this AbstractModule.
     **/
    void start();

    /**
     * @brief Stops this AbstractModule.
     **/
    void stop();

protected:
    QScopedPointer<AbstractModulePrivate> d_ptr;
    Q_DISABLE_COPY (AbstractModule)
    Q_DECLARE_PRIVATE (AbstractModule)
    virtual void initialize() = 0;
    virtual void deinitialize() = 0;
    explicit AbstractModule (QObject* parent = 0);
    AbstractModule (AbstractModulePrivate& p_dd, QObject* p_parent = 0);
    virtual ~AbstractModule();
};

}

}

#endif // SPEECHCONTROL_SERVICES_MODULE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
