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
 * @date 05/16/12 20:58:06 PM
 */

#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <lib/system.hpp>
#include "app/factory.hpp"
#include "app/indicator.hpp"
#include "app/sessions/session.hpp"
#include "app/services/engine.hpp"
#include "app/services/voxforge/service.hpp"
#include "app/ui/quickstart-wizard.hpp"
#include "app/coreprivate.hpp"
#include "app/core.hpp"

using namespace SpeechControl;
SPCHCNTRL_DEFINE_SINGLETON (Core)

CorePrivate::CorePrivate (Core* p_qPtr) : m_app (0),
    m_settings (0), m_trnsltr (0), q_ptr (p_qPtr)
{
    Q_Q (Core);
    m_trnsltr = new QTranslator (q);
    m_settings = new QSettings (QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", q);

    m_app->installTranslator (m_trnsltr);
}

void CorePrivate::hookUpSignals()
{
    Q_Q (Core);
    QObject::connect (m_app, SIGNAL (aboutToQuit()), q, SLOT (stop()));
    QObject::connect (q, SIGNAL (started()), ServiceEngine::instance(), SLOT (start()));
    QObject::connect (q, SIGNAL (started()), Factory::instance(), SLOT (start()));

    QObject::connect (q, SIGNAL (stopped()), ServiceEngine::instance(), SLOT (stop()));
    QObject::connect (q, SIGNAL (stopped()), Factory::instance(), SLOT (stop()));

    ServiceEngine::start();
}

CorePrivate::~CorePrivate()
{

}

Core::Core() : QObject()
{
    qFatal ("This constructor shouldn't ever be called.");
}

Core::Core (int p_argc, char** p_argv, QApplication* app) : QObject (app),
    d_ptr (new CorePrivate (this))
{
    Q_D (Core);
    d->m_app = app;

    if (s_inst) {
        qFatal ("The Core instance of SpeechControl was being invoked again. This is a fatal and funny error.");
    }

    s_inst = this;

    // start application.
    System::start (&p_argc, &p_argv);
    Session::init();

    // Create application's configuration directory.
    QDir configDir;
    configDir.mkdir (configurationPath().path() + "/contents");

    d->hookUpSignals();
    loadTranslations (QLocale::system());

    Indicator::instance();
}

Core::Core (const Core& p_other) : QObject (p_other.parent()), d_ptr (const_cast<CorePrivate*> (p_other.d_ptr.data()))
{

}

void Core::start()
{
    // Detect if a first-run wizard should be run.
    if (!QFile::exists (instance()->d_func()->m_settings->fileName())) {
        if (QMessageBox::question (0, tr ("First Run"),
                                   tr ("This seems to be the first time you've run SpeechControl on this system. "
                                       "A wizard allowing you to start SpeechControl will appear."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            Windows::QuickStart wiz;
            wiz.exec();
        }
    }

    emit instance()->started();
}

void Core::stop()
{
    emit instance()->stopped();
}

QVariant Core::configuration (const QString& p_attrName, QVariant p_attrDefValue)
{
    instance()->d_func()->m_settings->sync();
    return instance()->d_func()->m_settings->value (p_attrName, p_attrDefValue);
}

void Core::setConfiguration (const QString& p_attrName, const QVariant& p_attrValue)
{
    instance()->d_func()->m_settings->setValue (p_attrName, p_attrValue);
    instance()->d_func()->m_settings->sync();
}

int Core::exec()
{
    return instance()->d_func()->m_app->exec();
}

void Core::quit (const int& p_exitCode)
{
    instance()->d_func()->m_app->exit (p_exitCode);
}

void Core::loadTranslations (const QLocale& p_locale)
{
    qDebug() << "Loading locale" << p_locale.name();
    instance()->d_func()->m_trnsltr->load ("speechcontrol_" + p_locale.name());
}

QDir Core::configurationPath()
{
    return QDir (QDir::homePath().append ("/.config/speechcontrol"));
}

void Core::setAutoStart (const bool p_toggle)
{
    QFile* autoStartFile = new QFile (QDir::homePath().append ("/.config/autostart/SpeechControl.desktop"));

    if (p_toggle) {
        if (autoStartFile->open (QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream desktopFile (autoStartFile);
            desktopFile << "[Desktop Entry]" << endl
                        << "Name=Start SpeechControl on Launch" << endl
                        << "Icon=speechcontrol" << endl
                        << "Exec=speechcontrol" << endl
                        << "Terminal=false" << endl
                        << "Type=Application" << endl
                        << "X-GNOME-Autostart-enabled=true" << endl
                        << "X-GNOME-Autostart-Delay=30" << endl;
            autoStartFile->close();
            autoStartFile->setPermissions (autoStartFile->permissions() | QFile::ExeUser | QFile::ExeOwner | QFile::ExeGroup);
        }
    }
    else {
        if (autoStartFile->exists()) {
            autoStartFile->remove();
        }
    }
}

bool Core::doesAutoStart()
{
    QFile* autoStartFile = new QFile (QDir::homePath().append ("/.config/autostart/SpeechControl.desktop"));
    return autoStartFile->exists();
}

Core::~Core ()
{
    Q_D (Core);
    d->m_settings->sync();
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
