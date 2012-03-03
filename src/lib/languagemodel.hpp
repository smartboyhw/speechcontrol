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

#ifndef SPEECHCONTROL_LANGUAGEMODEL_HPP
#define SPEECHCONTROL_LANGUAGEMODEL_HPP

#include <QObject>
#include <QDir>
#include <QUuid>

namespace SpeechControl {

/**
 * @brief Represents a language model in its programmatic format.
 **/
class LanguageModel : public QObject {
    Q_OBJECT
    Q_PROPERTY ( QString Path READ path )   ///< The path to the language mode, be it a directory or file.

public:
    /**
     * @brief Null constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit LanguageModel ( QObject* p_parent = 0 );

    /**
     * @brief ...
     *
     * @param p_uuid ...
     **/
    LanguageModel ( const QUuid& p_uuid );
    /**
     * @brief ...
     *
     **/
    LanguageModel();

    /**
     * @brief ...
     *
     * @param p_directory ...
     * @return LanguageModel*
     **/
    static LanguageModel* fromDirectory ( const QDir& p_directory );

    /**
     * @brief ...
     *
     * @param p_archiveFile ...
     * @return LanguageModel*
     **/
    static LanguageModel* fromCompressedFile ( const QFile* p_archiveFile );

    /**
     * @brief ...
     *
     * @param p_path ...
     * @return LanguageModel*
     **/
    static LanguageModel* fromPath(const QString& p_path);
    /**
     * @brief ...
     *
     * @return QDir
     **/
    QString path() const;
    /**
     * @brief ...
     *
     **/
    virtual ~LanguageModel();
};

}

#endif // SPEECHCONTROL_LANGUAGEMODEL_HPP
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
