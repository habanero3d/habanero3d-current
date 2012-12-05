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

// author: akuda

#include "StackAllocator.h"
#include "MemoryManager.h"
#include "Memory.h"

namespace Habanero
{

	StackAllocator::MemoryUnit::MemoryUnit(void* a_begin) : begin(a_begin), top(a_begin)
	{
	}

	
	
	StackAllocator::StackAllocator(uint a_size, uint a_alignment)
	: size(a_size), alignment(a_alignment)
	{
		HASSERT(a_alignment > 0);
		HASSERT(!(a_alignment & (a_alignment - 1))); //true for powers of two

		
		void * new_begin = alignedAlloc(a_alignment, a_size);
		units.push_back(MemoryUnit(new_begin));
		currentUnit = &units[0];
	}

	StackAllocator::~StackAllocator()
	{
		for(uint i = 0 ; i < units.size(); ++i)
			alignedFree(units[i].begin);
	}

	
	
	void * StackAllocator::alloc(uint size_bytes)
	{
		//NEWLOG("[StackAllocator] INFO: requested %u bytes.", size_bytes);
		
		if(size_bytes > size)
		{
			NEWLOG("[StackAllocator] ERROR: this StackAllocator is unable to hold such large objects. %u bytes requested, %u per unit available.",
				 size_bytes,
				 size);
			return NULL;
		}
		
		uint actual_size = size_bytes;
		if (actual_size % alignment != 0u) // rounding up to alignment
			actual_size = ((actual_size / alignment) + 1u) * alignment;
		
		if (getMemoryLeftInPresentUnit() < actual_size)
		{
			NEWLOG("[StackAllocator] Warning: not enough memory, allocating new Unit, #%u", units.size());
			units.push_back(MemoryUnit(alignedAlloc(alignment, size_bytes)));
			currentUnit = &units[units.size()-1];
		}
		void * result = currentUnit->top;
		currentUnit->top = (void*) ((uint64) currentUnit->top + actual_size);
		return result;
	}

	uint StackAllocator::getConsumedMemory() const
	{
		return units.size() * size;
	}
	
	uint StackAllocator::getMemoryLeftInPresentUnit() const
	{
		return size - (uint) ((uint64) (currentUnit->top) - (uint64) (currentUnit->begin));
	}

	void * IStackAllocatedUnit::operator new(size_t size, StackAllocator &a_stackAllocator)
	{
		return a_stackAllocator.alloc(size);
	}
	
	void IStackAllocatedUnit::operator delete(void *)
	{
		// Nothing to do here, memory will be freed along with stack.
	}


}