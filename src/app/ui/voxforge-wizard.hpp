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


#ifndef SPCHCNTRL_VOXFORGE_WIZARD_HPP
#define SPCHCNTRL_VOXFORGE_WIZARD_HPP

#include <ui/wizards/base.hpp>

namespace SpeechControl
{
namespace Windows
{
namespace Wizards
{

class VoxforgeWizard : public AbstractWizard
{
    Q_OBJECT
    Q_DISABLE_COPY (VoxforgeWizard)

public:
    enum {
        IntroductionPage = 0,
        OptInConfirmationPage,
        PackageSelectionPage,
        LoginPortalPage,
        UploadEnactionPage,
        ConclusionPage
    };

    explicit VoxforgeWizard (QWidget* parent = 0);
    virtual ~VoxforgeWizard();
};

}
}
}
#endif // SPCHCNTRL_VOXFORGE_WIZARD_HPP
// kate: indent-mode cstyle; replace-tabs on; 
