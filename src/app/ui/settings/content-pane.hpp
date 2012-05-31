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
 * @date 05/20/12 14:32:22 PM
 */

#ifndef CONTENTSETTINGSPANE_HPP
#define CONTENTSETTINGSPANE_HPP

#include <ui/settings-dialog.hpp>

SPCHCNTRL_DEFINE_UIC(ContentSettingsPane)
namespace SpeechControl { namespace Windows {
class ContentSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit ContentSettingsPane ();
    ~ContentSettingsPane();
    void updateUi();
    virtual void show();
    virtual void restoreDefaults() { }
    virtual QString title() const;
    virtual QString id() const;
    virtual QPixmap pixmap() const;

protected:
    void changeEvent (QEvent* e);

private slots:
    void on_btnDelete_clicked();
    void on_btnAdd_clicked();
    void on_btnInfo_clicked();

private:
    Ui::ContentSettingsPane* ui;
};

} }

#endif // CONTENTSETTINGSPANE_HPP
// kate: indent-mode cstyle; replace-tabs on;
