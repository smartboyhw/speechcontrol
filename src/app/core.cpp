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
#include <QSettings>

#ifndef HAVE_KDE
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#else
#include <KDebug>
#include <KLocalizedString>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KApplication>
#include <KMessageBox>
#endif

// libspchcntrl includes
#include <lib/system.hpp>
#include <lib/microphone.hpp>
#include <lib/dummysc.hpp>

// local includes
#include "app/core.hpp"
#include "app/factory.hpp"
#include "app/sessions/session.hpp"
#include "app/windows/main-window.hpp"
#include "app/windows/quickstart-wizard.hpp"

using namespace SpeechControl;

using namespace SpeechControl::Wizards;

using SpeechControl::Core;

Core* Core::s_inst = 0;

/// @todo Add a check for the default microphone (if provided by the user).
Core::Core ( int argc, char** argv ) :
#ifndef HAVE_KDE
    QObject ( new QApplication ( argc, argv ) )
#else
    QObject ( 0 )
#endif
{
    s_inst = this;

#ifdef HAVE_KDE
    KAboutData l_abt ( "speechcontrol",
                       0,
                       ki18n ( "Speech Control" ),
                       SPCHCNTRL_BUILD_VERSION,
                       ki18n ( "Speech Recognition for Linux." ),
                       KAboutData::License_GPL_V3,
                       ki18n ( "Copyright (c) 2010 - 2012 Synthetic Intellect Institute" ),
                       ki18n ( "SpeechControl provides the basis of speech recognition technologies on Linux and quality-grade software equal to its alternative, Dragon NaturallySpeaking. " ),
                       "http://www.thesii.org",
                       "http://tasks.thesii.org/enter_bug.cgi?product=speechcontrol"
                     );

    l_abt.addAuthor ( ki18n ( "Jacky Alcine" )  , ki18n ( "Core Developer, Maintainer" ), "jacky.alcine@thesii.org", "http://jackyalcine.co.cc" );
    l_abt.addAuthor ( ki18n ( "Adrian Borucki" ), ki18n ( "Core Developer" ), "adrian@thesii.org" );
    l_abt.addCredit ( ki18n ( "Dante Ashton" )  , ki18n ( "CPU of Synthetic Intellect Institute" )  , "dante.ashton@thesii.org" );
    l_abt.setBugAddress ( "bugzilla@thesii.org" );

    KCmdLineArgs::init ( argc, argv, &l_abt );

    KCmdLineOptions l_options;
    m_app = new KApplication;
    setParent ( m_app );
#else
    // start application.
    m_app = qobject_cast< QApplication* > ( QApplication::instance() );
    m_app->setApplicationName ( "SpeechControl" );
    m_app->setOrganizationDomain ( "thesii.org" );
    m_app->setOrganizationName ( "Synthetic Intellect Institute" );
    m_app->setApplicationVersion ( SPCHCNTRL_BUILD_VERSION );
#endif /* HAVE_KDE */

    System::start ( &argc, &argv );
    Session::init();

    QDir l_dir;
    l_dir.mkdir ( QDir::homePath() + "/.speechcontrol/contents" );

    // build settings
    m_settings = new QSettings ( QSettings::UserScope, "Synthetic Intellect Institute", "SpeechControl", this );
    connect ( m_app,SIGNAL ( aboutToQuit() ),this,SLOT ( stop() ) );
    connect ( this,SIGNAL ( started() ),Plugins::Factory::instance(),SLOT ( start() ) );
    connect ( this,SIGNAL ( stopped() ),Plugins::Factory::instance(),SLOT ( stop() ) );
}

Core::~Core () {
    m_settings->sync();
}

void Core::start() {
    instance()->s_mw = new Windows::Main;

    // Detect if a first-run wizard should be run.
    if ( !QFile::exists ( s_inst->m_settings->fileName() ) ) {
        if ( QMessageBox::question ( instance()->s_mw, tr ( "First Run" ),
                                     tr ( "This seems to be the first time you've run SpeechControl on this system. "
                                          "A wizard allowing you to start SpeechControl will appear." ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
            QuickStart* l_win = new QuickStart ( instance()->s_mw );
            l_win->exec();
        }
    }


    emit instance()->started();

    instance()->s_mw->show();
}

Windows::Main* Core::mainWindow() {
    return instance()->s_mw;
}

void Core::stop() {
    qDebug() << "Core stopped.";
    emit instance()->stopped();
}

/// Experimental
void Core::asrFinished ( QString& text ) {
    qDebug() << "[ASR] Completed with " << text;
    dummyASR->run();
}

QVariant Core::configuration ( const QString &p_pth, QVariant p_vrt ) {
    return instance()->m_settings->value ( p_pth, p_vrt );
}

void Core::setConfiguration ( const QString& p_pth, const QVariant& p_vrt ) {
    instance()->m_settings->setValue ( p_pth, p_vrt );
}

Core * SpeechControl::Core::instance() {
    return Core::s_inst;
}

int Core::exec() {
    return instance()->m_app->exec();
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
