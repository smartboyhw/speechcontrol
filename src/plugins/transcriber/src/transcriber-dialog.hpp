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


#ifndef TRANSCRIBERDIALOG_HPP
#define TRANSCRIBERDIALOG_HPP

#include <QDialog>
#include "sphinx.hpp"

namespace Ui
{
class TranscriberDialog;
}

namespace SpeechControl
{
namespace Windows
{

class TranscriberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TranscriberDialog (QWidget* parent = 0);
    ~TranscriberDialog();

private slots:
    void on_btnOpen_clicked();
    void on_btnTranscribe_clicked();

private:
    Ui::TranscriberDialog* m_ui;
    SpeechControl::Plugins::Transcriber::Sphinx* m_sphnx;
public slots:
    void outputValue (const QString& p_value);
};

}
}

#endif // TRANSCRIBERDIALOG_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
