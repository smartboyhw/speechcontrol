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
#include <QErrorMessage>

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
                       "tasks.thesii.org"
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

    // check for microphones

    if ( Microphone::allMicrophones().empty() ) {
        QErrorMessage* l_msg = new QErrorMessage;
        l_msg->setWindowTitle ( "No Microphones Found" );
        l_msg->showMessage ( tr ( "No microphones were found on your system. Please ensure that you have one installed and detectable by " ) +
                             tr ( "the audio system and make sure that <b>gstreamer-plugins-good</b> is installed on your system." ),
                             "NoMicrophonesFoundOnStart" );
    }

    dummyASR = new DummySC ( DummySC::getStandardDescription() );
    connect ( dummyASR, SIGNAL ( finished ( QString& ) ), this, SLOT ( asrFinished ( QString& ) ) );
    connect ( this,SIGNAL ( started() ),dummyASR,SLOT ( run() ) );

    Plugins::Factory::availablePlugins();
}

Core::~Core () {
    m_settings->sync();
}

void Core::start() {
    Windows::Main* l_mw = new Windows::Main;

    if ( !QFile::exists ( s_inst->m_settings->fileName() ) ) {
        if ( QMessageBox::question ( l_mw, tr ( "First Run" ),
                                     tr ( "This seems to be the first time you've run SpeechControl on this system. "
                                          "A wizard allowing you to start SpeechControl will appear." ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
            QuickStart* l_win = new QuickStart ( l_mw );
            l_win->exec();
        }
    }

    l_mw->show();
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

#ifdef HAVE_KDE
#include "core.moc"
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
