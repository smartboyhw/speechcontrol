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
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

class QAction;
class QSystemTrayIcon;

namespace SpeechControl
{
struct IndicatorPrivate {
    IndicatorPrivate();
    ~IndicatorPrivate();
    void buildMenu();
    void buildActions();

    QSystemTrayIcon* m_icon;            ///< The tray icon.
    QAction* m_actionDesktopControlToggle;
    QAction* m_actionDesktopControlOptions;
    QAction* m_actionDictationToggle;
    QAction* m_actionDictationOptions;
    QAction* m_actionPluginOptions;
    QAction* m_actionAboutSpeechControl;
    QAction* m_actionAboutQt;
    QAction* m_actionHelpManual;
public slots:
    void on_actionAboutQt_triggered ();
};
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
