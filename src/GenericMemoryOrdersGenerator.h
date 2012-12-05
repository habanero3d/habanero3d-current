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

/*
 this object generates memory orders basing on objects usages.
 for now, it manages only meshes.
 in future, it should support also textures.
 
 this object is just a stub, since I had no time to truly optimise it.
 object counting should be done on-line by resource manager.
 
 */

#pragma once

#include "GeometryDrawTask.h"
#include "FrameDescription.h"
#include "MemorySettingOrder.h"
#include <vector>
#include <algorithm>

namespace Habanero
{

	class MemoryOrdersGenerator
	{
	private:

		struct usage_unit
		{
			uint size;
			uint uses;
			GenericGeometryBase * what;

			usage_unit(uint a_size, uint a_uses, GenericGeometryBase * a_what) :
			size(a_size),
			uses(a_uses),
			what(a_what)
			{
			}
		};
	public:

		MemoryOrdersGenerator(uint mem_size) :
		available_memory(mem_size)
		{
		}

		~MemoryOrdersGenerator()
		{
		}

		void putFrameDescription(const FrameDescription * frame);
		void getMemorySettingOrders(std::vector<MemorySettingOrder>& out_result);


	private:
		std::map<GenericGeometryBase *, uint> usages;
		uint available_memory;


		std::vector<usage_unit> memory_usages;

		void putGeometryDrawTasks(const std::vector<GeometryDrawTask *>& tasks, bool use_materials);
	};
}
