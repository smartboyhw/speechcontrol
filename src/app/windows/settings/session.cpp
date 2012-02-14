/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "sessions/session.hpp"
#include "session.hpp"
#include "ui_sessionsettingspane.h"

#include <QListWidget>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

SessionSettingsPane::SessionSettingsPane(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::SessionSettingsPane)
{
    m_ui->setupUi(this);
    updateList();
}

void SessionSettingsPane::show(){
    updateList();
    QFrame::show();
}

SessionSettingsPane::~SessionSettingsPane()
{
    delete m_ui;
}

void SessionSettingsPane::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

const QString SpeechControl::Windows::SessionSettingsPane::title() const
{
    return "Sessions";
}

const QString SpeechControl::Windows::SessionSettingsPane::id() const
{
    return "sssns";
}

void SessionSettingsPane::updateList()
{
    QListWidget* l_widget = m_ui->listWidgetSession;
    l_widget->clear();

    SessionList l_lst = Session::allSessions();
    Q_FOREACH(const Session* l_sessionItr, l_lst){
        QListWidgetItem* l_item = new QListWidgetItem(l_widget);
        l_item->setData(Qt::UserRole,l_sessionItr->uuid().toString());
        l_widget->addItem(l_item);
        const QString l_lbl = l_sessionItr->content()->title();

        if (l_lbl.isEmpty())
            l_item->setText("Unnamed");
        else
            l_item->setText(l_lbl);

    }
}

/// @todo Present a dialog showing information about the session.
void SpeechControl::Windows::SessionSettingsPane::on_btnInfo_clicked()
{

}
