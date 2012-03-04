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

#include <QFrame>

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
class DictationSettingsPane : public QFrame {
    Q_OBJECT
    Q_PROPERTY ( const QString Title READ title )   ///< The title of the pane.
    Q_PROPERTY ( const QString ID READ id )         ///< The ID of the pane.

public:
    /**
     * @brief Constructor.
     * @param parent Defaults to 0.
     **/
    explicit DictationSettingsPane ( QWidget *p_parent = 0 );

    /**
     * @brief Destructor.
     **/
    ~DictationSettingsPane();

    /**
     * @brief Obtains the title of this configuration pane.
     **/
    const QString title() const;

    /**
     * @brief Obtains the ID of this configuration pane.
     **/

    const QString id() const;

protected:
    void changeEvent ( QEvent *e );

private slots:
    void on_checkBoxEnable_toggled ( bool p_checked );
    void updateContent();

private:
    Ui::DictationSettingsPane *m_ui;
};

}
}

#endif // GENERAL_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
