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


#ifndef CONTENTS_WIZARD_HPP
#define CONTENTS_WIZARD_HPP

#include <ui/wizards/base.hpp>

namespace SpeechControl
{
class Content;
class AbstractContentSource;
namespace Windows { namespace Wizards
{

class ContentWizard : public AbstractWizard
{
    Q_OBJECT
public:
    enum Pages {
        IntroductionPage = 0,
        SourceSelectionPage,
        ConclusionPage
    };

    explicit ContentWizard (QWidget* parent = 0);
    virtual ~ContentWizard();
    virtual void accept();
    AbstractContentSource* source();
    void setSource (AbstractContentSource* p_src);

public slots:
    virtual int nextId() const;

private:
    mutable AbstractContentSource* m_src;
};

}
}
}
#endif // WIZARD_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
