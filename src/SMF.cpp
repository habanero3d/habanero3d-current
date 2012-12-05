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


/*!
@file
@brief Skinned Mesh Format - implementacja
@author qfel
 modified by everyone :)
*/
#include <memory>

#include "ResourceManager.h"
#include "SkinVertex.h"
#include "Exception.h"
#include "SubMesh.h"
#include "Memory.h"
#include "File.h"
#include "SMF.h"
#include "MTF.h"

namespace Habanero
{
	namespace SMF2
	{
		SkinVertex* SkinnedMesh::getVertices()
		{
			return (SkinVertex*)(this + 1);
		}

		Habanero::MF2::SubMesh* SkinnedMesh::getFirstSubMesh()
		{
			return (Habanero::MF2::SubMesh*)(getVertices() + numVertices);
		}

		BoundingVolumeType* SkinnedMesh::getBoundingVolume()
		{
			Habanero::MF2::SubMesh *subMesh = getFirstSubMesh();
			for (uint i = 0; i < numSubMeshes; i++)
				subMesh = subMesh->getNext();
			return (BoundingVolumeType*)subMesh;
		}

		Habanero::SkinnedMesh::InitInfo loadFromFile(const char *fileName,
													 ResourceMapping &materials, 
													 ResourceMapping &textures, 
													 ResourceMapping &skeletons)
		{

			uint64 size;
			// Uzycie release do obejscia bug'a gcc - patrz definicja nullptr_t
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid SMF signature");
			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");

			Habanero::SkinnedMesh::InitInfo skinnedMesh;
			if (file->skeletonId != 0)
			{
				skinnedMesh.skeleton = ResourceManager::getInstance().getSkeleton(skeletons[file->skeletonId].c_str());
				skinnedMesh.skeleton->load();
			}

			skinnedMesh.geometry.subMeshes.resize(file->mesh.numSubMeshes);

			skinnedMesh.geometry.vertices.resize(file->mesh.numVertices);
			std::copy(file->mesh.getVertices(), file->mesh.getVertices() + file->mesh.numVertices, skinnedMesh.geometry.vertices.begin());

			ref<Texture> texture;
			Habanero::MF2::SubMesh *fileSubMesh = file->mesh.getFirstSubMesh();
			for (uint i = 0; i < file->mesh.numSubMeshes; i++, fileSubMesh = fileSubMesh->getNext())
			{
				Habanero::SubMesh::InitInfo &subMesh = skinnedMesh.geometry.subMeshes[i];
				if (fileSubMesh->materialId != 0)
					subMesh.material = shared(MTF2::loadFromFile(materials[fileSubMesh->materialId].c_str(), textures));
				else
					subMesh.material = shared(new Material());
				subMesh.indices.insert(subMesh.indices.begin(), fileSubMesh->indices, fileSubMesh->indices + fileSubMesh->numIndices);
			}
			size_t bvSize;
			BoundingVolumeType *bvType = file->mesh.getBoundingVolume();
			skinnedMesh.boundingVolume = Habanero::MF::createBoundingVolume(bvType, &bvSize);
			if ((char*)file.get() + size != (char*)bvType + bvSize)
				RAISE(Exception, "Invalid file size");
			return skinnedMesh;
		}
	}
}
