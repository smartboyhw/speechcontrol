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
 *  License along with SpeechControl.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/17/12 18:48:00 PM
 */

#ifndef SPCHCNTRL_LANGUAGEMODEL_HPP
#define SPCHCNTRL_LANGUAGEMODEL_HPP

#include <QDir>
#include <QList>
#include <QObject>

#include <lib/global.hpp>

SPCHCNTRL_BEGIN_NAMESPACE

class LanguageModel;
struct LanguageModelPrivate;

typedef QList<LanguageModel*> LanguageModelList;
/**
 * @brief Represents a language model in its programmatic format.
 **/
class LanguageModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString Path READ path)     ///< The path to the language mode, be it a directory or file.
    Q_DECLARE_PRIVATE (LanguageModel)

private:
    QScopedPointer<LanguageModelPrivate> d_ptr;

public:
    /**
     * @brief Null constructor.
     * @param p_parent Defaults to 0.
     **/
    explicit LanguageModel (QObject* p_parent = 0);

    /**
     * @brief Destructor.
     **/
    virtual ~LanguageModel();

    /**
     * @brief Obtains a language model from a directory.
     *
     * @param p_directory Path to the language model.
     **/
    static LanguageModel* fromDirectory (const QDir& p_directory);

    /**
     * @brief Obtains all of the known language models.
     **/
    static LanguageModelList allModels();

    /**
     * @brief Obtains the directory that holds this language model.
     **/
    QString path() const;

    /**
     * @brief Determines if this model is a system language model.
     **/
    bool isSystem() const;

    /**
     * @brief Determines if this model is a user language model.
     **/
    bool isUser() const;

    /**
     * @brief Obtains the name of this language model.
     * @note This value is not human-friendly.
     **/
    QString name() const;

    /**
     * @brief Erases this LanguageModel.
     **/
    void erase();
};

SPCHCNTRL_END_NAMESPACE

#endif // SPEECHCONTROL_LANGUAGEMODEL_HPP
// kate: indent-mode cstyle; replace-tabs on; 
