/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef AGENT_HPP
#define AGENT_HPP

#include <QObject>
#include <sphinx.hpp>

/**
 * @brief Namespace containing all of SpeechControl's definitions.
 **/
namespace SpeechControl {
    /**
     * @brief Namespace containing the Desktop Control utilities.
     **/
    namespace DesktopControl {
        class Agent;

        /**
         * @brief ...
         **/
        class Agent : public QObject
        {
            Q_OBJECT
            Q_DISABLE_COPY(Agent)

        signals:
            /**
             * @brief ...
             *
             * @return void
             **/
            void started();
            /**
             * @brief ...
             *
             * @return void
             **/
            void stopped();
            /**
             * @brief ...
             *
             * @param  ...
             * @return void
             **/
            void stateChanged(const bool);

        public:
            /**
             * @brief ...
             *
             * @return void
             **/
            explicit Agent(QObject* = 0);
            /**
             * @brief ...
             *
             **/
            virtual ~Agent();

            /**
             * @brief ...
             *
             * @return void
             **/
            static void start();
            /**
             * @brief ...
             *
             * @return void
             **/
            static void stop();
            /**
             * @brief ...
             *
             * @param  ...
             * @return void
             **/
            static void invokeCommand(const QString&);
            /**
             * @brief ...
             *
             * @return :DesktopControl::Agent*
             **/
            static Agent* instance();
            /**
             * @brief ...
             *
             * @return void
             **/
            static const bool isActive();

        private:
            static Agent* s_inst;
            Sphinx* m_sphnx;
        };
    }
}

#endif // AGENT_H
