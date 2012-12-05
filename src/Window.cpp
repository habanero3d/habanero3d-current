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


#include <SFML/Window/Event.hpp>
#include <SFML/Window.hpp>

#include "Window.h"

#include "EventConverter.h"

namespace Habanero
{
	//#pragma warning(suppress:4355)

	Window::Window(Window::Handle handle) :
	IRenderTarget(RenderTargetType::Window),
	mouse(this)
	{
		sfmlWindow = new sf::Window(handle);
		sfmlClock = new sf::Clock();
		create2();
	}

	//#pragma warning(suppress:4355)

	Window::Window(const char *name, uint width, uint height, bool fullscreen) :
	IRenderTarget(RenderTargetType::Window),
	mouse(this)
	{
		sfmlWindow = new sf::Window();
		sfmlClock = new sf::Clock();
		create(name, width, height, fullscreen);
	}

	//#pragma warning(suppress:4355)

	Window::Window(const std::string &name, uint width, uint height, bool fullscreen) :
	IRenderTarget(RenderTargetType::Window),
	mouse(this)
	{
		sfmlWindow = new sf::Window();
		sfmlClock = new sf::Clock();
		create(name.c_str(), width, height, fullscreen);
	}

	Window::~Window()
	{
		delete sfmlWindow;
		delete sfmlClock;
	}

	void Window::create(const char *name, uint width, uint height, bool fullscreen)
	{
		sfmlWindow->create(sf::VideoMode(width, height), name, fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
		create2();
	}

	void Window::create2()
	{
		pendingClose = false;
		sfmlWindow->setKeyRepeatEnabled(false);
		//sfmlWindow->enableVerticalSync(true);
	}

	Mouse& Window::getMouse()
	{
		return mouse;
	}

	Keyboard& Window::getKeyboard()
	{
		return keyboard;
	}

	void Window::setActive(bool active)
	{
		sfmlWindow->setActive(active);
	}

	bool Window::processEvents()
	{
		sf::Event event;
		bool cont = true;
		while (sfmlWindow->pollEvent(event))
			if (event.type == sf::Event::Closed)
				cont = false;
			else
				handleEvent(EventConverter::getEventFromSFML(event));
		return cont && !pendingClose;
	}

	void Window::handleEvent(const Event &event)
	{
		switch (event.Type)
		{
			case Event::EventType::MouseWheelMoved:
			case Event::EventType::MouseButtonPressed:
			case Event::EventType::MouseButtonReleased:
			case Event::EventType::MouseMoved:
				mouse.handleEvent(event);
				break;
			case Event::EventType::KeyPressed:
			case Event::EventType::KeyReleased:
				keyboard.handleEvent(event);
				break;
			case Event::EventType::LostFocus:
				mouse.disable();
				keyboard.disable();
				break;
			case Event::EventType::GainedFocus:
				mouse.enable();
				keyboard.enable();
				break;
		}
	}

	void Window::update()
	{
		sfmlWindow->display();
	}

	void Window::close()
	{
		sfmlWindow->close();
	}

	void Window::postClose()
	{
		pendingClose = true;
	}

	float Window::getTimeSinceLastUpdate() const
	{
		float ret = sfmlClock->getElapsedTime().asMilliseconds();
		sfmlClock->restart();
		return ret;
	}

	uint Window::getHeight() const
	{
		return sfmlWindow->getSize().y;
	}
	
	uint Window::getWidth() const
	{
		return sfmlWindow->getSize().x;
	}
	
	uint Window::getNumberOfBits() const
	{
		//TODO: is it right?
		return 32u; 
	}
	
	uint Window::getSizeInPixels() const
	{
		return sfmlWindow->getSize().x * sfmlWindow->getSize().y;
	}
	
	bool Window::bindForDrawing()
	{
		sfmlWindow->setActive(true);
	}
	
	void Window::unbindForDrawing()
	{
		sfmlWindow->setActive(false);
	}
}
