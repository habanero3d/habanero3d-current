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


#include <cstdio>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "Exception.h"
#include "File.h"

namespace Habanero
{
	// TODO: uwaga na int64 -> int

	File::uptr File::map(const char *fileName, uint access, uint64 offset, size_t size, uint64 *fileSize)
	{

		struct HandleDeleter
		{
			typedef int pointer;

			void operator ()(pointer handle) const
			{
				if (handle != -1 && close(handle) != -1)
					RAISE(Exception, "close");
			}
		};

		HASSERT((access & ~(Read | Write | Copy)) == 0);
		int fflags = 0, prot = 0, mflags = MAP_SHARED;
		if (access & Read)
		{
			fflags = O_RDONLY;
			prot = PROT_READ;
		}
		if (access & Write)
		{
			prot |= PROT_WRITE;
			if (access & Copy)
				mflags = MAP_PRIVATE;
			else
				fflags = O_RDWR;
		}
		HASSERT(!(fflags & O_RDWR));
		// TODO: QuickFix ze ..
		int file(open(fileName, fflags));
		if (file == -1)
			RAISE(Exception, "open");
		struct stat info;
		if (fstat(file, &info) == -1)
			RAISE(Exception, "fstat");
		if (fileSize)
			*fileSize = info.st_size;
		//uptr data(mmap(nullptr, info.st_size, prot, mflags, file.get(), 0));
		uptr data(malloc(info.st_size));
		::read(file, data.get(), info.st_size);
		//if (data.get() == MAP_FAILED)
		//	RAISE(Exception, "mmap");
		return data;
	}

	void File::seek(int64 offset)
	{
		fseek(stream, offset, SEEK_CUR);
	}

	void File::seekFromStart(uint64 offset)
	{
		fseek(stream, offset, SEEK_SET);
	}

	void File::seekFromEnd(uint64 offset)
	{
		fseek(stream, offset, SEEK_END);
	}

	uint64 File::currentPosition() const
	{
		return(uint64) ftell(stream);
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
		// TODO: QuickFix ze..
		free(ptr);
		//if (!UnmapViewOfFile(ptr))
		//	RAISE(Exception, "UnmapViewOfFile");
	}

	void File::unmap(uptr && ptr)
	{
		unmap(ptr.release());
	}

	bool listDirectory(std::string directory, std::vector<std::string>& result)
	{
		DIR *dp;
		struct dirent *dirp;
		
		dp = opendir(directory.c_str());
		
		if(dp == NULL)
			return false;

		for(dirp = readdir(dp); dirp != NULL; dirp = readdir(dp))
		{
			std::string filename(dirp->d_name);
			if ( filename == "." || filename == ".." || filename == ".svn")
				continue;
			result.push_back(filename);
		}
		
		closedir(dp);
		
		return true;
	}
	
	/*
	 * http://www.gnu.org/software/libc/manual/html_node/Reading-Attributes.html#Reading-Attributes
	 * http://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html#Testing-File-Type
	 */
	
	bool isDirectory(std::string path)
	{
		struct stat buf;
		int res = stat(path.c_str(), &buf);
		if(res != 0)
			return false;
		
		return S_ISDIR(buf.st_mode);
	}
	
	bool isFile(std::string path)
	{
		struct stat buf;
		int res = stat(path.c_str(), &buf);
		if(res != 0)
			return false;
		
		return S_ISREG(buf.st_mode);
	}
}
