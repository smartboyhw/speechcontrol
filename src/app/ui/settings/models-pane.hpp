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

#ifndef MODEL_SETTINGS_HPP
#define MODEL_SETTINGS_HPP

#include <app/ui/settings-dialog.hpp>

namespace Ui
{
class ModelSettingsPane;
}

namespace SpeechControl
{
namespace Windows
{

class ModelSettingsPane : public AbstractSettingsPane
{
    Q_OBJECT

public:
    explicit ModelSettingsPane();
    ~ModelSettingsPane();

    virtual QString title() const;
    virtual QString id() const;
    virtual QPixmap pixmap() const;
    virtual void restoreDefaults();
    void updateLanguageTab();
    void updateAcousticTab();

protected:
    void updateUi();
    void changeEvent (QEvent* e);

private slots:
    void on_comboBoxAcousticModelView_currentIndexChanged(const int p_index);
    void on_comboBoxLanguageModelView_currentIndexChanged(const int p_index);
    void on_btnAdaptDelete_clicked();
    void on_btnAdaptAcoustic_clicked();
    void on_btnLanguageDelete_clicked();

private:
    Ui::ModelSettingsPane* m_ui;
};

}
}

#endif // GENERAL_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
