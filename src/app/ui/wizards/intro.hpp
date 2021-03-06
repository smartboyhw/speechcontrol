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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 13:39:36 PM
 */

#ifndef INTRO_HPP
#define INTRO_HPP

#include <QWizardPage>
#include <ui/wizards/base.hpp>

SPCHCNTRL_DEFINE_UIC(Introduction)
namespace SpeechControl { namespace Windows { namespace Pages {

class IntroductionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit IntroductionPage (const QString&);
    ~IntroductionPage();

private:
    Ui::Introduction* ui;
};

} } }
#endif // INTRO_HPP
// kate: indent-mode cstyle; replace-tabs on;
