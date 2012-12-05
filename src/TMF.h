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
@brief Tajemniczy Mesh Format
@author qfel
*/

#pragma once
#include "StaticMesh.h"
#include "MFCommon.h"

namespace Habanero
{
	struct StaticVertex;
	class BoundingVolumeType;
	class ResourceManager;

	namespace TMF2
	{
		using namespace MF2;

		const uint signature = '2FMT';

		struct StaticMesh
		{
			uint numVertices,
			     numSubMeshes;
			//StaticVertex vertices[numVertices];
			//SubMesh subMeshes[numSubMeshes];
			StaticVertex* getVertices();
			Habanero::MF2::SubMesh* getFirstSubMesh();
			BoundingVolumeType* getBoundingVolume();
		};

		struct File
		{
			uint signature;
			StaticMesh mesh;
		};

		Habanero::StaticMesh::InitInfo loadFromFile(const char *filename, Resource::Mapping &materials, Resource::Mapping &textures);
	}
}