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

#include "micsample.hpp"
#include "app/windows/micsetup-wizard.hpp"
#include "ui_micwizard-sample.h"

using namespace SpeechControl;
using namespace SpeechControl::Wizards::Pages;
using SpeechControl::Wizards::Pages::MicrophoneSample;

/// @todo The loudness of the content spoken should begin detection here.
MicrophoneSample::MicrophoneSample (QWidget* parent) :
    QWizardPage (parent), ui (new Ui::MicrophoneSample), m_sphnx (new MicrophoneSample::Sphinx),
    m_index (0)
{
    m_prompts << "To divide something is to separate it into its component parts."
              << "It was like the revival of an old melodrama that I had seen long ago with childish awe."
              << "Ultimately, when you're writing an extended definition, you'll need to make it as clear, real and understandable as possible."
              << "On May 10, 1984, Judge Bruce S. Jenkins handed down his opinion."
              << "My father's memory was correct."
              << "Writing a first draft is very much like watching a Polaroid develop."
              ;
    ui->setupUi (this);
    this->setLayout (ui->verticalLayout);
    connect (m_sphnx, SIGNAL (finished (QString)), this, SLOT (handleReceivedPrompt (QString)));;
    connect(ui->btnBeginPrompt,SIGNAL(clicked()),this,SLOT(on_btnBeginPrompt_clicked()));
}

MicrophoneSample::~MicrophoneSample()
{
    delete ui;
}

/// @todo Have this use the Microphone defined in the previous step.
void MicrophoneSample::on_btnBeginPrompt_clicked()
{
    m_sphnx->start();
    m_index = 0;
    ui->btnBeginPrompt->setEnabled(false);
    updateUi();
}

/// @todo Fill the combo box with all of the mics.
void SpeechControl::Wizards::Pages::MicrophoneSample::initializePage()
{
    ui->lblPrompt->setText (QString::null);
}

bool SpeechControl::Wizards::Pages::MicrophoneSample::validatePage()
{
    return isAtFinalPrompt();
}

bool SpeechControl::Wizards::Pages::MicrophoneSample::isComplete()
{
    return hasCompletedPrompts();
}

bool MicrophoneSample::hasCompletedPrompts()
{
    return m_index == -1;
}

bool MicrophoneSample::isAtFinalPrompt()
{
    return m_index == m_prompts.length() - 1;
}

void MicrophoneSample::updateUi()
{
    if (m_index < m_prompts.length()){
        ui->lblPrompt->setText(m_prompts[m_index]);
        m_index++;
    }
}

/// @todo Consider moving the algorithm for determining accuracy into AbstractSphinx.
void MicrophoneSample::handleReceivedPrompt (QString p_str)
{
    qDebug() << "[MicrophoneSample::handleReceivedPrompt()] Prompt was" << m_prompts[m_index]
             << "User said:" << p_str;

    QStringList wordsFound = p_str.split(" ");
    QStringList wordsHave = m_prompts[m_index].split(" ");
    const bool isTheSameSize = wordsHave.length() == wordsFound.length();

    /*
     * First, determine if the wordsFound is the same size as wordsHave.
     *  If so, you can then take a count of which words are the same and make a
     *  proportion from that. (similarWordCount/totalSize). That would be a percent.
     *  Take that value and multiply by the percentile (100/m_prompts.length) and then
     *  add it to the progress bar.
     *
     *  If the phrases don't have the same number of words (wordsFound <> wordsHave):
           Remove words found that aren't originally in the phrase.
           Then enact the steps as if they were the same size.
     */

    if (!isTheSameSize){
        Q_FOREACH(const QString wordFound, wordsFound){
            if (!wordsHave.contains(wordFound))
                wordsFound.removeOne(wordFound);
        }
    }

    int equivalenceCount = 0;

    Q_FOREACH(const QString wordHave, wordsHave){
        if (wordsFound.contains(wordHave))
            equivalenceCount += 1;
    }

    double percentEqual = ((double) equivalenceCount / (double) wordsHave.length());
    int increment = (100 / m_prompts.length()) * percentEqual;

        qDebug() << "[MicrophoneSample::handleReceivedPrompt()] Accuracy of current phrase: " << (int) (percentEqual * 100) << "%";
    ui->progressBarRate->setValue(ui->progressBarRate->value() + increment);
}

void MicrophoneSample::Sphinx::applicationMessage (const QGst::MessagePtr& p_message)
{
    QString l_msgType    = p_message->internalStructure()->name();
    QString l_hypothesis = p_message->internalStructure()->value ("hyp").toString();
    QString l_uttid      = p_message->internalStructure()->value ("uttid").toString();

    if (l_msgType == "result") {
        qDebug() << "[MicrophoneSample::Sphinx::applicationMessage()] Obtained hypothesis" << l_hypothesis << "from user...";
        emit finished (l_hypothesis);
    }
}

#include "micsample.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
