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

#include "IManagedObject.h"


// author: akuda

/*
 this file defines ref<IManagedObject derivate> class
 it is a multi purpose, shared reference, that supports object moving (meaning,
 that if a MemoryManager decides to move object, it updates all ref's to this
 object).
 
 The bad news is that therefore MemoryManager tracks all ref's, which means
 that you cannot move them in any other way than by calling defined copy and
 assignment operators. No memcpy, realloc etc here!
 
 */

#pragma once

namespace Habanero
{

	class IManagedObject;
	
	class BaseRef
	{
		friend class MemoryManager;
	protected:
		uint ID;
		IManagedObject * object;

	public:
	
		uint getObjectID() const;
		IManagedObject * getObject() const;
		
		bool isValid() const;
		
	protected:
		BaseRef(); // tworzy error pointera
		BaseRef(const BaseRef& source);
		BaseRef(const uint ID, const IManagedObject * o);
		
		~BaseRef();
	};
	
	template<typename T> 
	class ref : public BaseRef
	{
	private:	
		ref(const uint ID, const T * o);
	public:
		ref();
		ref(const ref<T> &s);
		
		T * operator->() const;
		
		T * getObject() const;
		
		ref<T>& operator=(const ref<T>& source);
		bool operator==(ref<T> const &b) const;
		
		inline static ref<T> fromID(const uint ID);
		inline static ref<T> fromRawPointer(const T* o);
	};
}