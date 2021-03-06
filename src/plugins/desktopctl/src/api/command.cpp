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
 * @date 05/22/12 19:35:30 PM
 */

#include <algorithm>

#include <QDebug>

#include "core.hpp"
#include "command.hpp"

using namespace std;
using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

struct GlobalCategory : public AbstractCategory {
    static GlobalCategory* s_inst;

    virtual const QString id() const {
        return "global";
    }

    virtual const QString title() {
        return "Global";
    }

    static AbstractCategory* instance() {
        if (s_inst == 0) {
            s_inst = new GlobalCategory;
        }

        return s_inst;
    }

    explicit GlobalCategory () : AbstractCategory() {
    }
};

GlobalCategory* GlobalCategory::s_inst = 0;
QMap<QString, AbstractCategory*> AbstractCategory::s_ctgrs;

AbstractCommand::AbstractCommand (AbstractCategory* p_parentCategory, QStringList p_commands) : QObject (p_parentCategory),
    m_commands (p_commands)
{
    qDebug() << "[AbstractCommand::{constructor}] Discovered commands" << p_commands;
}

bool AbstractCommand::areStatementsEquivalent (const QString p_command, const QString p_statement) const
{
    qDebug() << "[AbstractCommand::areStatementsEquivalent()]"
             << p_statement << "~=" << p_command
             << p_statement.toLower().startsWith (p_command.toLower());
    /*
     * If we were going to use a regular expression, this would have been appropriate.
     * ^\b(p_command)\b{1,1}
     */
    return p_statement.toLower().startsWith (p_command.toLower());
}

bool AbstractCommand::isValidStatement (const QString& p_statement) const
{
    qDebug() << "[AbstractCommand::isValidStatement()] Determing validity of statement" << p_statement;
    Q_FOREACH (const QString command, m_commands) {
        if (AbstractCommand::areStatementsEquivalent (command.toLower(), p_statement.toLower())) {
            qDebug() << "[AbstractCommand::isValidStatement()] Statement valid for " << command;
            return true;
        }
    }

    qDebug() << "[AbstractCommand::isValidStatement()] Statement is invalid.";
    return false;
}

QString AbstractCommand::obtainArgumentFromStatement (const QString p_statement) const
{
    Q_FOREACH (const QString command, m_commands) {
        if (p_statement.contains (command)) {
            return QString (p_statement).remove (command).trimmed().simplified();
        }
    }

    return p_statement;
}

QString AbstractCommand::obtainCommandFromStatement (const QString p_statement) const
{
    const QString argument = obtainArgumentFromStatement (p_statement);
    return QString (p_statement).remove (argument).trimmed().simplified();
}

const QStringList AbstractCommand::statements() const
{
    return m_commands;
}

void AbstractCategory::addCommand (AbstractCommand* p_command)
{
    if (!hasCommand (p_command)) {
        m_map.insert (p_command->id(), p_command);
        qDebug() << "[AbstractCategory::addCommand()] Added command" << p_command->id() << "to category" << id();
    }
}

bool AbstractCategory::hasCommand (AbstractCommand* p_command)
{
    return hasCommand (p_command->id());
}

bool AbstractCategory::hasCommand (const QString& p_id)
{
    qDebug() << "[AbstractCategory::hasCommand()] " << id() << "has" << p_id << "?" << m_map.contains (p_id);
    return m_map.contains (p_id);
}

void AbstractCategory::removeCommand (AbstractCommand* p_command)
{
    removeCommand (p_command->id());
}

void AbstractCategory::removeCommand (const QString& p_id)
{
    if (hasCommand (p_id)) {
        m_map.remove (p_id);
    }
}

AbstractCommand::~AbstractCommand()
{

}

AbstractCategory::AbstractCategory (AbstractCategory* p_parentCategory) : QObject (p_parentCategory)
{
    s_ctgrs.insert (QString::null, this);
}

AbstractCategory::AbstractCategory() : QObject (Core::instance())
{
    s_ctgrs.insert (QString::null, this);
}

/// @note The list of Commands represented here are only done within this AbstractCategory; it's not recursive.
/// @bug There's a lack of a recursive nature to this method.
/// @todo Implement a means of picking out child categories and have them return their commands upward.
CommandList AbstractCategory::commands()
{
    CommandList l_lst;

    l_lst.append (m_map.values());

    Q_FOREACH (QObject * l_child, children()) {
        if (l_child->metaObject()->superClass()->className() == AbstractCategory::staticMetaObject.className()) {
            AbstractCategory* l_category = qobject_cast<AbstractCategory*> (l_child);
            l_lst << l_category->commands();
        }
    }

    unique (l_lst.begin(), l_lst.end());
    return l_lst;
}

CommandList AbstractCategory::matchCommands (const QString& p_command)
{
    CommandList l_lst;

    Q_FOREACH (AbstractCommand * l_cmd, commands()) {
        if (l_cmd->isValidStatement (p_command)) {
            l_lst << l_cmd;
        }
    }

    return l_lst;
}


CategoryList AbstractCategory::categories()
{
    return s_ctgrs.values();
}

CommandList AbstractCategory::matchAllCommands (const QString& p_command)
{
    return AbstractCategory::global()->matchCommands (p_command);
}

AbstractCategory* AbstractCategory::global()
{
    return GlobalCategory::instance();
}

AbstractCategory::~AbstractCategory()
{
}

#include "command.moc"
// kate: indent-mode cstyle; replace-tabs on;
