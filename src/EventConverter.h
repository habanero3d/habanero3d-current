/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
This file is part of Habanero3d.

Habanero3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Habanero3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Habanero3d.  If not, see <http://www.gnu.org/licenses/>.


*/


/* 
 * File:   EventConverter.h
 * Author: akuda
 *
 * Created on 7 październik 2010, 23:33
 */

#pragma once

#include "Event.h"

namespace sf{class Event;}

namespace Habanero
{
    class EventConverter{
    public:
        static Event getEventFromSFML(sf::Event& event);
    private:
        static Event::EventType getEventTypeFromSFML(sf::Event &event);
        static MouseCodes::Button getMouseButtonFromSFML(sf::Event &event);
        static KeyboardCodes::Key getKeyCodeFromSFML(sf::Event &event);
    };
}

