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


#ifndef SESSIONINFORMATIONDIALOG_HPP
#define SESSIONINFORMATIONDIALOG_HPP

#include <QDialog>

namespace Ui
{
class SessionInformationDialog;
}

namespace SpeechControl
{
class Session;
namespace Windows
{

class SessionInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionInformationDialog (QWidget* parent = 0);
    SessionInformationDialog (Session* p_session);
    ~SessionInformationDialog();

private slots:
    void updateUi();
    void on_btnTrainSession_clicked();
    void on_btnOpenContent_clicked();
    void on_btnPhrasePlay_clicked();
    void on_horizontalSliderPhrase_valueChanged(const int p_value);
    void on_lineEditNickname_textChanged (const QString& p_newNickname);
    void updateProgress (const double p_progress);

private:
    Ui::SessionInformationDialog* m_ui;
    Session* m_session;
};

}
}

#endif // SESSIONINFORMATIONDIALOG_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
