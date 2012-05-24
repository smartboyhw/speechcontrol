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

#ifndef PLUGININFODIALOG_HPP
#define PLUGININFODIALOG_HPP

#include <QDialog>
#include <QUuid>
#include <app/plugin.hpp>

namespace Ui
{
class PluginInfoDialog;
}

SPCHCNTRL_BEGIN_NAMESPACE

class PluginInfoDialog : public QDialog
{
    Q_OBJECT

public:
    PluginInfoDialog (QUuid p_uuid, QWidget* parent = 0);
    PluginInfoDialog (AbstractPlugin* p_plugin, QWidget* parent = 0);

private:
    Ui::PluginInfoDialog* ui;
    void updateUi();
    GenericPlugin* m_plgn;

};

SPCHCNTRL_END_NAMESPACE
#endif
// kate: indent-mode cstyle; replace-tabs on; 
