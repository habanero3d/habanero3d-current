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


/*! @file
@brief Plik ktore jest automatycznie wszedzie include'owany (najbardziej podstawowe narzędzia).
  Pierwotnie napisany przez qfla, 3/4 wywalone przez akuda
@author qfel, akuda
*/
#pragma once

namespace Habanero
{
	/*!
	@brief Zwraca wielkość tablicy, w elementach
	@todo Powinno zwracać size_t, ale to jest typedef i nie chce go includować w pliku który jest wszędzie włączany
	*/
	template <class T, unsigned n>
	inline unsigned array_length(T (&arr)[n])
	{
		return n;
	}
        
	typedef signed char sbyte;
	typedef unsigned char byte;
	typedef short int16;
	typedef unsigned short uint16;
	typedef int int32;
	typedef unsigned int uint32;
	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint;
	typedef unsigned long ulong;
}