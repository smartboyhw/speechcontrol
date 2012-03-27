/***
 *  This file is part of SpeechControl.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  SpeechControl is free software; you can redistribute it and/or
 *  modify it -under the terms of the GNU Library General Public
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

#include <QImage>
#include <QDateTime>
#include <QDebug>

// Local
#include "core.hpp"
#include "indicator.hpp"
#include "ui/main-window.hpp"
#include <ui_main-window.h>

// Qt
#include <QImage>
#include <QMenu>
#include <QDebug>

using SpeechControl::Indicator;
using SpeechControl::Core;

Indicator* Indicator::s_inst = 0;

Indicator::Message::Message (const QString& p_keyName) : m_key (p_keyName)
{

}

QString Indicator::Message::description() const
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        return map.value ("description").toString();
    }

    return QString::null;
}

bool Indicator::Message::enabled() const
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        return map.value ("enabled").toBool();
    }

    return true;
}

QString Indicator::Message::key() const
{
    return m_key;
}

void Indicator::Message::setEnabled (const bool& p_isEnabled)
{
    if (Indicator::Message::exists (m_key)) {
        QVariantMap map = Indicator::Message::objectData (m_key);
        map.insert ("enabled", p_isEnabled);
        Core::setConfiguration ("Notifications/" + m_key, map);
    }
}

bool Indicator::Message::exists (const QString& p_keyName)
{
    QVariant msgObj = Core::configuration ("Notifications/" + p_keyName);
    return (msgObj.isValid());
}

Indicator::Message* Indicator::Message::create (const QString& p_keyName, const QString& p_keyDescription, const bool& p_isEnabled)
{
    QVariantMap map;
    map.insert ("description", p_keyDescription);
    map.insert ("enabled", p_isEnabled);

    Core::setConfiguration ("Notifications/" + p_keyName, map);
    return new Message (p_keyName);
}

QVariantMap Indicator::Message::objectData (const QString& p_keyName)
{
    return Core::configuration ("Notifications/" + p_keyName).toMap();
}

/// @todo Check for a configuration value to determine whether or not the indicator should be shown on initialization.
Indicator::Indicator () : QObject (Core::instance()),
    m_icon (0)
{
    s_inst = this;
    m_icon = new QSystemTrayIcon (icon(), this);
    connect (m_icon, SIGNAL (activated (QSystemTrayIcon::ActivationReason)), this, SLOT (on_mIcon_activated (QSystemTrayIcon::ActivationReason)));
    buildMenu();
}

void Indicator::on_mIcon_activated (QSystemTrayIcon::ActivationReason p_reason)
{
    switch (p_reason) {
    case QSystemTrayIcon::Trigger:
        Core::mainWindow()->isVisible() ? Core::mainWindow()->hide() : Core::mainWindow()->show();
        break;

    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
    case QSystemTrayIcon::Context:
    case QSystemTrayIcon::Unknown:
    default:
        break;
    }
}

void Indicator::buildMenu()
{
    QMenu* menu = new QMenu;
    QMenu* menuDesktopControl = menu->addMenu (Core::mainWindow()->m_ui->menuDesktopControl->icon(),
                                Core::mainWindow()->m_ui->menuDesktopControl->title());
    QMenu* menuDictation      = menu->addMenu (Core::mainWindow()->m_ui->menuDictation->icon(),
                                Core::mainWindow()->m_ui->menuDictation->title());
    QMenu* menuPlugins        = menu->addMenu (Core::mainWindow()->m_ui->menuPlugins->icon(),
                                Core::mainWindow()->m_ui->menuPlugins->title());
    QMenu* menuHelp           = menu->addMenu (Core::mainWindow()->m_ui->menuHelp->icon(),
                                Core::mainWindow()->m_ui->menuHelp->title());

    menuDesktopControl->addActions (Core::mainWindow()->m_ui->menuDesktopControl->actions());
    menuDictation->addActions (Core::mainWindow()->m_ui->menuDictation->actions());
    menuPlugins->addActions (Core::mainWindow()->m_ui->menuPlugins->actions());
    menuHelp->addActions (Core::mainWindow()->m_ui->menuHelp->actions());

    menu->addMenu (menuDesktopControl);
    menu->addMenu (menuDictation);
    menu->addMenu (menuPlugins);
    menu->addSeparator();
    menu->addMenu (menuHelp);
    menu->addAction (Core::mainWindow()->m_ui->actionOptions);
    menu->addSeparator();
    menu->addAction ("Restore", Core::mainWindow(), SLOT (open()));
    menu->addAction (QIcon::fromTheme ("application-exit"), "Quit", QApplication::instance(), SLOT (quit()));

    m_icon->setContextMenu (menu);
}


QIcon Indicator::icon()
{
    const QString state = Core::configuration ("Indicator/Icon").toString();
    qDebug() << "[Indicator::icon] " << state;

    if (state == "White")
        return QIcon (":/indicator/white");
    else if (state == "Black")
        return QIcon (":/indicator/black");
    else if (state == "Default")
        return QIcon (":/logo/sc");

    return QApplication::windowIcon();
}

void Indicator::hide()
{
    instance()->m_icon->hide();
}

void Indicator::show()
{
    instance()->m_icon->setIcon (icon().pixmap (48, 48));
    instance()->m_icon->show();
}

void Indicator::showMainWindow()
{
    Core::mainWindow()->open();
}

/// @todo Add an enumeration that allows the callee to specify the kind of message icon they'd  want to appear.
void Indicator::presentMessage (const QString& p_title, const QString& p_message, const int& p_timeout, const Indicator::Message& p_messageIndicator)
{
    if (!Indicator::Message::exists (p_messageIndicator.key()))
        Indicator::Message::create (p_messageIndicator.key(), p_message, true);

    if (p_messageIndicator.enabled())
        instance()->m_icon->showMessage (p_title, p_message, QSystemTrayIcon::Information, p_timeout);
}

bool Indicator::isVisible()
{
    return instance()->m_icon->isVisible();
}

bool Indicator::isEnabled()
{
    return Core::configuration ("Indicator/Show").toBool();
}

Indicator::~Indicator()
{

}

#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
