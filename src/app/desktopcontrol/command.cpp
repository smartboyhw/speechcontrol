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

#include <algorithm>
#include <QDebug>
#include "command.hpp"

using namespace std;
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
        if ( s_inst == 0 )
            s_inst = new GlobalCategory;

        return s_inst;
    }

    explicit GlobalCategory ( ) : AbstractCategory ( 0 ) {
    }
};

GlobalCategory* GlobalCategory::s_inst = 0;
QMap<QString,AbstractCategory*> AbstractCategory::s_ctgrs;

AbstractCommand::AbstractCommand ( AbstractCategory* p_parentCategory, QStringList p_commands ) : QObject ( p_parentCategory ),
    m_commands ( p_commands ) {
    qDebug() << "New commands!" << p_commands;
}

/// @todo Instead of returning a boolean value, we should have a comparison value on a scale of 0.0 to 1.0, where 0.0 = not equal and 1.0 = fully equal.
bool AbstractCommand::areStatementsEquivalent ( const QString p_command, const QString p_statement ) const {
    return p_statement.contains ( p_command );
}

/// @todo When @c areStatementsEquivalent is updated, this should return an average.
bool AbstractCommand::isValidStatement ( const QString& p_statement ) const {
    Q_FOREACH ( const QString l_statement, m_commands ) {
        if ( AbstractCommand::areStatementsEquivalent ( l_statement,p_statement ) )
            return true;
    }
    return false;
}

QString AbstractCommand::santizeStatement ( const QString p_statement ) const {
    Q_FOREACH ( const QString l_command, m_commands ) {
        if ( p_statement.contains ( l_command ) ) {
            return QString ( p_statement ).remove ( l_command );
        }
    }

    return p_statement;
}

const QStringList AbstractCommand::statements() const {
    return m_commands;
}

AbstractCommand::~AbstractCommand() {

}

AbstractCategory::AbstractCategory ( AbstractCategory* p_parentCategory ) : QObject ( p_parentCategory ) {
}

/// @note The list of Commands represented here are only done within this AbstractCategory; it's not recursive.
/// @bug There's a lack of a recursive nature to this method.
/// @todo Implement a means of picking out child categories and have them return their commands upward.
CommandList AbstractCategory::commands() {
    return m_lst;
}

CommandList AbstractCategory::matchCommands ( const QString& p_command ) {
    CommandList l_lst;

    Q_FOREACH ( QObject* l_child, children() ) {
        AbstractCategory* l_category = qobject_cast<AbstractCategory*> ( l_child );
        l_lst << l_category->matchCommands ( p_command );
    }

    Q_FOREACH ( AbstractCommand* l_cmd, commands() ) {
        if ( l_cmd->isValidStatement ( p_command ) )
            l_lst << l_cmd;
    }

    unique ( l_lst.begin(),l_lst.end() );
    return l_lst;
}


CategoryList AbstractCategory::categories() {
    return s_ctgrs.values();
}

CommandList AbstractCategory::matchAllCommands ( const QString& p_command ) {
    CommandList l_lst;

    Q_FOREACH ( AbstractCategory* l_ctgry, categories() ) {
        l_lst << l_ctgry->matchCommands ( p_command );
    }

    unique ( l_lst.begin(),l_lst.end() );
    return l_lst;
}

AbstractCategory* AbstractCategory::global() {
    return GlobalCategory::instance();
}

AbstractCategory::~AbstractCategory() {

}

#include "command.moc"
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
