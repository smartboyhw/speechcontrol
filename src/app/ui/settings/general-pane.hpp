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
 * @date 05/22/12 22:48:15 PM
 */

#ifndef GENERAL_SETTINGS_HPP
#define GENERAL_SETTINGS_HPP

#include <QFrame>
#include <ui/settings-dialog.hpp>

SPCHCNTRL_DEFINE_UIC(GeneralSettingsPane)
SPCHCNTRL_UI_BEGIN_NAMESPACE
/**
 * @brief General settings of SpeechControl.
 *
 * This window presents the user in the Settings dialog with general, overlying
 * options that can manipulate SpeechControl's operation.
 **/
class GeneralSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Defaults to 0.
     **/
    explicit GeneralSettingsPane ();

    /**
     * @brief Destructor.
     **/
    ~GeneralSettingsPane();
    virtual QString id() const;
    virtual QString title() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();

protected:
    void changeEvent (QEvent* e);

private slots:
    void on_checkBoxAutoStartApp_toggled (bool p_checked);
    void on_radioButtonIconWhite_toggled (bool p_checked);
    void on_radioButtonIconBlack_toggled (bool p_checked);
    void on_radioButtonIconDefault_toggled (bool p_checked);
    virtual void updateUi();

private:
    Ui::GeneralSettingsPane* ui;
};

SPCHCNTRL_UI_END_NAMESPACE
#endif // GENERAL_HPP
// kate: indent-mode cstyle; replace-tabs on;
