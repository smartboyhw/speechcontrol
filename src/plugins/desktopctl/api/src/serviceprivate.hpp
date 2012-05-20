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

#include <app/services/moduleprivate.hpp>

class QDeclarativeView;
namespace SpeechControl
{
namespace DesktopControl
{

class Service;
class Sphinx;

struct ServicePrivate : public Services::AbstractModulePrivate {
    virtual ~ServicePrivate();
    explicit ServicePrivate (Service* p_parent = 0);
    ServicePrivate (const Services::AbstractModulePrivate& p_other);
    virtual void changeState (AbstractServiceModule::ActivityState p_state);
    virtual Services::AbstractServiceModule::ActivityState handleStateChange (const Services::AbstractServiceModule::ActivityState p_state);
    Sphinx* m_sphinx;
    QDeclarativeView* m_view;
};

}
}
// kate: indent-mode cstyle; replace-tabs on; 

