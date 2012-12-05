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
#include <stdarg.h>
#include <string>

namespace Habanero
{
	/*!
		@brief Formatuje tekst
		@param[in] fmt Format
		@return Sformatowany tekst
		Funkcja działa dokładnie tak jak printf, z tą różnicą że zamiast wypisywać wynik, zwraca go.
	*/
	std::string format(const char *fmt, ...);

	/*!
		@brief Formatuje tekst
		@param[in] fmt Format
		@param[in] args Lista argumentów formatowania
		@return Sformatowany tekst
		Funkcja działa dokładnie tak jak vprintf, z tą różnicą że zamiast wypisywać wynik, zwraca go.
	*/
	std::string vformat(const char *fmt, va_list args);

	/*!
		@brief Kopiuje string'a
		@param s Wzór
		@return Nowy string zaalokowany przez new[] równy wzorowi
		@remarks Należy pamietać o zwolnieniu wyniku: delete[].
	*/
	char* strdup(const char *s);

	bool startsWith(const char *s, const char *preffix);
	bool startsWith(const std::string &s, const char *preffix);
	bool startsWith(const std::string &s, const std::string &preffix);

	bool endsWith(const char *s, const char *suffix);
	bool endsWith(const std::string &s, const char *suffix);
	bool endsWith(const std::string &s, const std::string &suffix);

	//! Komparator standardowych C-stringów. Służy do tworzenia kontenerów std w których kluczem jest wskaźnik na łańcuch.
	class CStringComparator
	{
	public:
		/*!
			@brief Porównanie leksykograficzne dwóch stringów.
			@param obj1 Lewa strona
			@param obj2 Prawa strona
			obj1 "<" obj2 <=> operator()(obj1, obj2) <=> obj1 jest leksykograficznie mniejszy od obj2.
		*/
		bool operator()(const char *obj1, const char *obj2) const;
	};
}
