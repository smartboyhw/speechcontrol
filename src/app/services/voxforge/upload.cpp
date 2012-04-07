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

#include "upload.hpp"

using namespace SpeechControl::Voxforge;

UploadHandler::UploadHandler (QObject* parent) : QObject (parent)
{

}

void UploadHandler::addPackage (Package* p_package)
{
    if (!m_pckgs.contains (p_package)) {
        m_pckgs.append (p_package);
    }
}

QList< Package* > UploadHandler::packages() const
{
    return m_pckgs;
}

void UploadHandler::setPassword (const QString& p_password)
{
    if (!p_password.isEmpty() && !p_password.isNull()) {
        m_pssWord = p_password;
    }
}

void UploadHandler::setUsername (const QString& p_username)
{
    if (!p_username.isEmpty() && !p_username.isNull()){
        m_usrName = p_username;
    }
}

void UploadHandler::cancelUpload()
{

}

void UploadHandler::upload()
{

}

UploadHandler::~UploadHandler()
{

}

#include "services/voxforge/upload.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
