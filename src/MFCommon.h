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
@brief Definicje wspólne dla formatów mesh'y
@author qfel
*/
#pragma once
#include "vector3.h"
#include "Material.h"

// Zeby nie narzekal na T array[0]
#pragma warning(push)
#pragma warning(disable:4200)
namespace Habanero
{
	template <typename Real> class IBoundingVolume;
	class BoundingVolumeType;

	namespace MF
	{
		struct AABB
		{
			float xMin, yMin, zMin,
			      xMax, yMax, zMax;
		};

		struct OBB
		{
			vector3f center,
			         ox, oy, oz; // połowa wysokości w prawo, połowa w górę, połowa w przód, parami prostopadłe
		};

		struct Sphere
		{
			vector3f center;
			float radius;
		};

		struct SubMesh
		{
			Material material;
			uint textureId,	// 0 w przypadku braku tekstury.
			     numIndices,
			     indices[0/*NumIndices*/];

			SubMesh* getNext();
		};

		IBoundingVolume<float>* createBoundingVolume(BoundingVolumeType *type, size_t *size);
	}

	namespace MF2
	{
		struct SubMesh
		{
			uint materialId,
			     numIndices,
			     indices[0/*NumIndices*/];

			SubMesh* getNext();
		};

		struct LODLevel
		{
			uint numVertices;
			uint numSubMeshes;

			SubMesh* getFirstSubMesh();
			LODLevel* getNext();
		};

		struct VertexMove
		{
			uint pattern[3];

			uint fromId;
			uint toId;

			VertexMove * getNext();
		};

		struct TriangleAddition
		{
			uint indices[3];
			
			TriangleAddition * getNext();
		};

		struct VSplit
		{
			uint subMeshId;
			uint numMoves;
			uint numAdditions;

			VertexMove * getFirstMove();
			TriangleAddition * getFirstAddition();
			VSplit * getNext();
		};

		struct VSplitRefinement
		{
			float radius;
			float sinAlphaSquared;
			vector3f referencePosition;
			vector3f referenceNormal;
		};
	}
}
#pragma warning(pop)
