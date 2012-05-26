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
 * @date 05/20/12 13:17:09 PM
 */

#ifndef ADAPT_ENACTIONSELECTION_HPP
#define ADAPT_ENACTIONSELECTION_HPP

#include <QWizardPage>
#include <ui/wizards/base.hpp>
#include <sessions/adaptionutility.hpp>
#include <sessions/accuracymeter.hpp>

SPCHCNTRL_DEFINE_UIC(Enaction)
SPCHCNTRL_FORWARD_DECLARE_CLASS(Session)
namespace SpeechControl { namespace Windows { namespace Pages {

typedef SpeechControl::AdaptationUtility::Phase Phases;

class Enaction : public QWizardPage
{
    Q_OBJECT

public:
    explicit Enaction (QWidget* parent = 0);
    ~Enaction();
    virtual void initalizePage();
    virtual void cleanupPage();
    virtual bool isComplete() const;

public slots:
    void on_mUtility_phaseStarted (const Phases& p_phase);
    void on_mUtility_phaseEnded (const Phases& p_phase);
    void on_mUtility_endedAdapting();
    void on_mUtility_startedAdapting();

private slots:
    void on_btnAdapt_clicked();
    void on_mMeter_assessmentCompleted ();

private:
    void invokeAdaption (Session* p_session);
    Ui::Enaction* ui;
    AdaptationUtility* m_utility;
    SessionList m_sessions;
    AcousticModel* m_model;
    AccuracyMeter* m_meter;
};

} } }
#endif // ADAPT_ENACTIONSELECTION_HPP
// kate: indent-mode cstyle; replace-tabs on;
