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

#include "enaction.hpp"
#include "ui_adaptwizard-enaction.h"

using namespace SpeechControl;
using SpeechControl::Wizards::Pages::Enaction;

Enaction::Enaction (QWidget* parent) :
    QWizardPage (parent),
    ui (new Ui::Enaction)
{
    ui->setupUi (this);
    this->setLayout (ui->gridLayout);
    setCommitPage(true);
    initializePage();
}

void Enaction::initalizePage()
{

}

void Enaction::cleanupPage()
{

}

bool Enaction::isComplete() const
{
    return true;
}

Enaction::~Enaction()
{
    delete ui;
}

#include "enaction.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
