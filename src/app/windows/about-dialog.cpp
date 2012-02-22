/***
 *  This file is part of SpeechControl.
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
 *  You should have received a copy of the GNU Library General Public License
 *  along with SpeechControl .  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QIcon>
#include <QDateTime>
#include <QApplication>

#include <config.hpp>

#include "about-dialog.hpp"

using namespace SpeechControl::Windows;

AboutDialog::AboutDialog(QWidget *parent) : SC_DIALOG(parent) {
  ui.setupUi(this);
  ui.versionLabel->setText(QString(tr("<b>Version:</b> %1<br><b>Built:</b> %2"))
                                     .arg(SPCHCNTRL_BUILD_VERSION)
                                     .arg(SPCHCNTRL_BUILD_DATE));
  ui.aboutTextBrowser->setHtml(about());
  ui.authorTextBrowser->setHtml(authors());
  ui.contributorTextBrowser->setHtml(contributors());
  ui.thanksToTextBrowser->setHtml(thanksTo());
  setWindowIcon(QIcon(":/logo/sc"));
}

QString AboutDialog::about() const {
  QString res;
  res = tr("<b>Speech Recognition for Linux.</b><br><br>"
           "&copy;2010 - 2012 by the Synthetic Intellect Institute<br>"
           "<a href=\"http://wiki.thesii.org/SpeechControl\">http://wiki.thesii.org/SpeechControl</a><br>"
           "<a href=\"irc://irc.freenode.net/sii-speechcontrol\">#sii-speechcontrol</a> on <a href=\"http://www.freenode.net\">Freenode</a><br><br>"
           "SpeechControl is licensed under <a href=\"http://www.gnu.org/licenses/gpl-3.0.txt\">GPLv3</a>.<br>"
           "Please use <a href=\"http://tasks.thesii.org\">http://tasks.thesii.org</a> to report bugs."
          );

  return res;
}

QString AboutDialog::authors() const {
  QString res;
  res = tr("SpeechControl is mainly developed by:") +
        "<dl>"
        "<dt><b>Jacky Alcine</b></dt><dd><a href=\"mailto:jacky.alcine@thesii.org\">jacky.alcine@thesii.org</a><br>"
             "Core Developer, Packager, Maintainer, Designer</dd>"
        "<dt><b>Adrian Borucki</b></dt><dd><a href=\"mailto:adrian@thesii.org\">adrian@thesii.org</a><br>"
             "Core Developer, Packager, Maintainer, Idea Man</dd>"
        "</dl>";

  return res;
}

QString AboutDialog::contributors() const {
  QString res;
  res = tr("We would like to thank the following contributors (in alphabetical order) and everybody we forgot to mention here:")
      + QString::fromUtf8("<br>"
           "<dl>"
                "<dt><b>Jacky Alcine</b></dt><dd><a href=\"mailto:jacky.alcine@thesii.org\">jacky.alcine@thesii.org</a><br>"
                "Core Developer, Packager, Maintainer, Designer</dd>"
                "<dt><b>Adrian Borucki</b></dt><dd><a href=\"mailto:adrian@thesii.org\">adrian@thesii.org</a><br>"
                "Core Developer, Packager, Maintainer, Idea Man</dd>"
           "</dl><br>"
           "...and anybody else finding and reporting bugs, giving feedback, helping others and being part of the community!");

  return res;
}

QString AboutDialog::thanksTo() const {
  QString res;
  res = tr("Special thanks goes to:<br>"
           "<dl>"
           "<dt><b><a href=\"http://www.trolltech.com\">Qt Software formerly known as Trolltech</a></b></dt>"
              "<dd>for creating Qt and Qtopia</dd>"
           "<dt><a href=\"http://www.nokia.com\">Nokia</a></b></dt>"
              "<dd>for keeping Qt alive.</dd>"
          );

  return res;
}

#ifdef HAVE_KDE
#include "about-dialog.moc"
#endif