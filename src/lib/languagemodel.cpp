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

#include <QDebug>
#include <QDirIterator>

#include "config.hpp"
#include "languagemodel.hpp"

using namespace SpeechControl;

LanguageModel::LanguageModel (QObject* p_parent) : QObject (p_parent), m_path()
{

}

LanguageModel* LanguageModel::fromDirectory (const QDir& p_directory)
{
    LanguageModel* lm = new LanguageModel;
    lm->m_path = p_directory.absolutePath();
    return lm;
}

QString LanguageModel::path() const
{
    return m_path;
}

QString LanguageModel::name() const
{
    QDir dir (path());
    return dir.dirName();
}

bool LanguageModel::isSystem() const
{
    return !isUser();
}

bool LanguageModel::isUser() const
{
    return m_path.contains (QDir::homePath());
}

/// @note This assumes that the LM file is the same name as the directory.
QStringList findAllLanguageModels (const QDir p_dir)
{
    QDirIterator itr (p_dir.absolutePath(), QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
    QStringList aList;

    while (itr.hasNext()) {
        const QString listing = itr.next();
        QDir listingDir (listing);
        qDebug() << "[findAllLanguageModels()] Found language model directory: " << listing;
        QFileInfo featParams (listing + "/" + listingDir.dirName() + ".lm");

        if (featParams.exists()) {
            aList << listing;
        }
        else continue;
    }

    qDebug() << "[findAllLanguageModels()] Removed" << aList.removeDuplicates() << "duplicates.";
    aList.removeAll (".");

    return aList;
}

LanguageModelList LanguageModel::allModels()
{
    // First, find the models imported by Sphinx. They're all stored under weird
    // folder names in MODELDIR.

    QDir baseModelDir (POCKETSPHINX_MODELDIR);
    baseModelDir.cd ("lm");
    QStringList dirs = findAllLanguageModels (baseModelDir);

    // Alright, we got the folders. Now, just build LanguageModel objects with it.
    LanguageModelList list;
    Q_FOREACH (const QString directory, dirs) {
        list << LanguageModel::fromDirectory (directory);
    }

    return list;
}

LanguageModel::~LanguageModel()
{

}

#include "languagemodel.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
