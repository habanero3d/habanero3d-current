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
#include <cstdio> // Nie da sie forward-deklarowac FILE bo to typedef (C++ sux)
#include "IStream.h"

namespace Habanero
{
	//! Strumień opakowujący operacje na plikach z CRT
	class CFileStream : public IStream
	{
	private:
		CFileStream(const CFileStream&);
		void operator=(const CFileStream&);

		bool isOwner;

	protected:
		FILE *stream;

	public:
		enum
		{
			Read = 1, //*< Możliwy odczyt z pliku
			Write = 2, //*< Możliwy zapis do pliku
			Append = 4 //*< Dopisuj do końca pliku, zamiast tworzyć go od nowa
		};
		CFileStream(FILE *stream, bool isOwner = false);
		CFileStream(const char *fileName, uint mode = Read);
		~CFileStream();
		virtual size_t read(void *buff, size_t size);
		virtual void write(const void *buff, size_t size);
	};

}
