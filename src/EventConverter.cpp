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
 * File:   EventConverter.cpp
 * Author: akuda
 * 
 * Created on 7 październik 2010, 23:33
 */

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "EventConverter.h"

/*
 * są tutaj 2 bardzo podobne switche po event.Type. Bardzo proszę ich nie łączyć.
 *  - akuda
 */

namespace Habanero
{
    Event EventConverter::getEventFromSFML(sf::Event& event)
    {
        Event result;
        result.Type = getEventTypeFromSFML(event);

        switch(result.Type)
        {
            case Event::EventType::MouseWheelMoved:
                result.Data.MouseWheel.Delta = event.mouseWheel.delta;
                break;

            case Event::EventType::MouseButtonPressed:
                result.Data.MouseButton.Button = getMouseButtonFromSFML(event);
                result.Data.MouseButton.X = event.mouseButton.x;
                result.Data.MouseButton.Y = event.mouseButton.y;
                break;

            case Event::EventType::MouseButtonReleased:
                result.Data.MouseButton.Button = getMouseButtonFromSFML(event);
                result.Data.MouseButton.X = event.mouseButton.x;
                result.Data.MouseButton.Y = event.mouseButton.y;
                break;

            case Event::EventType::MouseMoved:
                result.Data.MouseMove.X = event.mouseMove.x;
                result.Data.MouseMove.Y = event.mouseMove.y;
                break;

            case Event::EventType::KeyPressed:
                result.Data.Key.Code = getKeyCodeFromSFML(event);
                break;

            case Event::EventType::KeyReleased:
                result.Data.Key.Code = getKeyCodeFromSFML(event);
                break;


            // TODO: add this dziwadła:
            case Event::EventType::LostFocus:
                break;
            case Event::EventType::GainedFocus:
                break;
            default:
                break;
        }

        return result;
    }

    Event::EventType EventConverter::getEventTypeFromSFML(sf::Event &event)
    {
        switch(event.type)
        {
            case sf::Event::EventType::MouseWheelMoved:
                return Event::EventType::MouseWheelMoved;
            case sf::Event::EventType::MouseButtonPressed:
                    return Event::EventType::MouseButtonPressed;
            case sf::Event::EventType::MouseButtonReleased:
                return Event::EventType::MouseButtonReleased;
            case sf::Event::EventType::MouseMoved:
                return Event::EventType::MouseMoved;
            case sf::Event::EventType::KeyPressed:
                return Event::EventType::KeyPressed;
            case sf::Event::EventType::KeyReleased:
                return Event::EventType::KeyReleased;
            case sf::Event::EventType::LostFocus:
                return Event::EventType::LostFocus;
            case sf::Event::EventType::GainedFocus:
                return Event::EventType::GainedFocus;
            default:
                return Event::EventType::UnimplementedEvent;
        }
    }

