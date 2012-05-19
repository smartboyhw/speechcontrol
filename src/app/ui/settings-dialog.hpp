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


#ifndef SPCHCNTRL_SETTINGS_DIALOG_HPP
#define SPCHCNTRL_SETTINGS_DIALOG_HPP

#include <QMap>
#include <QDialog>
#include <QPixmap>
#include <QTreeWidgetItem>
#include <QAbstractButton>
#include "global.hpp"

namespace Ui
{
class SettingsDialog;
}

SPCHCNTRL_BEGIN_NAMESPACE

class SettingsDialog;
class AbstractSettingsPane;

/**
 * @brief Settings dialog
 **/
class SettingsDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY (SettingsDialog)
    SC_SINGLETON (SettingsDialog)

public:
    explicit SettingsDialog (QWidget* m_prnt);
    static void addPane (AbstractSettingsPane* p_pane);
    static void removePane (const QString& p_paneID);
    static void displayPane (const QString& p_paneID = "gnrl");
    virtual ~SettingsDialog();

private slots:
    void on_treeNavigation_itemSelectionChanged();
    void on_buttonBox_clicked (QAbstractButton* p_button);

private:
    void buildWindow();
    QTreeWidgetItem* findPaneForItem (const QString& p_panelID);
    AbstractSettingsPane* currentPane();
    AbstractSettingsPane* findPane (QString id);
    Ui::SettingsDialog* m_ui;
    QMap<QString, AbstractSettingsPane*> m_panes;
};

class AbstractSettingsPane : public QFrame
{
    Q_OBJECT
    Q_PROPERTY (QString ID READ id)
    Q_PROPERTY (QString Title READ title)
    Q_PROPERTY (QPixmap Pixmap READ pixmap)
    friend class SettingsDialog;

public:
    explicit AbstractSettingsPane (QWidget* parent = 0);
    virtual ~AbstractSettingsPane();
    bool hasPane (const QString& p_paneID) const;
    virtual void restoreDefaults() = 0;
    virtual QString title() const = 0;
    virtual QString id() const = 0;
    virtual QPixmap pixmap() const = 0;

protected:
    void addPane (AbstractSettingsPane* p_subPane);
    void removePane (AbstractSettingsPane* p_subPane);
    void removePane (const QString& p_subPaneID);
    virtual void updateUi() = 0;

private:
    QMap<QString, AbstractSettingsPane*> m_panes;
};

SPCHCNTRL_END_NAMESPACE

#endif // SETTINGS_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
