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
 * @date 05/16/12 23:23:53 PM
 */

#include "plugin-info-dialog.hpp"
#include "ui_plugin-info-dialog.h"
#include <factory.hpp>

SPCHCNTRL_USE_NAMESPACE

PluginInfoDialog::PluginInfoDialog (QUuid p_uuid, QWidget* parent) : QDialog (parent)
{
    m_plgn = new GenericPlugin (p_uuid);
    updateUi();
}

PluginInfoDialog::PluginInfoDialog (SpeechControl::AbstractPlugin* p_plugin, QWidget* parent) : QDialog (parent)
{
    m_plgn = (GenericPlugin*) p_plugin;
    updateUi();
}

void PluginInfoDialog::updateUi()
{
    ui = new Ui::PluginInfoDialog;
    ui->setupUi (this);
    ui->lblTitle->setText (QString ("<em style='font-size: large'>%1</em></font><br /><small>version %2</small>").arg (m_plgn->name()).arg (m_plgn->version()));
    ui->lblAbout->setText (m_plgn->description());
    ui->lblAuthors->setText (m_plgn->author());

    if (!m_plgn->isLoaded())
        ui->lblIcon->setPixmap (m_plgn->pixmap());
    else
        ui->lblIcon->setPixmap (Factory::plugin (m_plgn->id())->pixmap());
}

#include "ui/plugin-info-dialog.moc"
// kate: indent-mode cstyle; replace-tabs on; 
