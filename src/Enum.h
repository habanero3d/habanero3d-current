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


/*!
@file
@author qfel
*/
#pragma once

#include "default.h"

#define ENUM_(name, decl, ...) \
	class name { \
	public: enum EnumType decl {__VA_ARGS__, COUNT}; \
	private: EnumType value; \
	public: \
		name() {} \
		name(EnumType value) : value(value) {} \
		explicit name(int value) : value((EnumType)value) {} \
		name& operator =(name rhs) { value = rhs.value; return *this; } \
		operator EnumType() const { return value; } \
	}

#define ENUMT(name, type, ...) ENUM_(name, : type, __VA_ARGS__)
#define ENUM(name, ...) ENUM_(name, , __VA_ARGS__)
