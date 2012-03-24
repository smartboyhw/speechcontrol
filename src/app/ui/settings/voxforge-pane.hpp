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
#ifndef VOXFORGESETTINGSPANE_HPP
#define VOXFORGESETTINGSPANE_HPP

#include <QFrame>
#include <uisettings-dialog.hpp>

namespace Ui
{
class VoxforgeSettingsPane;
}

namespace SpeechControl
{
namespace Windows
{

class VoxforgeSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit VoxforgeSettingsPane ();
    ~VoxforgeSettingsPane();
    virtual QString id() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();
    virtual QString title() const;

protected:
    virtual void updateUi();
    void changeEvent (QEvent* e);

private slots:
    void on_checkBox_clicked();

private:
    Ui::VoxforgeSettingsPane* ui;
};

}
}

#endif // VOXFORGESETTINGSPANE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
