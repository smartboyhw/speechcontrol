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
 * @date 05/20/12 14:02:08 PM
 */

#ifndef MS_MICSELECT_HPP
#define MS_MICSELECT_HPP

#include <QObject>
#include <QWizardPage>
#include <ui/wizards/base.hpp>

SPCHCNTRL_DEFINE_UIC (MicrophoneSelection)

namespace SpeechControl
{
class DeviceAudioSource;
namespace Windows
{
namespace Pages
{

class MicrophoneSelection : public QWizardPage
{
    Q_OBJECT

public:
    explicit MicrophoneSelection (QWidget* p_parent = 0);
    ~MicrophoneSelection();
    DeviceAudioSource* device();
    void setDevice (DeviceAudioSource* p_device);
    virtual void initializePage();
    virtual void cleanupPage();
    virtual bool validatePage();
    virtual bool isComplete();

private slots:
    void on_mic_bufferObtained (QByteArray p_buffer);
    void on_comboBoxMicrophones_activated (int p_index);

private:
    Ui::MicrophoneSelection* ui;
    DeviceAudioSource* m_mic;
};

}
}
}
#endif // MICSELECT_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
