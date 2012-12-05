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
 * File:   Event.h
 * Author: akuda
 *
 * Created on 4 październik 2010, 23:36
 */

/* ten plik to PIMPL ukrywający SFMLa przed programami korzystającymi z Habanero */

#pragma once

namespace Habanero
{
        namespace MouseCodes
        {
            enum Button
            {
                Left,
                Right,
                Middle,
                XButton1,
                XButton2
            };
        }

        namespace KeyboardCodes
        {
            enum Key {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            _0,
            _1,
            _2,
            _3,
            _4,
            _5,
            _6,
            _7,
            _8,
            _9,
            Escape,
            LControl,
            LShift,
            LAlt,
            LSystem,
            RControl,
            RShift,
            RAlt,
            RSystem,
            Menu,
            LBracket,
            RBracket,
            SemiColon,
            Comma,
            Period,
            Quote,
            Slash,
            BackSlash,
            Tilde,
            Equal,
            Dash,
            Space,
            Return,
            BackSpace,
            Tab,
            PageUp,
            PageDown,
            End,
            Home,
            Insert,
            Delete,
            Add,
            Subtract,
            Multiply,
            Divide,
            Left,
            Right,
            Up,
            Down,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            Pause
        };
    }

    struct Event {

        enum EventType
        {
            MouseWheelMoved,
            MouseButtonPressed,
            MouseButtonReleased,
            MouseMoved,
            KeyPressed,
            KeyReleased,
            LostFocus,
            GainedFocus,
            UnimplementedEvent
        };

        EventType Type;

        union DataType
        {
            struct MouseMoveEvent {
                int X, Y;                
            } MouseMove;

            struct MouseButtonEvent {
                int X, Y;
                MouseCodes::Button Button;
            } MouseButton;


            struct MouseWheelEvent {
                int Delta;
            } MouseWheel;

            struct KeyEvent {
                KeyboardCodes::Key Code;
            } Key;
        } Data;
    };
}
