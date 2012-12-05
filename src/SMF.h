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
@brief Skinned Mesh Format
@author qfel
*/
#pragma once
#include "SkinnedMesh.h"
#include "MFCommon.h"

// Zeby nie narzekal na T array[0]
#pragma warning(push)
#pragma warning(disable:4200)
namespace Habanero
{
	struct SkinVertex;
	class BoundingVolumeType;
	class ResourceManager;

	namespace SMF2
	{
		using namespace MF2;

		const uint signature = '2FMS';

		struct SkinnedMesh
		{
			uint numVertices,
			     numSubMeshes;
			//SkinVertex Vertices[NumVertices];
			//SubMesh SubMeshes[NumSubMeshes];
			SkinVertex* getVertices();
			Habanero::MF2::SubMesh* getFirstSubMesh();
			BoundingVolumeType* getBoundingVolume();
		};

		struct File
		{
			uint signature,  // '1FMS'
			     skeletonId; // 0 to brak szkieletu
			SkinnedMesh mesh;
		};

		Habanero::SkinnedMesh::InitInfo loadFromFile(const char *fileName,
													 ResourceMapping &materials, 
													 ResourceMapping &textures, 
													 ResourceMapping &skeletons);
	}
}
#pragma warning(pop)