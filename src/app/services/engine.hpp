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
 * @date 05/16/12 22:21:08 PM
 */

#ifndef SPCHCNTRL_SERVICES_ENGINE_HPP
#define SPCHCNTRL_SERVICES_ENGINE_HPP

#include <QMap>
#include <QObject>
#include <QScopedPointer>
#include <app/global.hpp>

class QPixmap;

SPCHCNTRL_BEGIN_NAMESPACE

class AbstractServiceModule;
class ServiceEngine;
struct ServiceEnginePrivate;

typedef QList<AbstractServiceModule*> ServiceModuleList;

/**
 * @brief Represents the core Service engine.
 *
 * Engine holds the integral part of dispatching the addition and removal of
 * modules in SpeechControl.
 **/
class ServiceEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (ServiceEngine)
    Q_DECLARE_PRIVATE(ServiceEngine)
    SPCHCNTRL_SINGLETON (ServiceEngine)
    friend class AbstractServiceModule;
    QScopedPointer<ServiceEnginePrivate> d_ptr;

signals:
    /**
     * @brief Emitted when the Engine's started.
     **/
    void started();

    /**
     * @brief Emitted when the Engine's stopped.
     **/
    void stopped();

public:
    /**
     * @brief Searches for a AbstractModule.
     *
     * @param p_id The ID of the AbstractModule in question.
     * @return A pointer to a AbstractModule if found, NULL otherwise.
     **/
    static AbstractServiceModule* findModule (const QString& p_id);

    /**
     * @brief Obtains all of the AbstractModules registered.
     *
     * @return :Services::ModuleList
     **/
    static ServiceModuleList allModules();

    /**
     * @brief Registers a AbstractModule.
     *
     * @param p_module The Module to add to the Engine.
     **/
    static void registerModule (AbstractServiceModule* p_module);

    /**
     * @brief Unregisters a AbstractModule.
     *
     * @param p_module The Module to remove to the Engine.
     **/
    static void unregisterModule (AbstractServiceModule* p_module);

public slots:
    /**
     * @brief Starts the Engine.
     **/
    static void start();

    /**
     * @brief Stops the Engine.
     **/
    static void stop();
};

SPCHCNTRL_END_NAMESPACE
#endif
// kate: indent-mode cstyle; replace-tabs on;
