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
 * @date 05/22/12 23:21:19 PM
 */

#ifndef SESSIONSETTINGSPANE_HPP
#define SESSIONSETTINGSPANE_HPP

#include <ui/settings-dialog.hpp>

SPCHCNTRL_DEFINE_UIC(SessionSettingsPane)
SPCHCNTRL_UI_BEGIN_NAMESPACE

/**
 * @brief Represents the settings dialog's pane for Sessions.
 **/
class SessionSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit SessionSettingsPane ();
    ~SessionSettingsPane();
    virtual QString id() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();
    virtual QString title() const;

protected:
    void changeEvent (QEvent* e);
    virtual void updateUi();

private slots:
    void on_btnInfo_clicked();
    void on_actionDelete_triggered();
    void on_actionCopy_triggered();

private:
    Ui::SessionSettingsPane* m_ui;
};

SPCHCNTRL_UI_END_NAMESPACE

#endif // SESSIONSETTINGSPANE_HPP
// kate: indent-mode cstyle; replace-tabs on;
