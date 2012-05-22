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


#ifndef CONTENTINFORMATIONDIALOG_HPP
#define CONTENTINFORMATIONDIALOG_HPP

#include <QDialog>

namespace Ui
{
class ContentInformationDialog;
}

namespace SpeechControl
{
class Content;
namespace Windows
{

class ContentInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContentInformationDialog (Content* p_content);
    ~ContentInformationDialog();

private slots:
    void updateUi();
    void goToNextPage();
    void goToPreviousPage();

private:
    void goToPage (const int p_index);
    Ui::ContentInformationDialog* m_ui;
    Content* m_content;
    uint m_indx;
};

}
}

#endif // CONTENTINFORMATIONDIALOG_HPP
// kate: indent-mode cstyle; replace-tabs on; 
