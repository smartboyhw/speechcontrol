/***
 *  This file is part of SpeechControl.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef SPCHCNTRL_VOXFORGE_LOGIN_HPP
#define SPCHCNTRL_VOXFORGE_LOGIN_HPP
#include <QWizardPage>

namespace Ui
{
class LoginPortal;
}

namespace SpeechControl
{
namespace Windows
{

namespace Pages
{
class LoginPortal : public QWizardPage
{
    Q_OBJECT

public:
    LoginPortal (QWidget* parent = 0);
    virtual ~LoginPortal();
    virtual bool isComplete() const;

private slots:
    void on_btnLogin_clicked();
    void on_checkBoxRemeberCreds_toggled (const bool& p_checked);
    void on_lineEditUsername_textChanged (const QString& p_text);
    void on_lineEditPassword_textChanged (const QString& p_text);

private:
    Ui::LoginPortal* ui;
    bool m_successLogin;
};
}
}
}

#endif //SPCHCNTRL_VOXFORGE_LOGIN_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
