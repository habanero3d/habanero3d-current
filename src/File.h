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
@author qfel, akuda

*/
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "CFileStream.h"

namespace Habanero
{
	struct MappingDeleter
	{
		void operator() (const void *ptr) const;
	};

	/*!
	@brief Obsluga plików
	@remarks Dziedziczenie z CFileStream może się zmienić (napewno będzie dziedziczyć z IStream).
	@todo Wartość File#Copy powinna być w zgodzie z odziedziczonymi po CFileStream (mogą być użyte razem z nią w File#map i File#unmap)
	*/
	class File : public CFileStream
	{

        public:
                typedef std::unique_ptr<void, MappingDeleter> uptr;
        private: 
		static uptr map(const char *fileName, uint access, uint64 offset, size_t size, uint64 *fileSize);

	public:            
		enum
		{
			Copy = 8 //!> Copy-on-write, zmapowane fragmenty pliku z tym atrybutem można zmieniać, ale zmiany nie będą odwzorowane w samym pliku
		};

		File(const char *fileName, uint mode = Read) : CFileStream(fileName, mode)
		{
			NEWLOG("Opening file %s ...", fileName);
		}

		void seek(int64 offset);
		void seekFromStart(uint64 offset);
		void seekFromEnd(uint64 offset);
		uint64 currentPosition() const;

		/*!
		@brief Mapuje plik w pamięci
		@param fileName Nazwa pliku
		@param access Rządany dostęp do pliku (kombinacja Read, Write i Copy)
		@param offset Adres w pliku porcji do zamapowania
		@param size Wielkość porcji do zamapowania
		@return Adres zamapowanego fragmentu
		W razie błędu funkcja zgładza wyjątek.
		Zamapowany fragment pliku należy zwolnić przez File#unmap.
		*/
		static uptr map(const char *fileName, uint access = Read, uint64 offset = 0, size_t size = 0);

		/*!
		@brief Mapuje plik w pamięci
		@param fileName Nazwa pliku
		@param access Rządany dostęp do pliku (kombinacja Read, Write i Copy)
		@param[out] size Wielkość zamapowanego pliku
		@return Adres zamapowanego pliku
		W razie błędu funkcja zgładza wyjątek.
		Zamapowany fragment pliku należy zwolnić przez File#unmap.
		Działa tak samo jak przeciążenie File#map z offset i size, z tym że mapuje cały plik i zwraca jego wielkość.
		*/
		static uptr map(const char *fileName, uint access, uint64 *size);

		/*!
		@brief Zwalnia zamapowany fragment pliku
		@param ptr Adres zmapowanego fragmentu
		W razie błędu funkcja zgładza wyjątek.
		*/
		static void unmap(void *ptr);
		static void unmap(uptr &&ptr);
	};

	inline void MappingDeleter::operator() (const void *ptr) const
	{
		File::unmap((void*)ptr);
	}
	
	/* 
	 * funkcja zwraca true jeśli się powiodła, false wpp.
	 * wynik zapisywany jest pod result
	 */
	
	bool listDirectory(std::string directory, std::vector<std::string>& result);
	bool isDirectory(std::string path);
	bool isFile(std::string path);
	void Normalize(std::string &path);
}
