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


#include "languagemodel.hpp"

using namespace SpeechControl;

LanguageModel::LanguageModel (QObject* p_parent) : QObject (p_parent)
{

}

LanguageModel::LanguageModel (const QUuid& p_uuid) : QObject(), m_uuid (p_uuid)
{

}

LanguageModel::LanguageModel()
{

}

LanguageModel* LanguageModel::fromCompressedFile (const QFile* p_archiveFile)
{
    return 0;
}

LanguageModel* LanguageModel::fromDirectory (const QDir& p_directory)
{
    return 0;
}

LanguageModel* LanguageModel::fromPath (const QString& p_path)
{
    return 0;
}

QString LanguageModel::path() const
{
    return QString::null;
}

LanguageModel::~LanguageModel()
{

}

#include "languagemodel.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
