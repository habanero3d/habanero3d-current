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

#include "MemoryManager.h"
#include "IManagedObject.h"
#include "ref.h"

#include "cstdio"

namespace Habanero
{

	MemoryManager::MemoryManager()
	{
		freeid = 1;
		records.insert(std::make_pair(0, MapRecord(NULL)));
		NEWLOG("[MemoryManager] started");
	}

	MemoryManager::~MemoryManager()
	{
		NEWLOG("[MemoryManager] stopped");
	}

	unsigned long int MemoryManager::__registerNewObject(const IManagedObject * o)
	{
		records.insert(std::make_pair(freeid, MapRecord(o)));
		return freeid++;
	}

	void MemoryManager::__deregisterObject(const IManagedObject * o)
	{
		idToRecords::iterator it = records.find(o->getObjectID());
		HASSERT(it->second.nonReferenced());
		records.erase(it);
	}

	void MemoryManager::__registerRef(const BaseRef * ref)
	{
		idToRecords::iterator it = records.find(ref->getObjectID());

		if (it == records.end())
			HASSERT(false);

		it->second.addNewReference(ref);
	}

	void MemoryManager::__deregisterRef(const BaseRef * ref)
	{
		idToRecords::iterator it = records.find(ref->getObjectID());
		if (it == records.end())
			HASSERT(false);

		it->second.removeReference(ref);
	}

	MemoryManager::MapRecord::MapRecord(const IManagedObject * object) : object(const_cast<IManagedObject*> (object)) //zdejmuje consta
	{
	}

	int MemoryManager::MapRecord::getNumberOfReferences() const
	{
		return refs.size();
	}

	bool MemoryManager::MapRecord::nonReferenced() const
	{
		return refs.empty();
	}

	void MemoryManager::MapRecord::addNewReference(const BaseRef * p)
	{
		refs.insert(p);
	}

	void MemoryManager::MapRecord::removeReference(const BaseRef * p)
	{
		refs.erase(p);
	}

	IManagedObject * MemoryManager::MapRecord::getObject() const
	{
		return object;
	}

	IManagedObject * MemoryManager::__getRawPointerFromID(const uint ID) const
	{
		/*if(ID == 0)
			NEWLOG("WARNING: returning a error reference!");*/
		idToRecords::const_iterator it = records.find(ID);
		if (ID != 0)
			HASSERT(it->second.getObject()->getObjectID() == ID);
		HASSERT(it != records.end());
		return it->second.getObject();
	}


}
