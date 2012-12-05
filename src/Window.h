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
#include <string>

#include "FastDelegate.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Event.h"
#include "IRenderTarget.h"

namespace sf
{
	class Window;
	class Clock;
}

#ifdef WIN32
struct HWND__;
#endif

namespace Habanero
{

	class Mouse;
	class Keyboard;

	class Window : public IRenderTarget
	{
		friend class Mouse;
		friend class Keyboard;
	private:
		Window(const Window&);
		void operator =(const Window&);

		sf::Window * sfmlWindow;
		sf::Clock *sfmlClock;
		Keyboard keyboard;
		Mouse mouse;
		bool pendingClose;

		void create(const char *name, uint width, uint height, bool fullscreen);
		void create2();
		void handleEvent(const Event &event);
	public:
#ifndef WIN32
		typedef unsigned long Handle;
#else
		typedef HWND__* Handle;
#endif

		Window(Handle handle);
		Window(const char *name, uint width, uint height, bool fullscreen = false);
		Window(const std::string &name, uint width, uint height, bool fullscreen = false);
		~Window();

		bool processEvents();
		Mouse& getMouse();
		Keyboard& getKeyboard();
		void setActive(bool active = true);

		/* miedzy innymi ustawia kontekst opengla */
		void update();
		void close();
		void postClose();
		float getTimeSinceLastUpdate() const;

		//IRenderTarget:
		uint getHeight() const;
		uint getWidth() const;
		uint getNumberOfBits() const;
		uint getSizeInPixels() const;
		
		bool bindForDrawing();
		void unbindForDrawing();

	};
};