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
@author qfel
*/
#pragma once
#include <string>

/*!
@def RAISE(type, ...)
@brief Zgłasza wyjątek
@param type Typ wyjątku
Zgładza wyjątek typu @a type przekazując mu podaną listę argumentów oraz nazwę pliku, funkcji i numer linii w której użyto tego makra.
*/
#define RAISE(type, ...) \
	throw type(__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__) 

namespace Habanero
{
	//! Bazowa klasa zgłaszanych wyjątków
	class Exception
	{
	protected:
		typedef std::string string;
	public:
		const string msg;
		const char * const fileName,
			* const functionName;
		const uint lineNumber;

		Exception(const string &msg,
				  const char *fileName,
				  const char *functionName,
				  uint lineNumber);
		//! Zwraca czytelny opis wyjątku
		virtual string describe() const;
	};

	//! Klasa wyjątków spowodowanych niepowodzeniem funkcji z CRT
	class CRTError : public Exception
	{
	public:
		const int error;

		//! @param error Kod blędu
		CRTError(const string &msg, int error, const char *fileName, const char *function, uint lineNumber);
		/*! Zapamiętywany jest kod błędu umieszczony w errno. */
		CRTError(const string &msg, const char *fileName, const char *function, uint lineNumber);
		virtual string describe() const;
	};
};
