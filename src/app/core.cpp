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
#include "system.hpp"
#include "microphone.hpp"
#include "dummysc.hpp"

// local includes
#include "core.hpp"
#include "factory.hpp"
#include "session.hpp"
#include "windows/main-window.hpp"
#include "windows/quickstart-wizard.hpp"

using namespace SpeechControl;

using namespace SpeechControl::Wizards;

using SpeechControl::Application::Core;

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
                       KLocalizedString(),
                       "http://www.thesii.org",
                       "http://tasks.thesii.org/buglist.cgi?query=product:spchcntrl"
                     );

    l_abt.addAuthor ( ki18n ( "Jacky Alcine" ) , ki18n ( "" ), "jacky.alcine@thesii.org" );
    l_abt.addAuthor ( ki18n ( "Adrian Borucki" ) , ki18n ( "" ), "adrian@thesii.org" );

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

    dummyASR = new DummySC ( DummySC::getStandardDescription() );
    connect ( dummyASR, SIGNAL ( finished ( QString& ) ), this, SLOT ( asrFinished ( QString& ) ) );
    connect ( this,SIGNAL ( started() ),dummyASR,SLOT ( run() ) );

    Plugins::Factory::availablePlugins();
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

    // Load enabled plug-ins.
    const QStringList l_plgnLst = s_inst->m_settings->value ( "Plugins/AutoStart" ).toStringList();
    Q_FOREACH ( const QString l_plgn, l_plgnLst ) {
        Plugins::Factory::loadPlugin ( QUuid ( l_plgn ) );
    }

    instance()->s_mw->show();
}

Windows::Main* Core::mainWindow() {
    return instance()->s_mw;
}

void Core::stop() { }

/// Experimental
void Core::asrFinished ( QString& text ) {
    qDebug() << "[ASR] Completed with " << text;
    dummyASR->run();
}

QVariant Core::configuration ( const QString &p_pth, QVariant p_vrt ) const {
    return m_settings->value ( p_pth, p_vrt );
}

void Core::setConfiguration ( const QString &p_pth, const QVariant &p_vrt ) {
    m_settings->setValue ( p_pth, p_vrt );
}

Core * SpeechControl::Core::instance() {
    return Core::s_inst;
}

int Core::exec() {
    return m_app->exec();
}

#include "core.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
