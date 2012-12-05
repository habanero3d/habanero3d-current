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
Author: Andrzej "akuda" Skalski
 */

#pragma once

#include <vector>
#include "Mapping.h"
#include "MemoryManager.h"
#include "ref.h"
#include "Singleton.h"
#include "GenericMemoryOrdersGenerator.h"

namespace Habanero
{
	class Texture;
	class Skeleton;
	class StaticMesh;
	class SkinnedMesh;
	class HeightMapMesh;
	class FrameDescription;
	class MemorySettingOrder;
	
	class ResourceManager : public Singleton<ResourceManager>
	{
		friend class RootObject;
	private:

		ResourceManager(const ResourceManager&); // shielding
		void operator=(const ResourceManager&); //shielding

		MemoryManager * memoryManagerPtr;

		typedef std::map<uint, std::string> Mapping;
		typedef std::map<std::string, uint> nameToIdType;
		nameToIdType nameToId;
		
		MemoryOrdersGenerator OrdersGenerator;

	public:
		/*
		 * Funkcja ładująca zasoby ze wsakzanego katalogu. W trakcie implementacji
		 */
		void discoveryDirectory(std::string directory);
		void loadMesh(std::string fullPath, std::string name, std::vector<std::string> files);
		
		void getMemoryOrders(const FrameDescription * frame, std::vector<MemorySettingOrder>& orders);

	private:
		ResourceManager();
		~ResourceManager();
		void meshesDiscovery(std::string fullPath, std::string prefix);
		void texturesDiscovery(std::string fullPath, std::string prefix);
		bool parseI2N(const std::string &path,
					const std::string &dir,
					Mapping &materials,
					Mapping &textures,
					Mapping &skeleton,
					Mapping &joints,
					Mapping &animations);
		void getI2NBlock(std::ifstream &file,
						Mapping &mapping,
						bool files = false,
						const std::string &dir = "",
						const std::string &ext = "");

		inline void fixLineEnd(char * line);


	public:

		void discoverSkinnedMesh(const std::string &path, Mapping materials, Mapping textures, Mapping skeletons);
		void discoverStaticMesh(const std::string &path, Mapping materials, Mapping textures);
		void discoverTexture(const std::string &path);
		void discoverSkeleton(const std::string &path);
		void discoverHeightMapMesh(const std::string &path, Mapping materials, Mapping textures);
		
		ref<Texture> getTexture(const std::string &name);
		ref<Skeleton> getSkeleton(const std::string &name);
		ref<StaticMesh> getStaticMesh(const std::string &name);
		ref<SkinnedMesh> getSkinnedMesh(const std::string &name);
		ref<HeightMapMesh> getHeightMapMesh(const std::string &name);
		
	};
}
