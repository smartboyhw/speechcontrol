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

#ifndef FILESELECT_HPP
#define FILESELECT_HPP

#include <QWizardPage>
#include <sessions/content.hpp>

namespace Ui {
class CustomSourcePage;
}

namespace SpeechControl {
namespace Wizards {
namespace Pages {

class CustomSourcePage : public QWizardPage {
    Q_OBJECT

public:
    explicit CustomSourcePage ( QWidget *parent = 0 );
    ~CustomSourcePage();

private:
    Ui::CustomSourcePage *m_ui;
    AbstractContentSource* m_src;
};

}
}
}

#endif // FILESELECT_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
