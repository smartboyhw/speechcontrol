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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 23:57:39 PM
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/16/12 23:44:43 PM
 */

#ifndef PLUGINS_SETTINGS_HPP
#define PLUGINS_SETTINGS_HPP

#include <QFrame>
#include <ui/settings-dialog.hpp>

class QListWidgetItem;

namespace Ui
{
class PluginsSettingsPane;
}

SPCHCNTRL_BEGIN_NAMESPACE

class PluginsSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit PluginsSettingsPane ();
    virtual ~PluginsSettingsPane();
    virtual QString id() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();
    virtual QString title() const;
protected:
    void changeEvent (QEvent* e);
    virtual void updateUi();

private slots:
    void on_btnInfo_clicked();
    void on_checkBoxEnabled_toggled (const bool p_checked);
    void on_checkBoxAutoStart_toggled (const bool p_checked);
    void on_lstPlugins_itemActivated (QListWidgetItem* p_item);
    void on_btnLoadPlugin_clicked();
    void on_lstPlugins_itemSelectedChanged();

private:
    Ui::PluginsSettingsPane* ui;
};

SPCHCNTRL_END_NAMESPACE

#endif // PLUGINS_HPP
// kate: indent-mode cstyle; replace-tabs on; 
