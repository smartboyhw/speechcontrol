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
#include <QSettings>
#include <QApplication>
#include <QMessageBox>

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
    QObject ( new QApplication ( argc, argv ) ) {
    if ( s_inst ) {
        qFatal ( "The Core instance of SpeechControl was being invoked again. This is a fatal and funny error." );
    }

    s_inst = this;

    // start application.
    m_app = qobject_cast< QApplication* > ( QApplication::instance() );
    m_app->setApplicationName ( "SpeechControl" );
    m_app->setOrganizationDomain ( "thesii.org" );
    m_app->setOrganizationName ( "Synthetic Intellect Institute" );
    m_app->setApplicationVersion ( SPCHCNTRL_BUILD_VERSION );

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
    emit instance()->stopped();
}

/// Experimental
void Core::asrFinished ( QString& text ) {
    qDebug() << "[ASR] Completed with " << text;
    dummyASR->start();
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
