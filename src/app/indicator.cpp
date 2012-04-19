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

#include <QMenu>
#include <QDebug>
#include <QImage>
#include <QDateTime>
#include <QApplication>
#include <QSystemTrayIcon>

#include "core.hpp"
#include "indicator.hpp"
#include "indicator.hxx"
#include "ui/settings-dialog.hpp"
#include "ui/about-dialog.hpp"
#include "services/dictation/service.hpp"
#include "services/desktopcontrol/service.hpp"

using namespace SpeechControl;

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
    qDebug() << "[Indicator::Message::exists()] Exists 'Notifications/" << p_keyName.toStdString().c_str() << "' ?" << msgObj.isValid();
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
    d_ptr (new IndicatorPrivate)
{
    Q_D (Indicator);
    s_inst = this;
    d->buildMenu();
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

/// @todo Add an enumeration that allows the callee to specify the kind of message icon they'd  want to appear.
void Indicator::presentMessage (const QString& p_title, const QString& p_message, const int& p_timeout, const Indicator::Message* p_messageIndicator)
{
    if (!Indicator::Message::exists (p_messageIndicator->key()))
        Indicator::Message::create (p_messageIndicator->key(), p_message, true);

    if (p_messageIndicator->enabled())
        instance()->d_func()->m_icon->showMessage (p_title, p_message, QSystemTrayIcon::Information, p_timeout);
}

void Indicator::addActionForPlugins (QAction* p_action)
{
    instance()->d_func()->m_menuPlugins->insertAction (0, p_action);
}

void Indicator::removeActionForPlugins (QAction* p_action)
{
    instance()->d_func()->m_menuPlugins->removeAction (p_action);
}

void Indicator::on_actionDesktopControlOptions_triggered ()
{
    Windows::Settings::displayPane ("dsktpcntrl");
}

void Indicator::on_actionDictationOptions_triggered ()
{
    Windows::Settings::displayPane ("dctn");
}

void Indicator::on_actionDictationToggle_toggled (const bool& p_checked)
{
    p_checked ? Dictation::Service::instance()->start() : Dictation::Service::instance()->stop();
}

void Indicator::on_actionDesktopControlToggle_toggled (const bool& p_checked)
{
    p_checked ? DesktopControl::Service::instance()->start() : DesktopControl::Service::instance()->stop();
}

void Indicator::on_actionOptions_triggered()
{
    Windows::Settings::displayPane();
}

void Indicator::on_actionAboutSpeechControl_triggered()
{
    Windows::AboutDialog dialog;
    dialog.exec();
}

Indicator::~Indicator()
{

}

IndicatorPrivate::IndicatorPrivate() : m_icon (new QSystemTrayIcon (QApplication::windowIcon())),
    m_actionDesktopControlOptions (0), m_actionDesktopControlToggle (0),
    m_actionDictationToggle (0), m_actionDictationOptions (0),
    m_actionPluginOptions (0), m_actionAboutSpeechControl (0),
    m_actionAboutQt (0), m_actionHelpManual (0)
{
    m_icon->setIcon (Indicator::icon().pixmap (48, 48));
    m_icon->show();
}

void IndicatorPrivate::buildActions()
{
    m_actionDesktopControlOptions = new QAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance());
    m_actionDesktopControlToggle = new QAction ("&Active", Indicator::instance());
    m_actionDictationOptions = new QAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance());
    m_actionDictationToggle = new QAction ("&Active", Indicator::instance());

    Indicator::instance()->connect (m_actionDesktopControlToggle, SIGNAL (toggled (bool)), SLOT (on_actionDesktopControlToggle_toggled (bool)));
    Indicator::instance()->connect (m_actionDictationToggle, SIGNAL (toggled (bool)), SLOT (on_actionDictationToggle_toggled (bool)));
    Indicator::instance()->connect (m_actionDesktopControlOptions, SIGNAL (triggered (bool)), SLOT (on_actionDesktopControlOptions_triggered()));
    Indicator::instance()->connect (m_actionDictationOptions, SIGNAL (triggered (bool)), SLOT (on_actionDictationOptions_triggered (bool)));

    m_actionDesktopControlToggle->setCheckable (true);
    m_actionDictationToggle->setCheckable (true);
    m_actionDesktopControlToggle->setChecked (DesktopControl::Service::instance()->isEnabled());
    m_actionDictationToggle->setChecked (Dictation::Service::instance()->isEnabled());
}

void IndicatorPrivate::buildMenu()
{
    buildActions();
    m_menu = new QMenu;

    m_menuDesktopControl = m_menu->addMenu (QIcon::fromTheme ("audio-headset"), "Desktop Control");
    m_menuDesktopControl->addActions (QList<QAction*>()
                                      << m_actionDesktopControlToggle
                                      << m_actionDesktopControlOptions
                                     );

    m_menuDictation      = m_menu->addMenu (QIcon::fromTheme ("audio-input-microphone"), "Dictation");
    m_menuDictation->addActions (QList<QAction*>()
                                 << m_actionDictationToggle
                                 << m_actionDictationOptions
                                );

    m_menuPlugins        = m_menu->addMenu (QIcon::fromTheme ("configure"), "Plug-ins");
    m_menuPlugins->addSeparator();
    m_menuPlugins->addAction (m_actionPluginOptions);

    m_menuHelp           = m_menu->addMenu (QIcon::fromTheme ("help"), "Help");
    m_actionAboutQt = m_menuHelp->addAction (QIcon::fromTheme ("qt"), "About &Qt", QApplication::instance(), SLOT (aboutQt()));
    m_actionAboutSpeechControl = m_menuHelp->addAction (QApplication::windowIcon(), "&About SpeechControl", Indicator::instance(), SLOT (on_actionAboutSpeechControl_triggered()));

    m_menu->addMenu (m_menuDesktopControl);
    m_menu->addMenu (m_menuDictation);
    m_menu->addMenu (m_menuPlugins);
    m_menu->addSeparator();
    m_menu->addAction (QIcon::fromTheme ("configure"), "&Options", Indicator::instance() , SLOT (on_actionOptions_triggered()));
    m_menu->addMenu (m_menuHelp);
    m_menu->addSeparator();
    m_menu->addAction (QIcon::fromTheme ("application-exit"), "Quit", Core::instance(), SLOT (quit()));

    m_icon->setContextMenu (m_menu);
}

IndicatorPrivate::~IndicatorPrivate()
{
    m_icon->hide();
}


#include "indicator.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

