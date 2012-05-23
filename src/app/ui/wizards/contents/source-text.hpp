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
 * @date 05/20/12 14:26:38 PM
 */

#ifndef CONTENTWIZARD_SOURCE_TEXT_HPP
#define CONTENTWIZARD_SOURCE_TEXT_HPP

#include <QWidget>
#include <ui/global.hpp>

SPCHCNTRL_DEFINE_UIC(TextContentSourceWidget)

namespace SpeechControl { namespace Windows {
class TextContentSourceWidget : public QWidget
{
    Q_OBJECT
    void updateView();

public:
    explicit TextContentSourceWidget (QWidget* parent = 0);
    ~TextContentSourceWidget();

private slots:
    void on_btnOpen_clicked();

private:
    Ui::TextContentSourceWidget* m_ui;
};

} }

#endif // CONTENTWIZARD_SOURCE_TEXT_HPP
// kate: indent-mode cstyle; replace-tabs on;
