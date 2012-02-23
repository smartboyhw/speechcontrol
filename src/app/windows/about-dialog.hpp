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

#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#ifndef HAVE_KDE
#include <QDialog>
#define SC_DIALOG QDialog
#else
#include <KDialog>
#define SC_DIALOG KDialog
#endif

#include "ui_about-dialog.h"

namespace SpeechControl {
namespace Windows {
class AboutDialog : public SC_DIALOG {
    Q_OBJECT

public:
    AboutDialog ( QWidget *parent = 0 );

private:
    Ui::AboutDialog ui;

    QString about() const;
    QString authors() const;
    QString contributors() const;
    QString thanksTo() const;

};
}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
