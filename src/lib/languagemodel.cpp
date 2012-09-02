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

#include "global.hpp"
#include "languagemodelprivate.hpp"
#include "languagemodel.hpp"

using namespace SpeechControl;

LanguageModel::LanguageModel (QObject* p_parent) : QObject (p_parent), d_ptr (new LanguageModelPrivate)
{

}

LanguageModel* LanguageModel::fromDirectory (const QDir& p_directory)
{
    LanguageModel* lm = new LanguageModel;
    lm->d_func()->m_path = p_directory.absolutePath();
    return lm;
}

QString LanguageModel::path() const
{
    return d_func()->m_path;
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
    return path().contains (QDir::homePath());
}

QStringList findLanguageModels (const QDir p_dir)
{
    QDirIterator itr (p_dir.absolutePath(), QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
    QStringList modelsList;

    while (itr.hasNext()) {
        QString listing = itr.next();
        QDir listingDir (listing);

        qDebug() << "[LanguageModel] Found language model directory: " << listing;
        qDebug() << "   Searching for .DMP files.";

        QStringList modelFilters;
        modelFilters << "*.DMP" << "*.dmp" << "*.lm";
        QStringList models = listingDir.entryList(modelFilters);

        qDebug() << "   Found" << models.size() << "models.";

        for (QStringList::const_iterator mName = models.constBegin();
             mName != models.constEnd(); ++mName)
        {
            modelsList.append(listing + '/' + (*mName));
        }
    }

    qDebug() << "   Removed" << modelsList.removeDuplicates() << "duplicates.";
    return modelsList;
}

LanguageModelList LanguageModel::allModels()
{
    // First, find the models imported by Sphinx. They're all stored under weird
    // folder names in MODELDIR.

    QDir baseModelDir (POCKETSPHINX_MODELDIR);
    baseModelDir.cd ("lm");
    QStringList dirs = findLanguageModels (baseModelDir);

    // Alright, we got the folders. Now, just build LanguageModel objects with it.
    LanguageModelList list;
    Q_FOREACH (const QString directory, dirs) {
        list << LanguageModel::fromDirectory (directory);
    }

    return list;
}

void LanguageModel::erase()
{
    if (isUser()) {
        QDir dir (path());
        dir.rmpath (".");
    }
}

LanguageModel::~LanguageModel()
{
}

#include "languagemodel.moc"
// kate: indent-mode cstyle; replace-tabs on; 