    MouseCodes::Button EventConverter::getMouseButtonFromSFML(sf::Event& event)
    {
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Left:
            return MouseCodes::Button::Left;
        case sf::Mouse::Right:
            return MouseCodes::Button::Right;
        case sf::Mouse::Middle:
            return MouseCodes::Button::Middle;
        case sf::Mouse::XButton1:
            return MouseCodes::Button::XButton1;
        case sf::Mouse::XButton2:
            return MouseCodes::Button::XButton2;
        }
    }

    KeyboardCodes::Key EventConverter::getKeyCodeFromSFML(sf::Event& event)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Key::A:
            return KeyboardCodes::Key::A;
            case sf::Keyboard::Key::B:
            return KeyboardCodes::Key::B;
            case sf::Keyboard::Key::C:
            return KeyboardCodes::Key::C;
            case sf::Keyboard::Key::D:
            return KeyboardCodes::Key::D;
            case sf::Keyboard::Key::E:
            return KeyboardCodes::Key::E;
            case sf::Keyboard::Key::F:
            return KeyboardCodes::Key::F;
            case sf::Keyboard::Key::G:
            return KeyboardCodes::Key::G;
            case sf::Keyboard::Key::H:
            return KeyboardCodes::Key::H;
            case sf::Keyboard::Key::I:
            return KeyboardCodes::Key::I;
            case sf::Keyboard::Key::J:
            return KeyboardCodes::Key::J;
            case sf::Keyboard::Key::K:
            return KeyboardCodes::Key::K;
            case sf::Keyboard::Key::L:
            return KeyboardCodes::Key::L;
            case sf::Keyboard::Key::M:
            return KeyboardCodes::Key::M;
            case sf::Keyboard::Key::N:
            return KeyboardCodes::Key::N;
            case sf::Keyboard::Key::O:
            return KeyboardCodes::Key::O;
            case sf::Keyboard::Key::P:
            return KeyboardCodes::Key::P;
            case sf::Keyboard::Key::Q:
            return KeyboardCodes::Key::Q;
            case sf::Keyboard::Key::R:
            return KeyboardCodes::Key::R;
            case sf::Keyboard::Key::S:
            return KeyboardCodes::Key::S;
            case sf::Keyboard::Key::T:
            return KeyboardCodes::Key::T;
            case sf::Keyboard::Key::U:
            return KeyboardCodes::Key::U;
            case sf::Keyboard::Key::V:
            return KeyboardCodes::Key::V;
            case sf::Keyboard::Key::W:
            return KeyboardCodes::Key::W;
            case sf::Keyboard::Key::X:
            return KeyboardCodes::Key::X;
            case sf::Keyboard::Key::Y:
            return KeyboardCodes::Key::Y;
            case sf::Keyboard::Key::Z:
            return KeyboardCodes::Key::Z;
            case sf::Keyboard::Key::Num0:
            return KeyboardCodes::Key::_0;
            case sf::Keyboard::Key::Num1:
            return KeyboardCodes::Key::_1;
            case sf::Keyboard::Key::Num2:
            return KeyboardCodes::Key::_2;
            case sf::Keyboard::Key::Num3:
            return KeyboardCodes::Key::_3;
            case sf::Keyboard::Key::Num4:
            return KeyboardCodes::Key::_4;
            case sf::Keyboard::Key::Num5:
            return KeyboardCodes::Key::_5;
            case sf::Keyboard::Key::Num6:
            return KeyboardCodes::Key::_6;
            case sf::Keyboard::Key::Num7:
            return KeyboardCodes::Key::_7;
            case sf::Keyboard::Key::Num8:
            return KeyboardCodes::Key::_8;
            case sf::Keyboard::Key::Num9:
            return KeyboardCodes::Key::_9;
            case sf::Keyboard::Key::Escape:
            return KeyboardCodes::Key::Escape;
            case sf::Keyboard::Key::LControl:
            return KeyboardCodes::Key::LControl;
            case sf::Keyboard::Key::LShift:
            return KeyboardCodes::Key::LShift;
            case sf::Keyboard::Key::LAlt:
            return KeyboardCodes::Key::LAlt;
            case sf::Keyboard::Key::LSystem:
            return KeyboardCodes::Key::LSystem;
            case sf::Keyboard::Key::RControl:
            return KeyboardCodes::Key::RControl;
            case sf::Keyboard::Key::RShift:
            return KeyboardCodes::Key::RShift;
            case sf::Keyboard::Key::RAlt:
            return KeyboardCodes::Key::RAlt;
            case sf::Keyboard::Key::RSystem:
            return KeyboardCodes::Key::RSystem;
            case sf::Keyboard::Key::Menu:
            return KeyboardCodes::Key::Menu;
            case sf::Keyboard::Key::LBracket:
            return KeyboardCodes::Key::LBracket;
            case sf::Keyboard::Key::RBracket:
            return KeyboardCodes::Key::RBracket;
            case sf::Keyboard::Key::SemiColon:
            return KeyboardCodes::Key::SemiColon;
            case sf::Keyboard::Key::Comma:
            return KeyboardCodes::Key::Comma;
            case sf::Keyboard::Key::Period:
            return KeyboardCodes::Key::Period;
            case sf::Keyboard::Key::Quote:
            return KeyboardCodes::Key::Quote;
            case sf::Keyboard::Key::Slash:
            return KeyboardCodes::Key::Slash;
            case sf::Keyboard::Key::BackSlash:
            return KeyboardCodes::Key::BackSlash;
            case sf::Keyboard::Key::Tilde:
            return KeyboardCodes::Key::Tilde;
            case sf::Keyboard::Key::Equal:
            return KeyboardCodes::Key::Equal;
            case sf::Keyboard::Key::Dash:
            return KeyboardCodes::Key::Dash;
            case sf::Keyboard::Key::Space:
            return KeyboardCodes::Key::Space;
            case sf::Keyboard::Key::Return:
            return KeyboardCodes::Key::Return;
			case sf::Keyboard::Key::BackSpace:
            return KeyboardCodes::Key::BackSpace;
            case sf::Keyboard::Key::Tab:
            return KeyboardCodes::Key::Tab;
            case sf::Keyboard::Key::PageUp:
            return KeyboardCodes::Key::PageUp;
            case sf::Keyboard::Key::PageDown:
            return KeyboardCodes::Key::PageDown;
            case sf::Keyboard::Key::End:
            return KeyboardCodes::Key::End;
            case sf::Keyboard::Key::Home:
            return KeyboardCodes::Key::Home;
            case sf::Keyboard::Key::Insert:
            return KeyboardCodes::Key::Insert;
            case sf::Keyboard::Key::Delete:
            return KeyboardCodes::Key::Delete;
            case sf::Keyboard::Key::Add:
            return KeyboardCodes::Key::Add;
            case sf::Keyboard::Key::Subtract:
            return KeyboardCodes::Key::Subtract;
            case sf::Keyboard::Key::Multiply:
            return KeyboardCodes::Key::Multiply;
            case sf::Keyboard::Key::Divide:
            return KeyboardCodes::Key::Divide;
            case sf::Keyboard::Key::Left:
            return KeyboardCodes::Key::Left;
            case sf::Keyboard::Key::Right:
            return KeyboardCodes::Key::Right;
            case sf::Keyboard::Key::Up:
            return KeyboardCodes::Key::Up;
            case sf::Keyboard::Key::Down:
            return KeyboardCodes::Key::Down;
            case sf::Keyboard::Key::Numpad0:
            return KeyboardCodes::Key::Numpad0;
            case sf::Keyboard::Key::Numpad1:
            return KeyboardCodes::Key::Numpad1;
            case sf::Keyboard::Key::Numpad2:
            return KeyboardCodes::Key::Numpad2;
            case sf::Keyboard::Key::Numpad3:
            return KeyboardCodes::Key::Numpad3;
            case sf::Keyboard::Key::Numpad4:
            return KeyboardCodes::Key::Numpad4;
            case sf::Keyboard::Key::Numpad5:
            return KeyboardCodes::Key::Numpad5;
            case sf::Keyboard::Key::Numpad6:
            return KeyboardCodes::Key::Numpad6;
            case sf::Keyboard::Key::Numpad7:
            return KeyboardCodes::Key::Numpad7;
            case sf::Keyboard::Key::Numpad8:
            return KeyboardCodes::Key::Numpad8;
            case sf::Keyboard::Key::Numpad9:
            return KeyboardCodes::Key::Numpad9;
            case sf::Keyboard::Key::F1:
            return KeyboardCodes::Key::F1;
            case sf::Keyboard::Key::F2:
            return KeyboardCodes::Key::F2;
            case sf::Keyboard::Key::F3:
            return KeyboardCodes::Key::F3;
            case sf::Keyboard::Key::F4:
            return KeyboardCodes::Key::F4;
            case sf::Keyboard::Key::F5:
            return KeyboardCodes::Key::F5;
            case sf::Keyboard::Key::F6:
            return KeyboardCodes::Key::F6;
            case sf::Keyboard::Key::F7:
            return KeyboardCodes::Key::F7;
            case sf::Keyboard::Key::F8:
            return KeyboardCodes::Key::F8;
            case sf::Keyboard::Key::F9:
            return KeyboardCodes::Key::F9;
            case sf::Keyboard::Key::F10:
            return KeyboardCodes::Key::F10;
            case sf::Keyboard::Key::F11:
            return KeyboardCodes::Key::F11;
            case sf::Keyboard::Key::F12:
            return KeyboardCodes::Key::F12;
            case sf::Keyboard::Key::F13:
            return KeyboardCodes::Key::F13;
            case sf::Keyboard::Key::F14:
            return KeyboardCodes::Key::F14;
            case sf::Keyboard::Key::F15:
            return KeyboardCodes::Key::F15;
            case sf::Keyboard::Key::Pause:
            return KeyboardCodes::Key::Pause;
        };
    }
}
