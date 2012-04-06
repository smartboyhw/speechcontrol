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

#include "sessions/content.hpp"
#include "contents-wizard.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "wizards/contents/selection.hpp"
#include "core.hpp"

#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QVariant>

using namespace SpeechControl;
using namespace SpeechControl::Windows::Wizards;

ContentWizard::ContentWizard (QWidget* parent) :
    AbstractWizard (parent), m_src (0)
{
    QIcon l_icon = QIcon::fromTheme ("text-plain");
    setPixmap (QWizard::LogoPixmap, l_icon.pixmap (32, 32, QIcon::Active, QIcon::On));
    setWindowTitle (tr ("Content Addition Wizard - SpeechControl"));
    setPage (ContentWizard::IntroductionPage,
             (new Windows::Wizards::Pages::IntroductionPage (tr ("This wizard allows you to add a new source of content into SpeechControl's collection of content."
                     "In SpeechControl, the term <b>content</b> refers to arbitrary text that's been formatted so "
                     "that SpeechControl can efficiently render it for training. Check the Synthetic Intellect Institute's "
                     "wiki for more information at "
                     "<a href=\"http://wiki.thesii.org/UserGuides/AddingContent\">http://wiki.thesii.org/UserGuides/AddingContent</a>."
                                                       ))));
    setPage (ContentWizard::SourceSelectionPage,
             (new Windows::Wizards::Pages::SourceSelectionPage));
    setPage (ContentWizard::ConclusionPage,
             (new Windows::Wizards::Pages::ConclusionPage (tr ("You've successfully added a Content into SpeechControl. With this new Content, you can now "
                     "start a new Session and train your acoustic models.Check the Synthetic Intellect Institute's "
                     "wiki for more information at "
                     "<a href=\"http://wiki.thesii.org/UserGuides/Training#Introduction\">http://wiki.thesii.org/UserGuides/Training#Introduction</a>."
                                                     ))));
}

AbstractContentSource* ContentWizard::source()
{
    return m_src;
}

void ContentWizard::setSource (AbstractContentSource* p_src)
{
    if (p_src) {
        m_src = new AbstractContentSource (*p_src);
        qDebug() << "[ContentWizard::setSource()] Got source type" << m_src->id();
    }
    else {
        m_src = 0;
    }
}

int ContentWizard::nextId() const
{
    switch (currentId()) {
    case IntroductionPage:
        return SourceSelectionPage;
        break;

    case SourceSelectionPage:
        return ConclusionPage;
        break;

    case ConclusionPage: {
        if (m_src) {
            if (!m_src->isValid()) {
                QMessageBox::warning (0,
                                      tr ("Failure Creating Content"),
                                      tr ("There was an issue creating your content; thus resulting in a failure."),
                                      QMessageBox::Ok
                                     );
                return SourceSelectionPage;
            }
        }

        return -1;
    }
    break;
    }

    return QWizard::nextId();
}

void ContentWizard::accept()
{
    m_src->generate();
    QDialog::accept();
}

ContentWizard::~ContentWizard()
{
}

#include "ui/contents-wizard.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
