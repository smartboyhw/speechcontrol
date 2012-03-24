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
#ifndef ADAPT_ENACTIONSELECTION_HPP
#define ADAPT_ENACTIONSELECTION_HPP

#include <QWizardPage>
#include <app/sessions/adaptionutility.hpp>

namespace Ui
{
class Enaction;
}

namespace SpeechControl
{

namespace Wizards
{
namespace Pages
{

typedef SpeechControl::AdaptationUtility::Phases Phases;
class Enaction : public QWizardPage
{
    Q_OBJECT

public:
    explicit Enaction (QWidget* parent = 0);
    ~Enaction();
    virtual void initalizePage();
    virtual void cleanupPage();
    virtual bool isComplete() const;

private slots:
    void on_btnAdapt_clicked();

private:
    Ui::Enaction* ui;
    AdaptationUtility* m_utility;
public slots:
    void on_mUtility_phaseStarted (const Phases& p_phase);
    void on_mUtility_phaseEnded (const Phases& p_phase);
};

}
}
}

#endif // ADAPT_ENACTIONSELECTION_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
