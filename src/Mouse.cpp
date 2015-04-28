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


#include "Enum.h"
#include "Window.h"
#include "Mouse.h"
#include <cstdio>
#include "assert.h"


#include <SFML/Window.hpp>

namespace Habanero
{
	Mouse::Mouse(Window *window) : window(window), relative(false)
	{
		enable();
	}

	void Mouse::enable()
	{
		if (relative)
		{
			window->sfmlWindow->setMouseCursorVisible(false);
			centre();
		}
		enabled = true;
	}

	void Mouse::disable()
	{
		if (relative)
			window->sfmlWindow->setMouseCursorVisible(true);
		enabled = false;
	}

	void Mouse::setRelativeMode(bool relative)
	{
		window->sfmlWindow->setMouseCursorVisible(!relative);
		if (relative)
			centre();
		this->relative = relative;
	}

	void Mouse::setMoveCallback(fastdelegate::FastDelegate2<int, int> callback)
	{
		moveCallback = callback;
	}

	void Mouse::setButtonCallback(fastdelegate::FastDelegate4<int, int, MouseCodes::Button, bool> callback)
	{
		buttonCallback = callback;
	}

	void Mouse::setWheelCallback(fastdelegate::FastDelegate1<int> callback)
	{
		wheelCallback = callback;
	}

	void Mouse::clearMoveCallback()
	{
		moveCallback.clear();
	}

	void Mouse::clearButtonCallback()
	{
		buttonCallback.clear();
	}

	void Mouse::clearWheelCallback()
	{
		wheelCallback.clear();
	}

	void Mouse::centre()
	{
        if (sf::Mouse::getPosition(*window->sfmlWindow) == sf::Vector2i(window->sfmlWindow->getSize().x / 2, window->sfmlWindow->getSize().y / 2))
            return;
        sf::Mouse::setPosition(sf::Vector2i(window->sfmlWindow->getSize().x / 2, window->sfmlWindow->getSize().y / 2), *window->sfmlWindow);
	}

	void Mouse::handleEvent(const Event &event)
	{
		if (!enabled)
			return;
		switch (event.Type)
		{
		case Event::EventType::MouseMoved: handleMouseMove(event.Data.MouseMove); break;
		case Event::EventType::MouseButtonPressed: handleMouseButton(event.Data.MouseButton, true); break;
		case Event::EventType::MouseButtonReleased: handleMouseButton(event.Data.MouseButton, false); break;
		case Event::EventType::MouseWheelMoved: handleMouseWheel(event.Data.MouseWheel); break;
		default: HASSERT(false);
		}
	}

	void Mouse::handleMouseMove(const Event::DataType::MouseMoveEvent &event)
	{
		int x = event.X, y = event.Y;
		fixPos(&x, &y);
		if (moveCallback && x != 0 && y != 0)
			moveCallback(x, y);
	}

	void Mouse::handleMouseButton(const Event::DataType::MouseButtonEvent &event, bool pressed)
	{
		int x = event.X, y = event.Y;
		fixPos(&x, &y);
		if (buttonCallback)
			buttonCallback(x, y, event.Button, pressed);
	}

	void Mouse::handleMouseWheel(const Event::DataType::MouseWheelEvent &event)
	{
		if (wheelCallback)
			wheelCallback(event.Delta);
	}

	void Mouse::fixPos(int *x, int *y)
	{
		if (!relative)
			return;
		*x = *x - int(window->sfmlWindow->getSize().x / 2);
		*y = *y - int(window->sfmlWindow->getSize().y / 2);
		centre();
	}
}
