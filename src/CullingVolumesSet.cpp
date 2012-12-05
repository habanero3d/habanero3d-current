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

#include "CullingVolumesSet.h"
#include "IBoundingVolume.h"
#include "matrix4.h"
#include "SRT.h"

// author: akuda

/*
 ta struktura opisuje frustę i ShadowCasterVolumes do przycinania
 */


namespace Habanero
{

	CullingVolumesSet::CullingVolumesSet(const Frustum<float>& frustum) :
	frustum(frustum)
	{
	}

	CullingVolumesSet::CullingVolumesSet(const CullingVolumesSet& cvs) :
	frustum(cvs.frustum),
	shadowCasterVolumes(cvs.shadowCasterVolumes)
	{
	}

	void CullingVolumesSet::transform(const matrix4<float>& matrix)
	{
		frustum.transform(matrix);
		for (uint i = 0; i < shadowCasterVolumes.size(); ++i)
			shadowCasterVolumes[i].transform(matrix);
	}

	uint CullingVolumesSet::getNumberOfPlanes() const
	{
		uint res = frustum.getPlanes().size();

		for (uint i = 0; i < shadowCasterVolumes.size(); ++i)
			res += shadowCasterVolumes[i].getPlanes().size();
		
		return res;
	}
	
	uint CullingVolumesSet::getNumberOfVolumes() const
	{
		return 1 + shadowCasterVolumes.size();
	}

	SceneMembership CullingVolumesSet::getSceneMembership(const IBoundingVolume<float>& ibv) const
	{
		if (!frustum.isCulling(ibv))
			return SceneMembership::VisibleObject;
		else
		{
			bool anyShadow = false;
			for (uint i = 0; i < shadowCasterVolumes.size(); ++i)
				if (!shadowCasterVolumes[i].isCulling(ibv))
				{
					anyShadow = true;
					break;
				}

			if (anyShadow)
				return SceneMembership::ShadowCaster;
			else
				return SceneMembership::NotPresent;
		}
	}

	void CullingVolumesSet::addPointLightSource(const vector3f &worldLightPos)
	{
		if (!frustum.isPointInside(worldLightPos))
		{
			Polyhedron<float> scv = frustum.getExpandedWithConvexPoint(worldLightPos);
			shadowCasterVolumes.push_back(scv);
		}
	}
}
