/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "books.hpp"
#include "sessions/session.hpp"
#include "wizards/contents/wizard.hpp"
#include "ui_booksettingspane.h"

#include <QListWidget>
#include <QMessageBox>
#include <QErrorMessage>

using namespace SpeechControl;
using namespace SpeechControl::Windows;

BookSettingsPane::BookSettingsPane(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BookSettingsPane)
{
    ui->setupUi(this);
    updateList();
}

BookSettingsPane::~BookSettingsPane()
{
    delete ui;
}

void BookSettingsPane::show(){
    updateList();
    QFrame::show();
}

void BookSettingsPane::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

const QString SpeechControl::Windows::BookSettingsPane::title() const
{
    return "Books";
}

const QString SpeechControl::Windows::BookSettingsPane::id() const
{
    return "bks";
}

void SpeechControl::Windows::BookSettingsPane::updateList()
{
    QListWidget* l_widget = ui->lstBooks;
    ContentList l_lst = Content::allContents();

    l_widget->clear();

    if (!l_lst.empty()){
        Q_FOREACH(const Content* l_cnt, l_lst){
            const QString l_lbl = l_cnt->title();
            QListWidgetItem* l_item = new QListWidgetItem(l_widget);
            l_item->setData(Qt::UserRole,l_cnt->uuid().toString());
            l_widget->addItem(l_item);

            if (l_lbl.isEmpty())
                l_item->setText("Unnamed");
            else
                l_item->setText(l_lbl);
        }
    }
}

void SpeechControl::Windows::BookSettingsPane::on_btnDelete_clicked()
{
    QListWidget* l_widg = ui->lstBooks;
    QListWidgetItem* l_itm = l_widg->selectedItems().first();
    Content* l_cntn = Content::obtain(l_itm->data(Qt::UserRole).toString());

    if (QMessageBox::Yes == QMessageBox::question(this,"Confirm Book Delete", "Are you sure you want to delete this book?\nAny session connected to the book will become invalid and untrainable.",
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::No)){
        l_cntn->erase();
        updateList();
    }
}

/// @todo Add the functionality to add a book.
void SpeechControl::Windows::BookSettingsPane::on_btnAdd_clicked()
{
    Wizards::ContentWizard l_wiz(this);
    if (l_wiz.exec() == QDialog::Accepted)
        updateList();
}

/// @todo Present a dialog showing information about the content/book.
void SpeechControl::Windows::BookSettingsPane::on_btnInfo_clicked()
{

}
