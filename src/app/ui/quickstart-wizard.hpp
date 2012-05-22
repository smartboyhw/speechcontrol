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
 * @date 05/20/12 14:09:05 PM
 */

#ifndef QS_WIZARD_HPP
#define QS_WIZARD_HPP

#include <QObject>
#include <ui/wizards/base.hpp>

SPCHCNTRL_UI_BEGIN_NAMESPACE

class QuickStart : public AbstractWizard
{
    Q_OBJECT

    enum Pages {
        IntroductionPage = 0,
        UserCreationPage,
        MicrophoneCreationPage,
        AccuracySamplingPage,
        ContentAdditionPage,
        ConclusionPage
    };

public:
    explicit QuickStart (QWidget* parent = 0);
    ~QuickStart();

public slots:
    virtual void accept();
};

SPCHCNTRL_UI_END_NAMESPACE

#endif // WIZARD_HPP
// kate: indent-mode cstyle; replace-tabs on;
