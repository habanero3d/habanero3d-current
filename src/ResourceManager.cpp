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

#include "ResourceManager.h"
#include "File.h"
#include <string.h>
#include <fstream>
#include <sstream>

#include "SkinnedMesh.h"
#include "StaticMesh.h"
#include "HeightMapMesh.h"
#include "Skeleton.h"
#include "default.h"
#include "FrameDescription.h"
#include "GeometryDrawTask.h"
#include <map>

namespace Habanero
{

	void ResourceManager::getMemoryOrders(const FrameDescription* frame, std::vector<MemorySettingOrder>& orders)
	{
		//TODO this is a stub method
		OrdersGenerator.putFrameDescription(frame);
		OrdersGenerator.getMemorySettingOrders(orders);
	}

	ResourceManager::ResourceManager() :
	Singleton<ResourceManager>(),
	OrdersGenerator(64 * 1024 * 1024)
	{
		memoryManagerPtr = new MemoryManager();
	}

	ResourceManager::~ResourceManager()
	{
		delete memoryManagerPtr;
	}

	void ResourceManager::discoveryDirectory(std::string directory)
	{
		if (!isDirectory(directory + "/meshes"))
		{
			NEWLOG("[ResourceManager] Content directory does not contains meshes directory.");
			return;
		}
		texturesDiscovery(directory + "/textures", "");
		if (!isDirectory(directory + "/textures"))
		{
			NEWLOG("[ResourceManager] Content directory does not contains textures directory.");
			return;
		}
		meshesDiscovery(directory + "/meshes", "");
	}

