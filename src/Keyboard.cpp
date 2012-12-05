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



#include "Keyboard.h"
#include "Event.h"

namespace Habanero
{
	Keyboard::Keyboard() : enabled(true)
	{
	}

	void Keyboard::enable()
	{
		enabled = true;
	}

	void Keyboard::disable()
	{
		enabled = false;
	}

	void Keyboard::setKeyCallback(fastdelegate::FastDelegate2<KeyboardCodes::Key, bool> callback)
	{
		keyCallback = callback;
	}

	/*
	void Keyboard::setKeyCallback(nullptr_t)
	{
		keyCallback.clear();
	}
	 */

	void Keyboard::handleEvent(const Event &event)
	{
		if (!enabled)
			return;
		switch (event.Type)
		{
                    case Event::EventType::KeyPressed: handleKeyEvent(event.Data.Key, true); break;
                    case Event::EventType::KeyReleased: handleKeyEvent(event.Data.Key, false); break;
                    default: HASSERT(false);
		}
	}

	void Keyboard::handleKeyEvent(const Event::DataType::KeyEvent &event, bool pressed)
	{
		if (keyCallback)
			keyCallback(event.Code, pressed);
	}
}
