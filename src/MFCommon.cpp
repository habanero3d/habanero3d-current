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

#include "AABB.h"
#include "Sphere.h"
#include "MFCommon.h"
#include "Exception.h"
#include "IBoundingVolume.h"

namespace Habanero
{
	namespace MF
	{
		SubMesh* SubMesh::getNext()
		{
			return (SubMesh*)((byte*)(this + 1) + numIndices * sizeof(indices[0]));
		}		

		IBoundingVolume<float>* createBoundingVolume(BoundingVolumeType *type, size_t *size)
		{
			*size = sizeof(BoundingVolumeType);
			switch (*type)
			{
				case BoundingVolumeType::AABB:
				{
					AABB *bb = (AABB*)(type + 1);
					*size += sizeof(AABB);
					return new Habanero::AABB<float>(bb->xMin, bb->yMin, bb->zMin, bb->xMax, bb->yMax, bb->zMax);
				}
				/*
				case BoundingVolumeType::OBB:
				{
					RAISE(Exception, "OBB is obsolete, removed from Habanero");
				}*/
				case BoundingVolumeType::Sphere:
				{
					Sphere *bb = (Sphere*)(type + 1);
					*size += sizeof(Sphere);
					return new Habanero::Sphere<float>(bb->center, bb->radius);
				}
				default:
					RAISE(Exception, "Unknown bounding box type");
			}
		}
	}

	namespace MF2
	{
		SubMesh* LODLevel::getFirstSubMesh()
		{
			return (SubMesh*)(this + 1);	// przeskakujemy pola w strukturce
		}

		LODLevel* LODLevel::getNext()
		{
			SubMesh* subMesh = getFirstSubMesh();	// pierwszy SubMesh
			for(uint i = 0; i < numSubMeshes; i++)
				subMesh = subMesh->getNext();	// przeskakujemy wszystkie
			return (LODLevel*)subMesh;
		}

		SubMesh* SubMesh::getNext()
		{
			return (SubMesh*)((byte*)(this + 1) + numIndices * sizeof(indices[0]));
		}

		VertexMove * VertexMove::getNext()
		{
			return (this + 1);
		}

		TriangleAddition * TriangleAddition::getNext()
		{
			return (this + 1);
		}

		VertexMove * VSplit::getFirstMove()
		{
			return (VertexMove *)(this + 1);
		}

		TriangleAddition * VSplit::getFirstAddition()
		{
			return (TriangleAddition *)(getFirstMove() + numMoves);
		}

		VSplit * VSplit::getNext()
		{
			return (VSplit *)(getFirstAddition() + numAdditions);
		}
	}
}