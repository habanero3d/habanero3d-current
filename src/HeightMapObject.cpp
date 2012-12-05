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

#include <stddef.h>
#include <math.h>

#include "HeightMapObject.h"
#include "AABB.h"
#include "Memory.h"
#include "Spatial.h"
#include "StaticVertex.h"
#include "GenericGeometry.h"
#include "Memory.h"
#include "HabMath.h"

namespace Habanero
{
	HeightMapObject::HeightMapObject(ref<HeightMapMesh> hm) :
		Spatial(hm->boundingVolume->clone()), heightMap(hm)
	{
	}

	void HeightMapObject::initBoundingVolume()
	{
		boundingVolume->growToContain(*(heightMap->boundingVolume));
	}
	
	void HeightMapObject::fillFrameDescription(const CullingVolumesSet& cullingVolumes, FrameDescription& scene, const SceneMembership membership) const
	{
		matrix4f transformation = getWorldToLocalR();
		
		GenericGeometry<StaticVertex> * geometry = heightMap->getGeometry();
		
		StaticObjectDrawTask * task = new (scene) StaticObjectDrawTask(transformation, geometry);
		
		if(membership == SceneMembership::VisibleObject)
			scene.visibleObjects.push_back(task);
		else
			scene.shadowCasters.push_back(task);
	}

	//float HeightMap::getHeightAt(float x, float y) const
    float HeightMapObject::getHeightAt(double x, double y) const
	{
		return heightMap->getHeightAt(x, y);
	}

	bool HeightMapObject::reload()
	{
		ref<Texture> tex = ResourceManager::getInstance().getTexture(heightMap->getTerrainTexture());
		tex->unload();
		heightMap->unload();
		return heightMap->load();
	}
}
