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
 * @date 05/20/12 13:06:28 PM
 */

#ifndef ADAPTION_WIZARD_HPP
#define ADAPTION_WIZARD_HPP

#include <ui/wizards/base.hpp>

namespace SpeechControl
{
class Session;
namespace Windows
{
namespace Pages
{
class Enaction;
class Results;
}

class AdaptWizard : public AbstractWizard
{
    Q_OBJECT
    Q_DISABLE_COPY (AdaptWizard)
    enum {
        IntroductionPage = 0,
        ModelSelectionPage,
        SessionSelectionPage,
        EnactionPage,
        ResultsPage,
        ConclusionPage
    };

    friend class Pages::Enaction;
    friend class Pages::Results;

public:
    explicit AdaptWizard (QWidget* parent = 0);
    virtual ~AdaptWizard();
    virtual int nextId() const;
    void setSession (Session* session);

public slots:
    virtual void accept();
};

}
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

