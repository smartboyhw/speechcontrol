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
 * @date 05/16/12 21:38:41 PM
 */

#include <QList>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QPluginLoader>
#include <app/global.hpp>

namespace SpeechControl {

class AbstractPlugin;

struct AbstractPluginPrivate {
    Q_DECLARE_PUBLIC(AbstractPlugin)
    AbstractPluginPrivate(AbstractPlugin* p_Qptr);
    virtual ~AbstractPluginPrivate();
    /**
     * @brief Loads the library into memory.
     * @internal
     * @return TRUE if the plug-in could be loaded, FALSE otherwise.
     **/
    bool loadLibrary();

    /**
     * @brief Loads all of the plug-in's dependency plug-ins.
     * @internal
     * @return TRUE if the plug-ins were ALL loaded, FALSE otherwise.
     **/
    bool loadPlugins();

    QPluginLoader* ldr;     ///< The magical QPluginLoader!
    QString id;             ///< The ID of the plug-in.
    QList<QAction*> acts;   ///< The QActions used to add the 'Plugins' menu.
    QList<QMenu*> menus;
    AbstractPlugin* q_ptr;
};

}
