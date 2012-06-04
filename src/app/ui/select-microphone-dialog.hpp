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


#ifndef SELECT_MIC_DIALOG_HPP
#define SELECT_MIC_DIALOG_HPP

#include <ui/wizards/base.hpp>

namespace Ui
{
class MicrophoneSelectionDialog;
}

namespace SpeechControl
{
class DeviceAudioSource;
namespace Windows
{

class MicrophoneSelectionDialog : public QDialog
{
    Q_OBJECT
    void updateUi();
public:
    explicit MicrophoneSelectionDialog (QWidget* parent = 0);
    ~MicrophoneSelectionDialog();

public slots:
    virtual void accept();

private slots:
    void on_comboBoxDevices_currentIndexChanged (const int index);
    void on_checkBoxUseDefault_toggled (const bool checked);

private:
    Ui::MicrophoneSelectionDialog* ui;
    DeviceAudioSource* m_mic;

};

}
}
#endif // WIZARD_HPP
// kate: indent-mode cstyle; replace-tabs on; 
