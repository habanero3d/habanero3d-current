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

#ifdef WIN32

#include <cstdio>
#include "win_windows.h"

#include "Exception.h"
#include "File.h"

namespace Habanero
{
	File::uptr File::map(const char *fileName, uint access, uint64 offset, size_t size, uint64 *fileSize)
	{
		struct HandleDeleter
		{
			typedef HANDLE pointer;
			void operator ()(pointer handle) const
			{
				if (handle != INVALID_HANDLE_VALUE && !CloseHandle(handle))
					RAISE(Exception, "CloseHandle");
			}
		};

		HASSERT((access & ~(Read|Write|Copy)) == 0);
		DWORD fileAccess = 0, mappingAccess, mapAccess;
		if (access & Read)
		{
			fileAccess = FILE_READ_DATA;
			mappingAccess = PAGE_READONLY;
			mapAccess = FILE_MAP_READ;
		}
		if (access & Write)
			if (access & Copy)
			{
				mappingAccess = PAGE_WRITECOPY;
				mapAccess = FILE_MAP_COPY;
			}
			else
			{
				fileAccess |= FILE_WRITE_DATA;
				mappingAccess = PAGE_READWRITE;
				mapAccess = FILE_MAP_WRITE;
			}
		std::unique_ptr<HANDLE, HandleDeleter> hFile(CreateFileA(fileName, fileAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL));
		if (hFile.get() == INVALID_HANDLE_VALUE)
			RAISE(Exception, "CreateFile");
		if (fileSize)
			GetFileSizeEx(hFile.get(), (LARGE_INTEGER*)fileSize);
		std::unique_ptr<HANDLE, HandleDeleter> hMapping(CreateFileMapping(hFile.get(), NULL, mappingAccess, 0, size, NULL));
		if (!hMapping)
			RAISE(Exception, "CreateFileMapping");
		uptr data(MapViewOfFile(hMapping.get(), mapAccess, offset >> 32, (DWORD)offset, size));
		if (!data)
			RAISE(Exception, "MapViewOfFile");
		return data;
	}

	void File::seek(int64 offset)
	{
		_fseeki64(stream, offset, SEEK_CUR);
	}

	void File::seekFromStart(uint64 offset)
	{
		_fseeki64(stream, offset, SEEK_SET);
	}

	void File::seekFromEnd(uint64 offset)
	{
		_fseeki64(stream, offset, SEEK_END);
	}

	uint64 File::currentPosition() const
	{
		return (uint64)_ftelli64(stream);
	}

	File::uptr File::map(const char *fileName, uint access, uint64 offset, size_t size)
	{
		return map(fileName, access, offset, size, NULL);
	}

	File::uptr File::map(const char *fileName, uint access, uint64 *size)
	{
		HASSERT(size != NULL);
		return map(fileName, access, 0, 0, size);
	}

	void File::unmap(void *ptr)
	{
		if (!UnmapViewOfFile(ptr))
			RAISE(Exception, "UnmapViewOfFile");
	}

	void File::unmap(uptr &&ptr)
	{
		unmap(ptr.release());
	}

	bool listDirectory(std::string directory, std::vector<std::string>& result)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(std::string(directory + "\\*").c_str(), &FindFileData); 
		if (hFind == INVALID_HANDLE_VALUE) 
			return false;
		do 
		{
			std::string filename(FindFileData.cFileName);
			if ( filename == "." || filename == ".." || filename == ".svn")
				continue;
			result.push_back(filename);
		} 
		while (FindNextFile(hFind, &FindFileData));
		
		if (GetLastError() != ERROR_NO_MORE_FILES)
			return false;

		FindClose(hFind);
		return true;
	}

	bool isDirectory(std::string path)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(path.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
			return false;
		return FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
	}

	bool isFile(std::string path)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(path.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
			return false;
		return ! (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}
}

#endif
