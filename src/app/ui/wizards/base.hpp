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

#ifndef SPCHCNTRL_UI_WIZARDS_BASE_HPP_
#define SPCHCNTRL_UI_WIZARDS_BASE_HPP_

#include <QWizard>

namespace Ui
{
class WizardBase;
}

namespace SpeechControl
{
namespace Windows
{
namespace Wizards
{

/// @todo Allow automatic insertion of introduction page.
class AbstractWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AbstractWizard (QWidget* parent = 0);
    virtual ~AbstractWizard();
    virtual void setPage (const int p_pageIndex, QWizardPage* p_page);

private:
    Ui::WizardBase* ui;
};

}
}
}
#endif // SPCHCNTRL_UI_WIZARDS_BASE_HPP_
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
