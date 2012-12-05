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
#include <memory>
#include <malloc.h>

namespace Habanero
{
	template <typename T>
	T* uninitializedAlloc(uint count)
	{
		return (T*)operator new(sizeof(T) * count);
	}

	template <typename T>
	void uninitializedDelete(T *ptr, uint count)
	{
		for (uint i = 0; i < count; i++)
			ptr[i].T::~T();
		operator delete(ptr);
	}

	template <typename T>
	std::unique_ptr<T> unique(T *ptr)
	{
		return std::unique_ptr<T>(ptr);
	}

	template <typename T>
	std::shared_ptr<T> shared(T *ptr)
	{
		return std::shared_ptr<T>(ptr);
	}
	
	inline void * alignedAlloc(uint alignment, uint a_size)
	{
		//TODO on windows use _aligned_malloc()
		return memalign(alignment, a_size);
	}
	
	inline void alignedFree(void* a_pointer)
	{
		//TODO on windows use _aligned_free()
		free(a_pointer);
	}
}
