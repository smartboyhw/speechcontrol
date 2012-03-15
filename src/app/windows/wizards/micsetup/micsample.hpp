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

#ifndef MS_MICSAMPLE_HPP
#define MS_MICSAMPLE_HPP

#include <QWizardPage>
#include <abstractsphinx.hpp>

namespace Ui
{
class MicrophoneSample;
}

namespace SpeechControl
{
class DeviceAudioSource;
namespace Wizards
{
namespace Pages
{

class MicrophoneSample : public QWizardPage
{
    Q_OBJECT

public:
    explicit MicrophoneSample (QWidget* p_parent = 0);
    virtual ~MicrophoneSample();
    virtual void initializePage();
    virtual bool validatePage();
    virtual bool isComplete();

private slots:
    bool isAtFinalPrompt();
    bool hasCompletedPrompts();
    void updateUi();
    void handleReceivedPrompt (QString p_str);
    void on_btnBeginPrompt_clicked();

private:
    class Sphinx : public AbstractSphinx {
        friend class MicrophoneSample;
        virtual void applicationMessage (const QGst::MessagePtr& p_message);
    };

    Ui::MicrophoneSample* ui;
    Sphinx* m_sphnx;
    int m_index;
    QStringList m_prompts;
};

}
}
}
#endif // MICSAMPLE_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
