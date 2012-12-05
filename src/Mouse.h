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


#pragma once

#include "FastDelegate.h"
#include "Event.h"

namespace Habanero
{
	class Window;

	class Mouse
	{
		friend class Window;
	private:
		Mouse(const Mouse&);
		void operator =(const Mouse&);

		fastdelegate::FastDelegate2<int, int> moveCallback;
		fastdelegate::FastDelegate4<int, int, MouseCodes::Button, bool> buttonCallback;
		fastdelegate::FastDelegate1<int> wheelCallback;
		Window *window;
		bool enabled, relative;

		Mouse(Window *window);
		void centre();
		void fixPos(int *x, int *y);

		void handleEvent(const Event &event);
		void handleMouseMove(const Event::DataType::MouseMoveEvent &event);
		void handleMouseButton(const Event::DataType::MouseButtonEvent &event, bool pressed);
		void handleMouseWheel(const Event::DataType::MouseWheelEvent &event);
	public:
		void enable();
		void disable();
		void setRelativeMode(bool relative);
		void setMoveCallback(fastdelegate::FastDelegate2<int, int> callback);
		void setButtonCallback(fastdelegate::FastDelegate4<int, int, MouseCodes::Button, bool> callback);
		void setWheelCallback(fastdelegate::FastDelegate1<int> callback);
		void clearMoveCallback();
		void clearButtonCallback();
		void clearWheelCallback();
	};
};
