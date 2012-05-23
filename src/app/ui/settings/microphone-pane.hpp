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
 * @date 05/22/12 23:24:29 PM
 */

#ifndef MICROPHONE_SETTINGS_HPP
#define MICROPHONE_SETTINGS_HPP

#include <QFrame>
#include <ui/settings-dialog.hpp>

SPCHCNTRL_DEFINE_UIC(MicrophoneSettingsPane)
SPCHCNTRL_UI_BEGIN_NAMESPACE

class MicrophoneSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit MicrophoneSettingsPane ();
    virtual ~MicrophoneSettingsPane();
    virtual QString id() const;
    virtual QString title() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();

protected:
    void changeEvent (QEvent* e);

private slots:
    virtual void updateUi();

private:
    Ui::MicrophoneSettingsPane* ui;
public slots:
    void on_checkBoxMute_toggled (const bool p_checked);
    void on_comboBoxDevices_currentIndexChanged (const int p_index);
    void on_horizontialSliderVolume_valueChanged (const int p_value);
};

SPCHCNTRL_UI_END_NAMESPACE

#endif // MICROPHONE_HPP
// kate: indent-mode cstyle; replace-tabs on;
