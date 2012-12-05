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

// author: akuda

/*
 ta struktura opisuje frustę i ShadowCasterVolumes do przycinania
 
 TODO: zaimplementować współdzielenie płaszczyzn, może nawet wspólny test
 przecinania
 */

#pragma once

#include "Spatial.h"
#include "Enum.h"
#include "Frustum.h"
#include "Polyhedron.h"
#include "SceneMembership.h"

namespace Habanero
{	
	template <typename Real> class IBoundingVolume;
	
	class CullingVolumesSet
	{
	private:
		Frustum<float> frustum;
		std::vector<Polyhedron<float> > shadowCasterVolumes;
	public:

		CullingVolumesSet(const Frustum<float>& frustum);
		CullingVolumesSet(const CullingVolumesSet& cvs);
		
		
		//! zwraca sumaryczną ilość płaszczyzn biorących udział w teście
		uint getNumberOfPlanes() const;
		
		//! zwraca liczbę brył (1 + liczba shadow casterów)
		uint getNumberOfVolumes() const;
		
		void transform(const matrix4<float>& matrix);
		void addPointLightSource(const vector3f &worldLightPos);
		
		SceneMembership getSceneMembership(const IBoundingVolume<float>& ibv) const;
	};
}
