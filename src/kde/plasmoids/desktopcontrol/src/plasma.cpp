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
 *  along with SpeechControl.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

// Qt includes
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>

// KDE includes
#include <KLocale>

// Plasma includes
#include <Plasma/Svg>
#include <Plasma/Theme>
#include <Plasma/Plasma>

#include "plasma.hpp"

DesktopControlPlasmoid::DesktopControlPlasmoid (QObject* parent, const QVariantList& args)
    : Plasma::Applet (parent, args),
      m_svg (this),
      m_icon ("document")
{
    setBackgroundHints (DefaultBackground);
    m_svg.setImagePath ("widgets/background");
    setHasConfigurationInterface (true);
    resize (200, 200);
}


DesktopControlPlasmoid::~DesktopControlPlasmoid()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    }
    else {
        // Save settings
    }
}

void DesktopControlPlasmoid::init()
{
    // A small demonstration of the setFailedToLaunch function
    if (m_icon.isNull()) {
        setFailedToLaunch (true, i18n ("No world to say hello"));
    }
}


void DesktopControlPlasmoid::paintInterface (QPainter* p,
        const QStyleOptionGraphicsItem* option, const QRect& contentsRect)
{
    p->setRenderHint (QPainter::SmoothPixmapTransform);
    p->setRenderHint (QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    m_svg.resize ( (int) contentsRect.width(), (int) contentsRect.height());
    m_svg.paint (p, (int) contentsRect.left(), (int) contentsRect.top());

    // We place the icon and text on our background
    p->drawPixmap (25, 40, m_icon.pixmap ( (int) contentsRect.width(), (int) contentsRect.width() - 50));
    p->save();
    p->setPen (Qt::white);
    p->setFont (QFont ("Times", 15, QFont::Bold));
    p->drawText (contentsRect,
                 Qt::AlignBottom | Qt::AlignHCenter,
                 i18n ("Hello Plasmoid!"));
    p->restore();
}

#include "plasma.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
