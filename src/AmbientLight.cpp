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


/*author: akuda */

#include "AmbientLight.h"
#include "vector3.h"
#include "Color.h"
#include "Frustum.h"
#include "Memory.h"

namespace Habanero
{

	AmbientLight::AmbientLight(Color color, float range) : Light(color, range)
	{
	}

	void AmbientLight::initBoundingVolume()
	{
		boundingVolume->growToContain(range);
	}

	void AmbientLight::fillLights(std::vector<const Light*>& lights) const
	{
		lights.push_back(this);
	}

	void AmbientLight::fillFrameDescription(const CullingVolumesSet& cullingVolumes, FrameDescription& scene, const SceneMembership membership) const
	{
		vector3f position = this->getWorldToLocalR().transformVertex(vector3f::zero);
		float radius = getWorldToLocalR().transformNormal(vector3f(range.radius, 0, 0)).length();
		scene.lights.push_back(
							new (scene) AmbientLightDrawTask(position, radius, color));
	}
}