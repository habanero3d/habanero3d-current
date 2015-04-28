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

/*
 this class represents a stack allocator. Objects are allocated sequentially,
 respectind a desired alignment.
 
 One StackAllocator can hold multiple StackUnits. Whenever a StackUnit limit
 is reached, a new one is allocated. It is the only time the syscall is made.
 
 Objects using StackAllocator as their memory provider are expected to inherit
 from IStackAllocatedUnit.
 */

#pragma once
#include "default.h"
#include <vector>

namespace Habanero
{

	class StackAllocator
	{
	protected:

		struct MemoryUnit
		{
			void * begin;
			void * top;

			MemoryUnit(void * a_begin);
		};

	private:

		std::vector<MemoryUnit> units;
		uint alignment, size;
		MemoryUnit * currentUnit;

	public:
		StackAllocator(uint a_size, uint a_alignment = 64u);
		virtual ~StackAllocator();

		void * alloc(uint a_size_bytes);

		template <typename T>
		T* allocArray(uint element_count)
		{
			return static_cast<T*> (alloc(sizeof (T) * element_count));
		}

		uint getConsumedMemory() const;
		uint getMemoryLeftInPresentUnit() const;

	private:
		StackAllocator(const StackAllocator&);
		void operator=(const StackAllocator&);

	};

	class IStackAllocatedUnit
	{
	public:
		void * operator new(size_t size, StackAllocator &a_stackAllocator);
		void operator delete(void * p);
	
	private: //shielding, IStackAllocatedUnit can be constructed only with stack placement
		void *operator new(size_t size);
		void *operator new[](size_t size);
		void operator delete[](void *p);
	};

}
