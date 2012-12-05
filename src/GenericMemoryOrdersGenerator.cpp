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

#include "GenericMemoryOrdersGenerator.h"

namespace Habanero
{

	void MemoryOrdersGenerator::putFrameDescription(const FrameDescription * frame)
	{
		usages.clear();
		memory_usages.clear();
		putGeometryDrawTasks(frame->visibleObjects, true);
		putGeometryDrawTasks(frame->shadowCasters, false);

		for (auto it = usages.begin(); it != usages.end(); ++it)
		{
			GenericGeometryBase * geometry = it->first;
			memory_usages.push_back(
									usage_unit(
											geometry->getIndicesSize() + geometry->getVertexDataSize(),
											it->second,
											geometry)
									);
		}

		{

			std::sort(memory_usages.begin(), memory_usages.end(),
					[](const struct usage_unit& a, const struct usage_unit & b) -> bool
			{
					  if (a.uses == b.uses)
					  return a.size > b.size;
				else
					  return a.uses > b.uses;
				}
					  );
		}
	}

	void MemoryOrdersGenerator::getMemorySettingOrders(std::vector<MemorySettingOrder>& out_result)
	{
		out_result.clear();

		uint present_usage = 0;

		for (uint i = 0; i < memory_usages.size(); ++i)
		{
			if (present_usage + memory_usages[i].size < this->available_memory)
			{
				if (!memory_usages[i].what->MemorySetting.isCached())
				{
					out_result.push_back(MemorySettingOrder(
															true,
															memory_usages[i].what
															));
				}
			}
			else
			{
				if (memory_usages[i].what->MemorySetting.isCached())
				{
					out_result.push_back(MemorySettingOrder(
															false,
															memory_usages[i].what
															));
				}
			}
		}

		//NEWLOG("orders dispatch");
	}

	void MemoryOrdersGenerator::putGeometryDrawTasks(const std::vector<GeometryDrawTask *>& tasks, bool use_materials)
	{
		for (uint i = 0; i < tasks.size(); ++i)
		{
			switch (tasks[i]->type)
			{
				case GeometryDrawTask::GeometryType::StaticMesh:
				{
					StaticObjectDrawTask * task = (StaticObjectDrawTask *) tasks[i];
					usages[task->getGeometry()]++;
					break;
				}
				case GeometryDrawTask::GeometryType::SkinnedMesh:
				{
					SkinnedObjectDrawTask * task = (SkinnedObjectDrawTask *) tasks[i];
					for (uint meshNo = 0; meshNo < task->getNumMeshes(); ++meshNo)
						usages[task->getGeometryAt(meshNo)]++;
					break;
				}
				default:
					break;
			}
		}
	}
}
