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

#include <QDir>
#include <QUuid>
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QDirIterator>

#include <algorithm>

#include "global.hpp"
#include "noisedictionary.hpp"
#include "acousticmodelprivate.hpp"
#include "acousticmodel.hpp"

using namespace std;
using SpeechControl::AcousticModel;
using SpeechControl::AcousticModelPrivate;
using SpeechControl::NoiseDictionary;
using SpeechControl::AcousticModelList;

void cloneDirectory (QDir p_base, QDir p_newDir)
{
    QStringList entries = p_base.entryList (QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    Q_FOREACH (const QString entry, entries) {
        QFileInfo entryInfo (entry);

        if (entryInfo.isDir()) {
            p_base.mkpath (p_newDir.absolutePath() + "/" + entryInfo.baseName());
            qDebug() << "[cloneDirectory()] Descending into " << entryInfo.absolutePath();
            cloneDirectory (QDir (entryInfo.absolutePath()), QDir (p_newDir.absolutePath() + "/" + entryInfo.baseName()));
        }
        else {
            QString fileBase = p_base.absoluteFilePath (entry);
            QString fileNew = p_newDir.absoluteFilePath (entryInfo.fileName());
            qDebug() << "[cloneDirectory()] Copying" << fileBase << "to" << fileNew << "..";
            QFile::copy (fileBase, fileNew);
        }
    }
}

AcousticModelPrivate::AcousticModelPrivate (const AcousticModel* p_model)
    : m_params(), m_path(),
      m_noisedict (0), m_mdl (const_cast<AcousticModel*> (p_model))
{

}

void AcousticModelPrivate::loadFeatureParameters()
{
    QFile* file = new QFile (m_mdl->parameterPath());
    file->open (QIODevice::ReadOnly | QIODevice::Text);

    QTextStream strm (file);

    while (!strm.atEnd()) {
        const QStringList tokens = strm.readLine().split (" ");
        QString paramName = tokens[0];

        if (paramName.startsWith ("-"))
            paramName = paramName.remove (0, 1);

        QString paramValue = tokens[1];
        qDebug() << "[AcousticModel::loadFeatureParameters()] Parsing parameter" << paramName << "=" << paramValue;
        m_mdl->setParameter (paramName, paramValue);
    }

    file->close();
}

void AcousticModelPrivate::loadNoiseDictionary()
{
    QFile* noiseDictFile = new QFile (m_path +  "/noisedict");
    m_noisedict = NoiseDictionary::fromFile (noiseDictFile);
}


AcousticModel::AcousticModel (const AcousticModel& p_other) :
    QObject (p_other.parent()), d_ptr (const_cast<AcousticModelPrivate*> (p_other.d_ptr.data()))
{
}

AcousticModel::AcousticModel (const QString& p_path, QObject* p_parent) : QObject (p_parent),
    d_ptr (new AcousticModelPrivate (this))
{
    load (p_path);
}

AcousticModel::AcousticModel (QObject* p_parent) : QObject (p_parent), d_ptr (new AcousticModelPrivate (this))
{

}

AcousticModel::~AcousticModel()
{
}

void AcousticModel::load (QString p_path)
{
    QDir dir (p_path);

    if (!dir.exists())
        return;

    d_func()->m_path = p_path;

    d_func()->loadFeatureParameters();
    d_func()->loadNoiseDictionary();
}

QFile* AcousticModel::modelDefinitions() const
{
    return new QFile (path() + "/mdef");
}

QFile* AcousticModel::mixtureWeights()
{
    return new QFile (path() + "/mixture_weights");
}

QFile* AcousticModel::transitionMatrices()
{
    return new QFile (path() + "/transition_matrices");
}

QFile* AcousticModel::variances()
{
    return new QFile (path() + "/variances");
}

QFile* AcousticModel::means()
{
    return new QFile (path() + "/means");
}

QFile* AcousticModel::senDump()
{
    return new QFile (path() + "/sendump");
}

QString AcousticModel::parameterPath() const
{
    return path() + "/feat.params";
}

void AcousticModel::setParameter (const QString& p_key, const QVariant& p_value)
{
    d_func()->m_params.insert (p_key, p_value);
}

void AcousticModel::setParameters (QVariantMap const& p_params)
{
    d_func()->m_params = p_params;
}

void AcousticModel::mergeParameters (QVariantMap const& p_params)
{
    for (QMap< QString, QVariant >::const_iterator it = p_params.constBegin();
            it != p_params.constEnd(); ++it) {

        d_func()->m_params.insert (it.key(), it.value());
    }
}

QVariant AcousticModel::parameter (const QString& p_key) const
{
    return d_func()->m_params.value (p_key);
}

QVariantMap AcousticModel::parameters() const
{
    return d_func()->m_params;
}

quint16 AcousticModel::sampleRate() const
{
    return 16000;
}

QString AcousticModel::name() const
{
    QDir dir (path());
    return dir.dirName();
}

bool AcousticModel::isSystem() const
{
    return !isUser();
}

bool AcousticModel::isUser() const
{
    return path().contains (QDir::homePath());
}

QString AcousticModel::path() const
{
    return d_func()->m_path;
}

bool AcousticModel::isValid() const
{
    return (QDir (path())).exists();
}

NoiseDictionary* AcousticModel::noiseDictionary() const
{
    return d_func()->m_noisedict;
}

AcousticModel* AcousticModel::clone()
{
    // obtain directory info.
    QDir model (path());
    QDir newDir (QDir::homePath() + "/.config/speechcontrol/models");
    newDir.mkpath (newDir.absolutePath());

    QString randomID = QString::number (qrand());
    const QString newModelName = model.dirName() + "-" + randomID;
    newDir.mkdir (newModelName);
    newDir.cd (newModelName);

    // create directory.
    cloneDirectory (model, newDir);

    qDebug() << "[AcousticModel::clone()] Cloned" << path() << "to" << newDir;
    return new AcousticModel (newDir.path());
}

QStringList findAllAcousticModels (const QDir p_dir)
{
    QDirIterator itr (p_dir.absolutePath(), QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
    QStringList aList;

    while (itr.hasNext()) {
        const QString listing = itr.next();
        QFileInfo featParams (listing + "/feat.params");

        if (featParams.exists()) {
            aList << listing;
        }

        else continue;
    }

    qDebug() << "[findAllAcousticModels()] Removed" << aList.removeDuplicates() << "duplicates.";
    aList.removeAll (".");

    return aList;
}

/// @todo This should find the models installed automagically installed by the user's package management system.
AcousticModelList AcousticModel::allModels()
{
    // First, find the models imported by Sphinx. They're all stored under weird
    // folder names in MODELDIR.

    QDir systemModelDir (POCKETSPHINX_MODELDIR);
    QDir userModelDir (QDir::homePath() + "/.config/speechcontrol/models");
    systemModelDir.cd ("hmm");

    QStringList dirs = findAllAcousticModels (systemModelDir);
    dirs.append (findAllAcousticModels (userModelDir));

    // Alright, we got the folders. Now, just build AcousticModel objects with it.
    AcousticModelList list;
    Q_FOREACH (const QString directory, dirs) {
        list << new AcousticModel (directory);
    }

    return list;
}

void AcousticModel::erase()
{
    if (isUser()) {
        QDir dir (path());
        dir.rmpath (".");
    }
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
#include "acousticmodel.moc"
