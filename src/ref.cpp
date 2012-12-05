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

#include "ref.h"
#include "MemoryManager.h"
#include "IManagedObject.h"

#include "Skeleton.h"
#include "SkinnedMesh.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "HeightMapMesh.h"


namespace Habanero
{

	uint BaseRef::getObjectID() const
	{
		return ID;
	}

	IManagedObject * BaseRef::getObject() const
	{
		return object;
	}

	BaseRef::BaseRef()
	{
		ID = 0u;
		object = MemoryManager::getInstance().__getRawPointerFromID(ID);
		MemoryManager::getInstance().__registerRef(this);
	}

	BaseRef::BaseRef(const BaseRef& source)
	{
		ID = source.getObjectID();
		object = source.getObject();
		MemoryManager::getInstance().__registerRef(this);
	}

	BaseRef::BaseRef(const uint ID, const IManagedObject* o)
	: ID(ID), object(const_cast<IManagedObject*>(o))
	{
		if(ID != 0u)
			HASSERT(object->getObjectID() == ID);
		MemoryManager::getInstance().__registerRef(this);
	}

	BaseRef::~BaseRef()
	{
		MemoryManager::getInstance().__deregisterRef(this);
	}

	bool BaseRef::isValid() const
	{
		return ID != 0u;
	}

	template <typename T>
	ref<T>::ref(const uint ID, const T * o) : BaseRef(ID, o)
	{
	}

	template <typename T>
	ref<T>::ref() : BaseRef()
	{
	}

	template <typename T>
	ref<T>::ref(const ref<T> &s) : BaseRef(s)
	{
	}

	template <typename T>
	T * ref<T>::operator->() const
	{
		return static_cast<T*> (this->object);
	}

	template <typename T>
	T * ref<T>::getObject() const
	{
		return static_cast<T*> (this->object);
	}

	template <typename T>
		ref<T>& ref<T>::operator=(const ref<T>& source)
	{
		MemoryManager &m = MemoryManager::getInstance();
		m.__deregisterRef(this);
		ID = source.getObjectID();
		object = source.getObject();
		m.__registerRef(this);
		return *this;
	}

	template <typename T>
	bool ref<T>::operator==(ref<T> const &b) const
	{
		return ID == b.ID;
	}

	template <typename T>
	ref<T> ref<T>::fromID(const unsigned int ID)
	{
		T * p = static_cast<T*> (MemoryManager::getInstance().__getRawPointerFromID(ID));
		if(ID != 0u)
			HASSERT(p->getObjectID() == ID);
		return ref<T > (ID, p);
	}

	template <typename T>
	ref<T> ref<T>::fromRawPointer(const T * o)
	{
		uint ID = o->getObjectID();
		return ref<T > (ID, o);
	}

	template class ref<Texture>;
	template class ref<Skeleton>;
	template class ref<SkinnedMesh>;
	template class ref<StaticMesh>;
	template class ref<HeightMapMesh>;

}