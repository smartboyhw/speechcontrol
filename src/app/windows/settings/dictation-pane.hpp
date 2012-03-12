/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#ifndef DICTATION_SETTINGS_HPP
#define DICTATION_SETTINGS_HPP

#include <app/windows/settings-dialog.hpp>

namespace Ui {
class DictationSettingsPane;
}

namespace SpeechControl {
namespace Windows {

/**
 * @brief General settings of SpeechControl.
 *
 * This window presents the user in the Settings dialog with general, overlying
 * options that can manipulate SpeechControl's operation.
 **/
class DictationSettingsPane : public AbstractSettingsPane {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Defaults to 0.
     **/
    explicit DictationSettingsPane (  );
    ~DictationSettingsPane();
    QString title() const;
    QString id() const;
    void updateUi();
    virtual bool containsText ( const QString& p_query ) const;
    virtual QPixmap pixmap() const;
    virtual void resetPanel();
    virtual void restoreDefaults();

protected:
    void changeEvent ( QEvent *e );

private slots:
    void on_checkBoxEnable_toggled ( bool p_checked );
    void on_checkBoxEnableStartup_toggled (bool p_checked);

private:
    Ui::DictationSettingsPane *m_ui;
};

}
}

#endif // GENERAL_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
