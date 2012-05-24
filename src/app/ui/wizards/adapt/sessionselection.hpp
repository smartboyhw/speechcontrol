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
 * @date 05/20/12 13:30:46 PM
 */

#ifndef ADAPT_SESSIONSELECTION_HPP
#define ADAPT_SESSIONSELECTION_HPP

#include <QWizardPage>
#include <ui/wizards/base.hpp>
#include <sessions/session.hpp>

SPCHCNTRL_DEFINE_UIC(SessionSelection)
SPCHCNTRL_UI_WIZARD_PAGES_BEGIN_NAMESPACE

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

SPCHCNTRL_UI_WIZARD_PAGES_END_NAMESPACE
#endif // ADAPT_SESSIONSELECTION_HPP
// kate: indent-mode cstyle; replace-tabs on;
