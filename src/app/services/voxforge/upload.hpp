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
 *  License along with SpeechControl .
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/20/12 14:53:37 PM
 */

#ifndef UPLOADHANDLER_HPP
#define UPLOADHANDLER_HPP

#include <QList>
#include <QObject>

namespace SpeechControl
{
namespace Voxforge
{

class Package;

class UploadHandler : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Undefined = -1,
        Connecting,
        Authenticating,
        Authenticated,
        Connected,
        Uploading,
        Uploaded
    };

    explicit UploadHandler (QObject* parent = 0);
    virtual ~UploadHandler();
    void addPackage (Package* p_package);
    void setUsername (const QString& p_username);
    void setPassword (const QString& p_password);
    QList<Package*> packages() const;

public slots:
    void upload();
    void cancelUpload();

signals:
    void stateChanged(const Status& p_status);
    void uploadProgressChanged(const double& p_progress);

private:
    QList<Package*> m_pckgs;
    QString m_usrName;
    QString m_pssWord;
};
}
}

#endif
// kate: indent-mode cstyle; replace-tabs on;
