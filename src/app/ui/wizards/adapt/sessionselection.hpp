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
#ifndef ADAPT_SESSIONSELECTION_HPP
#define ADAPT_SESSIONSELECTION_HPP

#include <QWizardPage>
#include <sessions/session.hpp>

namespace Ui
{
class SessionSelection;
}

namespace SpeechControl
{

class Session;
namespace Windows
{
namespace Wizards
{
namespace Pages
{

class SessionSelection : public QWizardPage
{
    Q_OBJECT

public:
    explicit SessionSelection (QWidget* parent = 0);
    virtual ~SessionSelection();
    virtual void initializePage();
    virtual void cleanupPage();
    virtual bool validatePage();
    SessionList sessions() const;
    void setSession (Session* p_session);
    void setSessions (SessionList p_sessionList);

private slots:
    void on_listWidgetSession_itemSelectionChanged();

private:
    Ui::SessionSelection* ui;
    SessionList m_listSession;
};

}
}
}
}
#endif // ADAPT_SESSIONSELECTION_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
