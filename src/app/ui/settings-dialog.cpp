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

#include <QDebug>
#include <QMessageBox>
#include <QTreeWidget>

#include "core.hpp"
#include "ui/main-window.hpp"
#include "settings-dialog.hpp"
#include "settings/general-pane.hpp"
#include "settings/plugins-pane.hpp"
#include "settings/voxforge-pane.hpp"
#include "settings/training-pane.hpp"
#include "settings/services-pane.hpp"
#include "ui_settings-dialog.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

Settings* Settings::s_inst = 0;

Settings::Settings() : QDialog()
{
    s_inst = this;
    buildWindow();
}

Settings::Settings (QWidget* m_prnt) :
    QDialog (m_prnt)
{
    s_inst = this;
    buildWindow();
}

void Settings::buildWindow()
{
    qDebug() << "[Settings::buildWindow()] Building settings window.";
    m_ui = new Ui::SettingsDialog;
    m_ui->setupUi (this);
    this->setLayout (m_ui->gridLayout);

    addPane (new GeneralSettingsPane);
    addPane (new TrainingSettingsPane);
    addPane (new ServicesSettingsPane);
    qDebug() << "[Settings::buildWindow()] Built settings window.";
}


Settings::Settings (const Settings& p_other) : QDialog(p_other.parentWidget())
{

}

/// @todo Add it to the list of options.
void Settings::addPane (AbstractSettingsPane* p_pane)
{
    if (instance()->m_panes.contains (p_pane->id()))
        return;

    QString parentPaneId = p_pane->property ("parent-widget").toString();
    const bool hasParentPane = (!parentPaneId.isNull() && !parentPaneId.isEmpty());
    QTreeWidgetItem* parentItem = (hasParentPane) ? instance()->findPaneForItem (parentPaneId) : 0;
    QTreeWidgetItem* itm = 0;
    QTreeWidget* treeWidget = instance()->m_ui->treeNavigation;

    // got a parent? add them to it.
    if (hasParentPane)
        itm = new QTreeWidgetItem (parentItem , (QStringList() << p_pane->title()));
    else {
        itm = new QTreeWidgetItem (treeWidget , (QStringList() << p_pane->title()));
        treeWidget->addTopLevelItem (itm);
    }

    instance()->m_panes.insert (p_pane->id(), p_pane);
    itm->setData (0, Qt::UserRole, p_pane->id());
    itm->setIcon (0, p_pane->pixmap());

    p_pane->setParent (instance()->m_ui->frmPageContainer);
    p_pane->hide();

    // add sub-panes.
    if (!p_pane->m_panes.isEmpty()) {
        Q_FOREACH (AbstractSettingsPane * pane, p_pane->m_panes) {
            addPane (pane);
        }
    }
}

void Settings::displayPane (const QString& p_paneID)
{
    AbstractSettingsPane* currentPane = instance()->m_panes.value (p_paneID);
    QTreeWidget* treeNavi = instance()->m_ui->treeNavigation;

    if (currentPane != 0) {
        currentPane->show();
        currentPane->updateUi();
        treeNavi->setCurrentItem (instance()->findPaneForItem (p_paneID));
    }
    else {
        Core::mainWindow()->setStatusMessage (tr ("Invalid settings pane ID '%1'").arg (p_paneID));
        displayPane ("gnrl");
    }

    if (!instance()->isVisible()) {
        instance()->open();
    }
}

AbstractSettingsPane* Settings::currentPane()
{
    QTreeWidget* treeNavi = instance()->m_ui->treeNavigation;
    QTreeWidgetItem* treeItem = treeNavi->currentItem();

    if (treeItem) {
        AbstractSettingsPane* l_pane = m_panes.value (treeItem->data (0, Qt::UserRole).toString());
        return l_pane;
    }
    else {
        return 0;
    }
}

QTreeWidgetItem* Settings::findPaneForItem (const QString& p_panelID)
{
    QTreeWidget* treeNavi = instance()->m_ui->treeNavigation;
    AbstractSettingsPane* pane = m_panes.value (p_panelID);
    QList<QTreeWidgetItem*> items = treeNavi->findItems (pane->title(), Qt::MatchExactly, 0);

    if (items.isEmpty())
        return 0;

    return items.first();
}

/// @todo Remove this from the list of options.
void Settings::removePane (const QString& p_panelID)
{
    instance()->m_panes.remove (p_panelID);
}

void Settings::on_treeNavigation_itemSelectionChanged()
{
    QWidget* container = instance()->m_ui->frmPageContainer;
    QTreeWidget* treeNavi = instance()->m_ui->treeNavigation;

    if (!treeNavi->selectedItems().empty()) {
        QTreeWidgetItem* itm = treeNavi->selectedItems().first();
        const QString id = itm->data (0, Qt::UserRole).toString();

        Q_FOREACH (QObject * subPane, container->children()) {
            ( (QWidget*) subPane)->hide();
        }

        findPane (id)->show();
        findPane (id)->updateUi();;
    }
}

void Settings::on_buttonBox_clicked (QAbstractButton* p_button)
{
    QDialogButtonBox::StandardButton buttonState = m_ui->buttonBox->standardButton (p_button);

    switch (buttonState) {
    case QDialogButtonBox::Ok:
        this->accept();
        break;

    case QDialogButtonBox::Help:
        /// @todo Add ability to queue for help.
        break;

    case QDialogButtonBox::RestoreDefaults:
        /// @todo Add functionality to set the values of the properties all back to default.
        currentPane()->restoreDefaults();
        break;

    default:
        break;
    }
}
Settings::~Settings()
{
    delete m_ui;
}

AbstractSettingsPane::AbstractSettingsPane (QWidget* parent) : QFrame (parent)
{
}

AbstractSettingsPane* Settings::findPane (QString id)
{
    return m_panes.value (id);
}

void AbstractSettingsPane::addPane (AbstractSettingsPane* p_subPane)
{
    m_panes.insert (p_subPane->id(), p_subPane);
    p_subPane->setProperty ("parent-widget", id());
    qDebug() << "[AbstractSettingsPane::addPane()] Made" << p_subPane->id() << "sub-pane of" << this->id();
}

bool AbstractSettingsPane::hasPane (const QString& p_paneID) const
{
    return m_panes.contains (p_paneID);
}

/// @todo Ensure that the accompanying QListWidgetItem is removed as well.
void AbstractSettingsPane::removePane (AbstractSettingsPane* p_subPane)
{
    m_panes.remove (p_subPane->id());
    p_subPane->deleteLater();
}

void AbstractSettingsPane::removePane (const QString& p_subPaneID)
{
    removePane (m_panes.value (p_subPaneID));
}

AbstractSettingsPane::~AbstractSettingsPane()
{

}

#include "ui/settings-dialog.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
