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

#include "DirectionalLight.h"
#include "vector3.h"
#include "Color.h"
#include "Frustum.h"
#include "Memory.h"

namespace Habanero
{

	DirectionalLight::DirectionalLight(Color color, vector3f direction, float range) : Light(color, range),
	direction(direction)
	{
		//TODO delete this after debug
		this->setVisualizeBoudingVolume(true);
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::fillLights(std::vector<const Light *> &lights) const
	{
		lights.push_back(this);
	}

	void DirectionalLight::initBoundingVolume()
	{
		boundingVolume->growToContain(range);
	}

	void DirectionalLight::fillFrameDescription(const CullingVolumesSet& cullingVolumes, FrameDescription& scene, const SceneMembership membership) const
	{
		vector3f position = this->getWorldToLocalR().transformVertex(vector3f::zero);
		vector3f direction = getWorldToLocalR().transformNormal(direction);
		float radius = getWorldToLocalR().transformNormal(vector3f(range.radius, 0, 0)).length();
		scene.lights.push_back(
							new (scene) DirectionalLightDrawTask(position, radius, color, direction));


		int resolution = 300;

		vector3f * points = scene.allocArray<vector3f > (resolution);
		for (uint i = 0; i < resolution; i++)
		{
			points[i] = vector3f::zero + this->direction * (this->range.radius * (float(i) / float(resolution)));
		}

		scene.visibleObjects.push_back(
									new (scene) PointsDrawTask(getWorldToLocalR(), points, resolution, Color::CBlue)
									);
	}
}