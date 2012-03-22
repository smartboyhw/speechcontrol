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

#include <app/sessions/session.hpp>

#include <QDir>
#include <QDebug>
#include "sessionselection.hpp"
#include "ui_adaptwizard-sessionselection.h"

using namespace SpeechControl;
using SpeechControl::Wizards::Pages::SessionSelection;

SessionSelection::SessionSelection (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::SessionSelection), m_session (0)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    initializePage();
}

Session* SessionSelection::session()
{
    return m_session;
}

void SessionSelection::initializePage()
{
    ui->comboBoxSession->clear();

    SessionList sessions = Session::allSessions();

    Q_FOREACH (const Session * session, sessions) {
        if (session->isCompleted()) {
            const QString name = session->name();
            ui->comboBoxSession->addItem (name, session->uuid().toString());
        }
    }
}

void SessionSelection::cleanupPage()
{

}

bool SessionSelection::isComplete() const
{
    return true;
}

void SessionSelection::on_comboBoxSession_currentIndexChanged (const int index)
{
    QString path = ui->comboBoxSession->itemData (index).toString();
    m_session = Session::obtain (path);

    if (m_session->content())
        ui->lineEditUniqueWordCount->setText (QString::number (m_session->content()->uniqueWords()));
    else
        ui->lineEditUniqueWordCount->setText ("unknown; missing content");
}

SessionSelection::~SessionSelection()
{
    delete ui;
}

#include "sessionselection.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
