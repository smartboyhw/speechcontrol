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

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <QObject>
#include "plugins.hpp"

namespace SpeechControl {
    namespace Plugins {
        class Factory;

        /**
         * @brief ...
         **/
        class Factory : public QObject
        {
            Q_OBJECT

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
             * @return void
             **/
            void pluginLoaded();
            /**
             * @brief ...
             *
             * @param  ...
             * @return void
             **/
            void pluginLoaded(const QString& );
            /**
             * @brief ...
             *
             * @return void
             **/
            void pluginUnloaded();
            /**
             * @brief ...
             *
             * @param  ...
             * @return void
             **/
            void pluginUnloaded(const QString& );

        public:
            virtual ~Factory();

            /**
             * @brief ...
             *
             * @param  ...
             * @return const bool
             **/
            static const bool loadPlugin(const QUuid& );
            /**
             * @brief ...
             *
             * @param  ...
             * @return const bool
             **/
            static const bool unloadPlugin(const QUuid& );
            /**
             * @brief ...
             *
             * @return :Plugins::PluginList
             **/
            static PluginList loadedPlugins();
            /**
             * @brief ...
             *
             * @return :Plugins::PluginList
             **/
            static PluginList availablePlugins();
            /**
             * @brief ...
             *
             * @return :Plugins::Factory*
             **/
            static Factory* instance();

        private:
            explicit Factory();
            static Factory* s_inst;
        };
    }
}

#endif // FACTORY_HPP
