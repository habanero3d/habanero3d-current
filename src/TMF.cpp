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
@brief Tajemniczy Mesh Format - implementacja
@author qfel
 */
#include <memory>

#include "SkinVertex.h"
#include "Exception.h"
#include "SubMesh.h"
#include "Memory.h"
#include "File.h"
#include "TMF.h"
#include "MTF.h"
#include "StaticMesh.h"
#include "Mapping.h"

namespace Habanero
{
	namespace TMF2
	{

		StaticVertex* StaticMesh::getVertices()
		{
			return (StaticVertex*) (this +1);
		}

		Habanero::MF2::SubMesh* StaticMesh::getFirstSubMesh()
		{
			return (Habanero::MF2::SubMesh*)(getVertices() + numVertices);
		}

		BoundingVolumeType* StaticMesh::getBoundingVolume()
		{
			Habanero::MF2::SubMesh *subMesh = getFirstSubMesh();
			for (uint i = 0; i < numSubMeshes; i++)
				subMesh = subMesh->getNext();
			return (BoundingVolumeType*) subMesh;
		}

		Habanero::StaticMesh::InitInfo loadFromFile(const char *filename, Resource::Mapping &materials, Resource::Mapping &textures)
		{
			uint64 size;

			std::unique_ptr<File, MappingDeleter> file((File*) Habanero::File::map(filename, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid TMF signature");
			if (size < sizeof (File))
				RAISE(Exception, "Incomplete file");

			Habanero::StaticMesh::InitInfo staticMesh;
			staticMesh.geometry.subMeshes.resize(file->mesh.numSubMeshes);

			staticMesh.geometry.vertices.resize(file->mesh.numVertices);
			std::copy(file->mesh.getVertices(), file->mesh.getVertices() + file->mesh.numVertices, staticMesh.geometry.vertices.begin());

			MF2::SubMesh *fileSubMesh = file->mesh.getFirstSubMesh();
			for (uint i = 0; i < file->mesh.numSubMeshes; i++, fileSubMesh = fileSubMesh->getNext())
			{
				Habanero::SubMesh::InitInfo &subMesh = staticMesh.geometry.subMeshes[i];
				if (fileSubMesh->materialId != 0)
					subMesh.material = shared(MTF2::loadFromFile(materials[fileSubMesh->materialId].c_str(), textures));
				else
					subMesh.material = shared(new Material());
				subMesh.indices.insert(subMesh.indices.begin(), fileSubMesh->indices, fileSubMesh->indices + fileSubMesh->numIndices);
			}
			size_t bvSize;
			BoundingVolumeType *bvType = file->mesh.getBoundingVolume();
			staticMesh.boundingVolume = Habanero::MF::createBoundingVolume(bvType, &bvSize);
			if ((char*) file.get() + size != (char*) bvType + bvSize)
				RAISE(Exception, "Invalid file size");

			return staticMesh;
		}
	}
}