	void ResourceManager::texturesDiscovery(std::string fullPath, std::string prefix)
	{
		std::vector<std::string> files;
		if (!listDirectory(fullPath, files))
		{
			NEWLOG("[ResourceManager] cannot list directory \"%s\".", fullPath.c_str());
			return;
		}
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string path = fullPath + "/" + *it;
			if (isDirectory(path))
				texturesDiscovery(path, (prefix == "" ? *it : prefix + "." + *it));
			else
				discoverTexture(path);
		}
	}

	void ResourceManager::meshesDiscovery(std::string fullPath, std::string prefix)
	{
		std::vector<std::string> files;
		if (!listDirectory(fullPath, files))
		{
			NEWLOG("[ResourceManager] cannot list directory \"%s\".", fullPath.c_str());
			return;
		}
		bool onlyFiles = true, onlyDirectories = true;

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string path = fullPath + "/" + *it;
			if (isDirectory(path))
				onlyFiles = false;
			else
				onlyDirectories = false;
		}
		if (!onlyFiles && !onlyDirectories)
		{
			NEWLOG("[ResourceManager] Ignoring directory \"%s\": contains files and directories.", fullPath.c_str());
			return;
		}
		if (onlyFiles)
		{
			if (prefix == "")
				NEWLOG("[ResourceManager] Error: root meshes directory contains files only.");
			loadMesh(fullPath, prefix, files);
		}
		else
		{
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				std::string path = fullPath + "/" + *it;
				meshesDiscovery(path, (prefix == "" ? *it : prefix + "." + *it));
			}
		}
	}

	void ResourceManager::loadMesh(std::string path, std::string name, std::vector<std::string> files)
	{
		//TODO: dopisać tę metodę
		Resource::Mapping materials, textures, skeleton, joints, animations;
		bool i2nFound = false,
			saf = false, smf = false, tmf = false, hmf = false;
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if (!strcmp(it->c_str(), "i2n"))
			{
				std::string fullPath = path + "/" + *it;
				i2nFound = true;
				parseI2N(fullPath, path, materials, textures, skeleton, joints, animations);
			}
		}
		if (!i2nFound)
		{
			NEWLOG("[ResourceManager] ERROR: i2n not found in \"%s\"", path.c_str());
			return;
		}
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::string fullPath = path + "/" + *it;
			const char *s = fullPath.c_str();
			int len = fullPath.size();
			if (!strcmp(it->c_str(), "i2n") || !strcmp(s + len - 4, ".mtf"))
				continue;
			else if (!strcmp(s + len - 4, ".bmp"))
				discoverTexture(s);
			else if (!strcmp(s + len - 4, ".saf"))
			{
				if (saf)
				{
					NEWLOG("[ResourceManager] WARNING: Multiple .saf files in %s", path.c_str());
					continue;
				}
				discoverSkeleton(s);
				saf = true;
			}
			else if (!strcmp(s + len - 4, ".smf"))
			{
				if (smf)
				{
					NEWLOG("[ResourceManager] WARNING: Multiple .smf files in %s", path.c_str());
					continue;
				}
				{
					discoverSkinnedMesh(s, materials, textures, skeleton);
				}
				smf = true;
			}
			else if (!strcmp(s + len - 4, ".tmf"))
			{
				if (tmf)
				{
					NEWLOG("[ResourceManager] WARNING: Multiple .tmf files in %s", path.c_str());
					continue;
				}
				discoverStaticMesh(s, materials, textures);
				tmf = true;
			}
			else if (!strcmp(s + len - 4, ".hmf"))
			{
				if (hmf)
				{
					NEWLOG("[ResourceManager] WARNING: Multiple .hmf files in %s", path.c_str());
					continue;
				}
				discoverHeightMapMesh(s, materials, textures);
				hmf = true;
			}
			else
			{
				//	NEWLOG("ResourceManager: Ignoring %s", s);
			}
		}
		NEWLOG("[ResourceManager] Object %s discovered.", name.c_str());
	}

	void ResourceManager::getI2NBlock(std::ifstream &file,
									Mapping &mapping,
									bool files,
									const std::string &dir,
									const std::string &ext)
	{
		uint id;
		const int bufferSize = 80;
		char buffer[bufferSize];
		while (!file.eof() && file.peek() != '#')
		{
			file >> id;
			if (!(file.peek() == '.'))
				return;

			file.ignore(2, ' ');

			file.getline(buffer, bufferSize);
			fixLineEnd(buffer);

			if (files)
			{
				std::string path = dir + '/' + std::string(buffer) + ext;
				Normalize(path);
				mapping[id] = path;
			}
			else
			{
				mapping[id] = std::string(buffer);
			}
		}
	}

	bool ResourceManager::parseI2N(const std::string &path,
								const std::string &dir,
								Mapping &materials,
								Mapping &textures,
								Mapping &skeleton,
								Mapping &joints,
								Mapping &animations)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return false;
		const int bufferSize = 80;
		char buffer[bufferSize];
		while (!file.eof() && file.peek() == '#')
		{
			file.getline(buffer, bufferSize);
			fixLineEnd(buffer);

			if (!strncmp(buffer, "#materials", 10))
				getI2NBlock(file, materials, true, dir, ".mtf");
			if (!strncmp(buffer, "#textures", 9))
				getI2NBlock(file, textures, true, dir);
			if (!strncmp(buffer, "#skeleton", 9))
				getI2NBlock(file, skeleton, true, dir, ".saf");
			if (!strncmp(buffer, "#joints", 7))
				getI2NBlock(file, joints, false);
			if (!strncmp(buffer, "#animations", 11))
				getI2NBlock(file, animations, false);
		}
		file.close();
		return true;
	}

	/*
	 * Not supper effective way to remove \r and \n from the end of line
	 */
	inline void ResourceManager::fixLineEnd(char * line)
	{
		int finalCharacterPos = strlen(line) - 1;
		while (finalCharacterPos >= 0)
		{
			if (line[finalCharacterPos] == '\r' ||
				line[finalCharacterPos] == '\n')
			{
				line[finalCharacterPos] = '\0';
				finalCharacterPos--;
			}
			else break;
		}
	}

	void ResourceManager::discoverSkinnedMesh(const std::string &path,
											Mapping materials,
											Mapping textures,
											Mapping skeletons)
	{
		NEWLOG("[ResourceManager] Discovering skinned mesh at %s", path.c_str());

		nameToIdType::iterator it = nameToId.find(path);

		// TODO: wywalić w ramach optymalizacji
		if (it != nameToId.end())
		{
			NEWLOG("[ResourceManager] SkinnedMesh: Resource %s is already discovered.", path.c_str());
			return;
		}

		SkinnedMesh * res = new SkinnedMesh(path, materials, textures, skeletons);

		nameToId.insert(std::make_pair(path, res->getObjectID()));

		return;
	}

	void ResourceManager::discoverStaticMesh(const std::string &path,
											Mapping materials,
											Mapping textures)
	{
		NEWLOG("[ResourceManager] Discovering static mesh filename = name %s", path.c_str());

		nameToIdType::iterator it = nameToId.find(path);

		// TODO: wywalić w ramach optymalizacji
		if (it != nameToId.end())
		{
			NEWLOG("[ResourceManager] SkinnedMesh: Resource %s is already discovered.", path.c_str());
			return;
		}

		StaticMesh * res = new StaticMesh(path, materials, textures);

		nameToId.insert(std::make_pair(path, res->getObjectID()));

		return;
	}

	void ResourceManager::discoverTexture(const std::string &path) // path is a name for texture!
	{
		NEWLOG("[ResourceManager] Discovering texture of filename = name %s", path.c_str());

		nameToIdType::iterator it = nameToId.find(path);

		// TODO: wywalić w ramach optymalizacji
		if (it != nameToId.end())
		{
			NEWLOG("[ResourceManager] SkinnedMesh: Resource %s is already discovered.", path.c_str());
			return;
		}

		Texture * res = new Texture(path);

		nameToId.insert(std::make_pair(path, res->getObjectID()));

		return;
	}

