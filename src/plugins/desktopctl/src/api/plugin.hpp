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
 * @date 05/13/12 3:57:42 PM
 */

#ifndef SPCHCNTRL_DSKCTL_API_PLUGIN_HPP
#define SPCHCNTRL_DSKCTL_API_PLUGIN_HPP

#include <app/plugin.hpp>
#include "global.hpp"

class QMenu;
DSKTPCTLAPI_BEGIN_NAMESPACE

/// @todo Add a opaque pointer for this.
class Plugin : public SpeechControl::AbstractPlugin
{
    Q_OBJECT
    QAction* m_actionToggle;
    QAction* m_actionOptions;
    QMenu* m_menuDesktopControl;
    void Q_SLOT doMenuOptions();
    void Q_SLOT doMenuToggle(bool& p_checked);

public:
    explicit Plugin (QObject* parent = 0);
    virtual QPixmap pixmap() const;
    virtual ~Plugin();

protected:
    virtual void initialize();
    virtual void deinitialize();
};

DSKTPCTLAPI_END_NAMESPACE

#endif
// kate: indent-mode cstyle; replace-tabs on;
