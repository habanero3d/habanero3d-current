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
#include "default.h"
#include "Exception.h"
#include "CFileStream.h"

namespace Habanero {

	CFileStream::CFileStream(FILE *stream, bool isOwner)
	{
		HASSERT(stream != NULL);
		this->stream = stream;
		this->isOwner = isOwner;
	}

	CFileStream::CFileStream(const char *fileName, uint mode)
	{	
		HASSERT(fileName != NULL);
		HASSERT((mode & ~(Read|Write|Append)) == 0);

		const char *strMode;
		if (mode & Read)
			if (mode & Write)
				if (mode & Append)
					strMode = "a+b";
				else
					strMode = "w+b";
			else
				strMode = "rb";
		else if (mode & Write)
			if (mode & Append)
				strMode = "ab";
			else
				strMode = "wb";

		stream = fopen(fileName, strMode);
		if (!stream)
			RAISE(CRTError, "fopen");
		isOwner = true;
	}

	CFileStream::~CFileStream()
	{
		if (isOwner)
			fclose(stream);
	}

	size_t CFileStream::read(void *buff, size_t size)
	{
		HASSERT(buff != NULL || size == 0);
		size_t result = fread(buff, 1, size, stream);
		if (ferror(stream))
			RAISE(CRTError, "fread", ferror(stream));
		return result;
	}

	void CFileStream::write(const void *buff, size_t size)
	{
		HASSERT(buff != NULL || size == 0);
		size_t result = fwrite(buff, 1, size, stream);
		if (result != size)
			RAISE(Exception, "fwrite");
		else if (ferror(stream))
			RAISE(CRTError, "fwrite", ferror(stream));
	}
}