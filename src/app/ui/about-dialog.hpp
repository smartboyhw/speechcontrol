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
 * @date 05/19/12 16:28:49 PM
 */

#ifndef SPCHCNTRL_ABOUTDIALOG_HPP
#define SPCHCNTRL_ABOUTDIALOG_HPP

#include <QDialog>
#include <ui/global.hpp>

SPCHCNTRL_DEFINE_UIC(AboutDialog)

SPCHCNTRL_UI_BEGIN_NAMESPACE

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog (QWidget* parent = 0);

private:
    Ui::AboutDialog* ui;

};

SPCHCNTRL_UI_END_NAMESPACE

#endif
// kate: indent-mode cstyle; replace-tabs on; 
