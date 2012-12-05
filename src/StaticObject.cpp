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


/*! @file
@author akuda
*/
#include "IBoundingVolume.h"
#include "StaticObject.h"
#include "Memory.h"

namespace Habanero
{

	StaticObject::StaticObject(ref<StaticMesh> a_mesh) :
		Spatial(a_mesh->boundingVolume->clone()),
		mesh(a_mesh)
	{
	}

	void StaticObject::initBoundingVolume()
	{
		boundingVolume->growToContain(*(mesh->boundingVolume));
	}

	void StaticObject::fillFrameDescription(const CullingVolumesSet& cullingVolumes, FrameDescription& scene, const SceneMembership membership) const
	{
		matrix4f transformation = getWorldToLocalR();
		
		GenericGeometry<StaticVertex> * geometry = mesh->getGeometry();
		
		StaticObjectDrawTask * task = new (scene) StaticObjectDrawTask(transformation, geometry);
		
		if(membership == SceneMembership::VisibleObject)
			scene.visibleObjects.push_back(task);
		else
			scene.shadowCasters.push_back(task);
	}

	const IBoundingVolume<float>* StaticObject::getPickableBoundingVolume() const
	{
		return mesh->boundingVolume;
	}
}