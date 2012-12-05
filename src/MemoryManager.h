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

#pragma once

#include <set>
#include <map>

#include "Singleton.h"

namespace Habanero
{
	class IManagedObject;
	class BaseRef;

	class MemoryManager : public Singleton<MemoryManager>
	{
	private:
		uint freeid;

		MemoryManager(const MemoryManager&); // shielding
		void operator=(const MemoryManager&); //shielding

		class MapRecord
		{
		private:
			IManagedObject * object; // to nie jest const, bo dopuszczamy przesuwanie
			std::set< const BaseRef * > refs;
		public:
			MapRecord(const IManagedObject * object);
			int getNumberOfReferences() const;
			bool nonReferenced() const;
			void addNewReference(const BaseRef * p);
			void removeReference(const BaseRef * p);
			IManagedObject * getObject() const;
		};

		typedef std::map<uint, MapRecord> idToRecords;
		idToRecords records;

	public:

		MemoryManager();
		~MemoryManager();
		/*
		 wywoływane przez konstruktor IManagedObject,
		 pobiera ID i rejestruje mapę referencji
		 */
		unsigned long int __registerNewObject(const IManagedObject * o);
		/*
		 wywoływane przy niszczeniu IManagedObject,
		 TODO: nie przywraca zużytego ID do puli!
		 */
		void __deregisterObject(const IManagedObject * o);
		void __registerRef(const BaseRef * ref);
		void __deregisterRef(const BaseRef * ref);
		
		IManagedObject * __getRawPointerFromID(const uint ID) const;
	};

}
