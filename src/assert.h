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
@brief Asercja. Domyślnie włączana na początku każdego pliku (ustawienia kompilatora).
@author qfel
*/
#pragma once

/*!
@def HASSERT(condition)
@brief Minimalnie odbiegająca od standardowej asercja (loguje błąd zamiast wyświetlać okienka)
Asercji można też używać do optymalizacji - przy włączonym NDEBUG mówią kompilatorowi jakie wyrażenia zawsze są prawdziwe.
Np. dobrym pomysłem może być konstrukcja
@code
switch (expr):
{
	case 0: handle0(); break;
	case 1: handle1(); break;
	// ...
	// Obsluzono wszystkie mozliwe przypadki
	default: HASSERT(false);
}
@endcode
*/
#ifndef NDEBUG
#define HASSERT(condition) \
		((condition) || (Habanero::assert_func(#condition, __FILE__, __LINE__, __FUNCTION__), true))
#else
#	define HASSERT(condition) __assume(condition)
#endif

namespace Habanero
{
	void assert_func(const char *cond, const char *file, unsigned int line, const char *func);
}
