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
@todo Czy IInputStream#read i IOutputStream#write nie powinny używać uint zamiast size_t?
*/
#pragma once

namespace Habanero
{
	//! Strumień z którego można czytać
	class IInputStream
	{
	public:
		/*!
		@brief Czyta dane ze strumienia
		@param[out] buff Bufor do którego należy czytać dane
		@param size Ilość bajtów do przeczytania
		@return Ilość przeczytanych bajtów, lub 0 w przypadku końca strumienia
		Ilość przeczytanych danych może być mniejsza niż pozwala na to \a size,
		lecz nigdy zerowa przed definitywnym końcem strumienia.
		W przypadku błędu metoda powinna zgłosić wyjątek.
		*/
		virtual size_t read(void *buff, size_t size) = 0;
		void readall(void *buff, size_t size);
		
		template <typename T>
		void read(T &scalar)
		{
			readall(&scalar, sizeof(scalar));
		}

		template <typename T, size_t L>
		void read(T (&array)[L])
		{
			readall(array, sizeof(array));
		}
	};

	//! Strumień do którego można zapisywać
	class IOutputStream
	{
	public:
		/*!
		@brief Zapisuje dane do strumienia
		@param[in] buff Bufor który należy zapisać do strumienia
		@param size Ilość bajtów do zapisania
		W przypadku błędu metoda powinna zgłosić wyjątek.
		*/
		virtual void write(const void *buff, size_t size) = 0;
		//! Jeśli strumień jest buforowany, opróżnia bufor. Nie jest potrzebne przed zamknięciem.
		virtual void flush() {}

		template <typename T>
		void write(const T &scalar)
		{
			write(&scalar, sizeof(scalar));
		}

		template <typename T, size_t L>
		void write(const T (&array)[L])
		{
			write(array, sizeof(array));
		}
	};

	//! Strumień do zapisu i odczytu
	class IStream : public IInputStream, public IOutputStream
	{
	};
}
