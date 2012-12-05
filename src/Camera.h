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


#pragma once
#include <memory>

#include "matrix4.h"
#include "vector3.h"
#include "line3.h"

#include "Spatial.h"
#include "Frustum.h"


namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class Camera : public Spatial
	{
	private:
		matrix4f view;

		float fov;
		float ratio;
		float nearDist;
		float farDist;

		vector3f front;
		vector3f up;

	public:

		Camera();
		Camera(const vector3f &refPos, const vector3f &upVector, float fov, float ratio, float nearDist, float farDist);

		void lookAt(const vector3f &refPos, const vector3f &up);
		matrix4f getViewMatrix() const; //! Aktualnie zwraca macierz widoku dla swiata (worldToLocalR przemnozona przez lokalna)
		Frustum<float> getFrustum() const;

		float getFOV() const;
		float getRatio() const;
		float getNearDist() const;
		float getFarDist() const;

		const vector3f& getFront() const;
		const vector3f& getUp() const;

		void fillFrameDescription(const CullingVolumesSet &cullingVolumes, FrameDescription &frame, const SceneMembership membership) const;
		
		
		static void setLookAtMatrix(const vector3f& in_refPos, const vector3f& in_up, matrix4f& out_viewMatrix);
	};
}