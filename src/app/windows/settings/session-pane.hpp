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

#ifndef SESSIONSETTINGSPANE_HPP
#define SESSIONSETTINGSPANE_HPP

#include <app/windows/settings-dialog.hpp>

namespace Ui {
class SessionSettingsPane;
}

namespace SpeechControl {
namespace Windows {

class SessionSettingsPane : public AbstractSettingsPane {
    Q_OBJECT

public:
    explicit SessionSettingsPane (  );
    ~SessionSettingsPane();
    virtual QString title() const;
    virtual QString id() const;
    virtual void updateUi();
    virtual bool containsText ( const QString& p_query ) const;
    virtual QPixmap pixmap() const;
    virtual void resetPanel();
    virtual void restoreDefaults();
    virtual void show();

protected:
    void changeEvent ( QEvent *e );

private slots:
    void on_btnInfo_clicked();
    void on_actionDelete_triggered();
    void on_actionCopy_triggered();
    void on_actionBackup_triggered();
    void on_actionRestoreBackup_triggered();

private:
    Ui::SessionSettingsPane *m_ui;
};

}
}

#endif // SESSIONSETTINGSPANE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
