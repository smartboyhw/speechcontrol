/***
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QFile>
#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <QProcess>
#include <QErrorMessage>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMenu>

#include <lib/abstractaudiosource.hpp>

#include "core.hpp"
#include "indicator.hpp"
#include "dictation/agent.hpp"
#include "desktopcontrol/agent.hpp"
#include <engine.hpp>
#include "sessions/session.hpp"
#include "sessions/content.hpp"
#include "ui/main-window.hpp"
#include "ui/training-dialog.hpp"
#include "ui/settings-dialog.hpp"
#include "ui/content-manager.hpp"
#include "ui/session-manager.hpp"
#include "ui/about-dialog.hpp"
#include "ui/quickstart-wizard.hpp"
#include "ui/micsetup-wizard.hpp"
#include "ui/contents-wizard.hpp"
#include "ui/adapt-wizard.hpp"
#include "session-information-dialog.hpp"
#include "ui_main-window.h"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

using SpeechControl::Windows::Main;

/// @todo Add icons to the QActions.
Main::Main() : m_ui (new Ui::MainWindow), m_prgStatusbar (0)
{
    m_ui->setupUi (this);
    m_ui->retranslateUi (this);
    m_prgStatusbar = new QProgressBar (this);

    // Redo layout
    m_ui->centralwidget->setLayout (m_ui->gLayoutMain);
    m_ui->groupBoxSessions->setLayout (m_ui->gridLayoutSessions);
    m_ui->groupBoxRecognition->setLayout (m_ui->hLayoutRecognition);
    m_ui->groupBoxServices->setLayout (m_ui->gridLayoutServices);

    // Do a bit of cleanup on the status bar.
    m_ui->statusBar->addPermanentWidget (m_prgStatusbar);
    m_prgStatusbar->setMaximum (100);
    setProgress (0.0);

    // Update icons.
    m_ui->actionQuit->setIcon (QIcon::fromTheme ("application-exit"));
    m_ui->actionAboutSpeechControl->setIcon (QIcon (":/logo/sc"));
    m_ui->actionReportBug->setIcon (QIcon::fromTheme ("tools-report-bug"));
    m_ui->actionOptions->setIcon (QIcon::fromTheme ("configure"));
    m_ui->actionWizardMicrophone->setIcon (QIcon::fromTheme ("audio-input-microphone"));
    m_ui->menuDictation->setIcon (QIcon::fromTheme ("audio-input-microphone"));
    m_ui->menuDesktopControl->setIcon (QIcon::fromTheme ("audio-headset"));
    m_ui->actionWizardContent->setIcon (QIcon::fromTheme ("x-office-document"));
    m_ui->actionWizardAdaption->setIcon (QIcon::fromTheme ("configure"));
    m_ui->actionDesktopControlCommands->setIcon (QIcon::fromTheme ("view-list-text"));
    m_ui->actionStartTraining->setIcon (QIcon::fromTheme ("system-run"));
    m_ui->actionHelp->setIcon (QIcon::fromTheme ("help"));

    // Update the actions and buttons.
    connect (DesktopControl::Agent::instance(), SIGNAL (stateChanged (ActivityState)), this, SLOT (desktopControlStateChanged()));
    connect (Dictation::Agent::instance(), SIGNAL (stateChanged (ActivityState)), this, SLOT (dictationStateChanged()));
    desktopControlStateChanged();
    dictationStateChanged();
    on_actionDesktopControlActive_triggered (DesktopControl::Agent::instance()->isActive());
    on_actionDictationActive_triggered (Dictation::Agent::instance()->isActive());

    // Greet the user :)
    setStatusMessage (tr ("Welcome to %1, speech recognition for Linux.").arg (QApplication::applicationName()), 4000);
}

void Main::closeEvent (QCloseEvent* p_closeEvent)
{
    qDebug() << "[Windows::Main::hide()] Is indicator visible?" << Indicator::instance()->isVisible();

    if (Indicator::instance()->isVisible()) {
        QErrorMessage* l_msg = new QErrorMessage (this);
        l_msg->setModal (true);
        l_msg->setWindowTitle (tr ("Hidden From View"));
        l_msg->showMessage (tr ("SpeechControl has been hidden into the system's tray."
                                "You can restore it by clicking the menu and selecting 'Restore'."), "HiddenToTray");
        p_closeEvent->ignore();
        this->hide();
    }
    else {
        if (QMessageBox::Yes == QMessageBox::question (this, "Confirm Quit", "Are you sure you want to quit SpeechControl?", QMessageBox::Yes | QMessageBox::No)) {
            p_closeEvent->accept();
            QApplication::quit();
        }
        else {
            p_closeEvent->ignore();
        }
    }

    if (Core::configuration ("MainWindow/RememberState").toBool()) {
        Core::setConfiguration ("MainWindow/Geometry", saveGeometry());
        Core::setConfiguration ("MainWindow/State", saveState());

        if (Indicator::isEnabled())
            Core::setConfiguration ("MainWindow/Visible", isVisible());
        else
            Core::setConfiguration ("MainWindow/Visible", true);
    }
}

void Main::open()
{
    if (DeviceAudioSource::allDevices().empty()) {
        Indicator::presentMessage (tr ("No Microphone Found"),
                                   tr ("No microphones were founds on your system."),
                                   2000,
                                   new Indicator::Message ("NoMicrophoneFound")
                                  );

        QErrorMessage* l_msg = new QErrorMessage (this);
        l_msg->setModal (true);
        l_msg->setWindowTitle (tr ("No Microphones Found"));
        l_msg->showMessage (tr ("No microphones were found on your system. Please ensure that you have one installed and detectable by "
                                "the audio system and make sure that <b>%1</b> is installed on your system.").arg ("gstreamer-plugins-good"),
                            "NoMicrophonesFoundOnStart");
    }

    updateUi();

    if (Core::configuration ("MainWindow/RememberState").toBool()) {
        restoreGeometry (Core::configuration ("MainWindow/Geometry").toByteArray());
        restoreGeometry (Core::configuration ("MainWindow/State").toByteArray());
        const bool isIndicatorVisible = Indicator::isVisible() && Indicator::isEnabled();
        const bool isMainWindowVisible = Core::configuration ("MainWindow/Visible").toBool() == true;

        if (isIndicatorVisible || isMainWindowVisible)
            QMainWindow::show();
        else
            QMainWindow::hide();
    }
    else {
        QMainWindow::show();
    }
}

void Main::setStatusMessage (const QString& p_message , const int p_timeout)
{
    m_ui->statusBar->showMessage (p_message, p_timeout);
}

void Main::desktopControlStateChanged()
{
    QString msg;

    switch (DesktopControl::Agent::instance()->state()) {
    case AbstractAgent::ActivityState::Enabled:
        msg = tr ("Desktop control activated.");
        break;

    default:
    case AbstractAgent::ActivityState::Disabled:
        msg = tr ("Desktop control deactivated.");
        break;
    }

    Indicator::presentMessage (tr ("Desktop Control State Changed"),
                               msg,
                               2000,
                               new Indicator::Message ("DesktopControlStateChanged")
                              );
    setStatusMessage (msg);
}

void Main::dictationStateChanged()
{
    QString msg;

    switch (Dictation::Agent::instance()->state()) {
    case AbstractAgent::ActivityState::Enabled:
        msg = tr ("Dictation activated.");
        break;

    default:
    case AbstractAgent::ActivityState::Disabled:
        msg = tr ("Dictation deactivated.");
        break;
    }

    Indicator::presentMessage (tr ("Dictation State Changed"),
                               msg,
                               2000,
                               new Indicator::Message ("DictationStateChanged")
                              );
    setStatusMessage (msg);
}

void Main::updateUi()
{
    updateRecognitionInfo();
    updateSessionListing();
    updateServiceListing();
}

void Main::updateRecognitionInfo()
{

}

void Main::updateSessionListing()
{
    QListWidget* widget = m_ui->listWidgetSessions;
    widget->clear();

    SessionList sessions = Session::allSessions();

    Q_FOREACH (const Session * session, sessions) {
        QListWidgetItem* item = new QListWidgetItem (widget);
        item->setText (session->name());
        item->setData (Qt::UserRole, session->id());
        widget->addItem (item);
        qDebug() << "[Main::updateSessionListingListing()] Added session " << session->name() << "to listing.";
    }

    m_ui->btnSessionAdapt->setEnabled (false);
    m_ui->btnSessionTrain->setEnabled (false);
    m_ui->btnSessionInfo->setEnabled (false);
    widget->clearSelection();
}

void Main::updateServiceListing()
{
    QListWidget* widget = m_ui->listWidgetService;
    widget->clear();

    Services::AbstractModuleList list = Services::Engine::allModules();

    Q_FOREACH (const Services::AbstractModule * module, list) {
        QListWidgetItem* item = new QListWidgetItem (widget);
        item->setText (module->name());
        item->setCheckState (module->isActive() ? Qt::Checked : Qt::Unchecked);
        item->setHidden (!module->isEnabled());
        item->setIcon (module->pixmap());
        item->setData (Qt::UserRole, module->id());
        widget->addItem (item);
        qDebug() << "[Main::updateServiceListing()] Added service " << module->name() << "to listing.";
    }

    widget->clearSelection();
}

void Main::on_listWidgetService_itemClicked (QListWidgetItem* p_item)
{
    bool isChecked = p_item->checkState() == Qt::Checked;
    Services::AbstractModule* module = Services::Engine::findModule (p_item->data (Qt::UserRole).toString());

    (isChecked) ? module->start() : module->stop();
    qDebug() << "[Main::on_listWidgetService_itemClicked()] Is checked?" << isChecked;
    updateServiceListing();
}

void Main::setProgress (const double p_progress)
{
    const int l_val = (int) (p_progress * 100);

    if (l_val == 0 || l_val == 100) {
        m_prgStatusbar->hide();
    }
    else {
        m_prgStatusbar->show();
        m_prgStatusbar->setValue (l_val);
    }
}

void Main::on_btnAllServices_clicked()
{
    Settings::displayPane ("srvcs");
    updateUi();
}

void Main::on_actionDesktopControlOptions_triggered()
{
    Settings::displayPane ("dsktpcntrl");
    updateUi();
}

void Main::on_actionOptions_triggered()
{
    Settings::displayPane ("gnrl");
    updateUi();
}

void Main::on_actionAdaptModels_triggered()
{
    AdaptWizard* wiz = new AdaptWizard (this);
    wiz->exec();
    updateUi();
}

void Main::on_actionStartTraining_triggered ()
{
    Session* session = SessionManager::pickSession();

    updateUi();

    if (session && session->isValid() && !session->isCompleted()) {
        TrainingDialog::startTraining (session);
        setStatusMessage (tr ("Training session \"%1\"").arg (session->content()->title()) , 3000);
    }

    updateUi();
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDesktopControlActive_triggered (bool p_checked)
{
    if (p_checked && Dictation::Agent::instance()->isActive())
        return;

    DesktopControl::Agent::instance()->setState (p_checked ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled);
    setStatusMessage ( (p_checked ? tr ("Desktop control activated.") : tr ("Desktop control deactivated.")) , 3000);
    Indicator::presentMessage ("Desktop Control");
    updateUi();
}

/// @todo Allow configuration option to show specific notifications to prevent noise.
void Main::on_actionDictationActive_triggered (const bool p_checked)
{
    if (p_checked && DesktopControl::Agent::instance()->isActive())
        return;

    Dictation::Agent::instance()->setState ( (p_checked) ? SpeechControl::AbstractAgent::Enabled : SpeechControl::AbstractAgent::Disabled);
    setStatusMessage ( ( (p_checked) ? tr ("Dictation activated.") : tr ("Dictation deactivated."))  , 3000);
    updateUi();
}

void Main::on_actionCreateSession_triggered()
{
    Content* content = ContentManager::pickContent();

    if (content) {
        Session* session = Session::create (content);
        TrainingDialog::startTraining (session);
    }

    updateUi();
}

void Main::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

void Main::on_actionAboutSpeechControl_triggered()
{
    AboutDialog dlg (this);
    dlg.exec();
}

void Main::on_actionPluginOptions_triggered()
{
    Settings::displayPane ("plgns");
    updateUi();
}

void Main::on_actionTrainingOptions_triggered()
{
    Settings::displayPane ("trnng");
    updateUi();
}

void Main::on_actionDictationOptions_triggered()
{
    Settings::displayPane ("dctn");
    updateUi();
}

void Main::on_actionWizardMicrophone_triggered()
{
    MicrophoneSetup wiz (this);
    wiz.exec();
    updateUi();
}

void Main::on_actionWizardContent_triggered()
{
    ContentWizard wiz (this);
    wiz.exec();
    updateUi();
}

void Main::on_actionWizardVoxForge_triggered()
{

}

void Main::on_actionWizardQuickStart_triggered()
{
    QuickStart* wiz = new QuickStart;
    wiz->exec();
    updateUi();
}

void Main::on_actionReportBug_triggered()
{
    QProcess::startDetached ("x-www-browser", (QStringList() << "http://tasks.thesii.org/reporting-bugs"));
}

void Main::on_actionHelp_triggered()
{
    QProcess::startDetached ("x-www-browser", (QStringList() << "http://thesii.org/wiki/SpeechControl/UserGuides"));
}

void Main::on_actionWizardAdaption_triggered()
{
    AdaptWizard* wiz = new AdaptWizard (this);
    wiz->exec();
    updateUi();
}

void Main::on_btnSessionAdapt_clicked()
{
    QListWidget* widget = m_ui->listWidgetSessions;
    QListWidgetItem* item = widget->currentItem();
    Session* session = Session::obtain (item->data (Qt::UserRole).toString());
    AdaptWizard* wiz = new AdaptWizard (this);
    wiz->setSession (session);
    wiz->exec();
    updateUi();
}

void Main::on_btnSessionTrain_clicked()
{
    QListWidget* widget = m_ui->listWidgetSessions;
    QListWidgetItem* item = widget->currentItem();
    Session* session = Session::obtain (item->data (Qt::UserRole).toString());
    TrainingDialog::startTraining (session);
    updateUi();
}

void Main::on_listWidgetSessions_itemSelectionChanged()
{
    QListWidget* widget = m_ui->listWidgetSessions;
    QListWidgetItem* item = widget->currentItem();
    Session* session = Session::obtain (item->data (Qt::UserRole).toString());
    m_ui->btnSessionAdapt->setEnabled (session->isCompleted());
    m_ui->btnSessionTrain->setEnabled (!session->isCompleted());
    m_ui->btnSessionInfo->setEnabled (session->isValid());
}

void Main::on_btnSessionInfo_clicked()
{
    QListWidget* widget = m_ui->listWidgetSessions;
    QListWidgetItem* item = widget->currentItem();
    Session* session = Session::obtain (item->data (Qt::UserRole).toString());
    SessionInformationDialog* dialog = new SessionInformationDialog (session);
    dialog->exec();
    updateUi();
}

Main::~Main()
{
    delete m_ui;
}

#include "main-window.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;

