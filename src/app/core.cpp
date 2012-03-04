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

// Qt includes
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QLibraryInfo>
#include <QSettings>
#include <qtranslator.h>
#include <QApplication>
#include <QMessageBox>

// libspchcntrl includes
#include <lib/config.hpp>
#include <lib/system.hpp>
#include <lib/microphone.hpp>

// local includes
#include "app/core.hpp"
#include "app/factory.hpp"
#include "app/sessions/session.hpp"
#include "app/windows/main-window.hpp"
#include "app/windows/quickstart-wizard.hpp"
#include "desktopcontrol/agent.hpp"
#include "dictation/agent.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

Core* Core::s_inst = 0;

/// @todo Add a check for the default microphone (if provided by the user).
Core::Core (int p_argc, char** p_argv, QApplication* app) :
    QObject (new QApplication (p_argc, p_argv)), m_app (app)
{
    if (s_inst) {
        qFatal ("The Core instance of SpeechControl was being invoked again. This is a fatal and funny error.");
    }

    s_inst = this;

    // start application.
    m_app->setApplicationName ("SpeechControl");
    m_app->setOrganizationDomain ("thesii.org");
    m_app->setOrganizationName ("Synthetic Intellect Institute");
    m_app->setApplicationVersion (SPCHCNTRL_BUILD_VERSION);
    m_trnsltr = new QTranslator (this);
    m_app->installTranslator (m_trnsltr);

    System::start (&p_argc, &p_argv);
    Session::init();

    // Create application's configuration directory.
    QDir configDir;
    configDir.mkdir (QDir::homePath() + "/.config/speechcontrol/contents");

    // Settings
    m_settings = new QSettings (QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", this);

    // build settings
    m_settings = new QSettings (QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", this);
    connect (m_app, SIGNAL (aboutToQuit()), this, SLOT (stop()));
    connect (this, SIGNAL (started()), this, SLOT (invokeAutoStart()));
    connect (this, SIGNAL (started()), Plugins::Factory::instance(), SLOT (start()));
    connect (this, SIGNAL (stopped()), Plugins::Factory::instance(), SLOT (stop()));
    loadTranslations (QLocale::system());
}

Core::Core (const Core& p_other) : QObject (p_other.parent())
{

}

void Core::start()
{
    instance()->s_mw = new Windows::Main;

    // Detect if a first-run wizard should be run.
    if (!QFile::exists (s_inst->m_settings->fileName())) {
        if (QMessageBox::question (instance()->s_mw, QMessageBox::tr ("First Run"),
                                   QMessageBox::tr ("This seems to be the first time you've run SpeechControl on this system. "
                                           "A wizard allowing you to start SpeechControl will appear."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            QuickStart* l_win = new QuickStart (instance()->s_mw);
            l_win->exec();
        }
    }

    emit instance()->started();

    instance()->s_mw->show();
}

Windows::Main* Core::mainWindow()
{
    return instance()->s_mw;
}

void Core::stop()
{
    emit instance()->stopped();
}

/// Experimental
void Core::asrFinished (QString& text)
{
    qDebug() << "[ASR] Completed with " << text;
}

QVariant Core::configuration (const QString& p_attrName, QVariant p_attrDefValue)
{
    return instance()->m_settings->value (p_attrName, p_attrDefValue);
}

void Core::setConfiguration (const QString& p_attrName, const QVariant& p_attrValue)
{
    instance()->m_settings->setValue (p_attrName, p_attrValue);
}

Core* SpeechControl::Core::instance()
{
    return Core::s_inst;
}

int Core::exec()
{
    return instance()->m_app->exec();
}

void Core::quit (const int& p_exitCode)
{
    instance()->m_app->exit (p_exitCode);
}

void Core::invokeAutoStart()
{
    const bool l_dsktpCntrlState = configuration ("DesktopControl/AutoStart").toBool();
    const bool l_dctnState = configuration ("Dictation/AutoStart").toBool();
    DesktopControl::Agent::instance()->setState ( (l_dsktpCntrlState) ? SpeechControl::AbstractAgent::Enabled  : SpeechControl::AbstractAgent::Disabled);
    Dictation::Agent::instance()->setState ( (l_dctnState) ? SpeechControl::AbstractAgent::Enabled  : SpeechControl::AbstractAgent::Disabled);
}

void Core::loadTranslations (const QLocale& p_locale)
{
    qDebug() << "Loading locale" << p_locale.name();
    instance()->m_trnsltr->load ("speechcontrol_" + p_locale.name());
}

Core::~Core ()
{
    m_settings->sync();
}
#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