#define getSomething(type) \
ref<type> ResourceManager::get ## type (const std::string &name) \
{ \
	NEWLOG("[ResourceManager] Delivering %s %s as requested.", #type, name.c_str()); \
	nameToIdType::iterator it = nameToId.find(std::string(name)); \
	if (it == nameToId.end()) \
	{	\
		NEWLOG("[ResourceManager] ERROR: %s \"%s\" is not discovered.", #type, name.c_str()); \
		return ref<type>::fromID(0);	\
	}	\
	return ref<type>::fromID(it->second);	\
}

	getSomething(Texture)
	getSomething(Skeleton)
	getSomething(StaticMesh)
	getSomething(SkinnedMesh)
	getSomething(HeightMapMesh)
#undef getSomething

	void ResourceManager::discoverSkeleton(const std::string &path) // path is a name for texture!
	{
		NEWLOG("[ResourceManager] Discovering skeleton of filename = name %s", path.c_str());

		nameToIdType::iterator it = nameToId.find(path);

		// TODO: wywalić w ramach optymalizacji
		if (it != nameToId.end())
		{
			NEWLOG("[ResourceManager] SkinnedMesh: Resource %s is already discovered.", path.c_str());
			return;
		}

		Skeleton * res = new Skeleton(path);

		nameToId.insert(std::make_pair(path, res->getObjectID()));

		return;
	}

	void ResourceManager::discoverHeightMapMesh(const std::string &path,
												Mapping materials,
												Mapping textures)
	{
		NEWLOG("[ResourceManager] Discovering static mesh filename = name %s", path.c_str());

		nameToIdType::iterator it = nameToId.find(path);

		// TODO: wywalić w ramach optymalizacji
		if (it != nameToId.end())
		{
			NEWLOG("[ResourceManager] SkinnedMesh: Resource %s is already discovered.", path.c_str());
			return;
		}

		HeightMapMesh * res = new HeightMapMesh(path, materials, textures);

		nameToId.insert(std::make_pair(path, res->getObjectID()));

		return;
	}
}
