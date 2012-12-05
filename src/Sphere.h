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

#include "IBoundingVolume.h"

namespace Habanero
{
	template <typename Real> class OBB;
	template <typename Real> class AABB;
	template <typename Real> class vector3;
	class FrameDescription;
	
	template <typename Real>
	class Sphere : public IBoundingVolume<Real>
	{
	public:
		vector3<Real> center;
		Real radius;

		Sphere();
		Sphere(const vector3<Real> &c, Real r);
		Sphere(const vector3<Real> &c, const vector3<Real> &r);

		PlaneInterferenceType planeInterference(const plane<Real> &p) const;

		virtual void zero();

		bool contains(const vector3<Real> &p) const;
		bool contains(const IBoundingVolume<Real> &bbox) const;
		bool collides(const IBoundingVolume<Real> &bbox) const;
		InterferenceType interferes(const IBoundingVolume<Real> &bbox) const;
		bool growToContain(const IBoundingVolume<Real> &bbox);
		bool growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation);
		IBoundingVolume<Real> * clone() const;

		BoundingVolumeType getType() const;

		bool contains(const AABB<Real> &aabb) const;
		bool collides(const AABB<Real> &aabb) const;
		InterferenceType interferes(const AABB<Real> &aabb) const;
		bool growToContain(const AABB<Real> &aabb);
		bool growToContain(const AABB<Real> &bbox, const matrix4<Real> &transformation);

		bool contains(const OBB<Real> &obb) const;
		bool collides(const OBB<Real> &obb) const;
		InterferenceType interferes(const OBB<Real> &obb) const;
		bool growToContain(const OBB<Real> &obb);
		bool growToContain(const OBB<Real> &bbox, const matrix4<Real> &transformation);

		bool contains(const Sphere<Real> &sph) const;
		bool collides(const Sphere<Real> &sph) const;
		InterferenceType interferes(const Sphere<Real> &sph) const;
		bool growToContain(const Sphere<Real> &sph);
		bool growToContain(const Sphere<Real> &bbox, const matrix4<Real> &transformation);

		void insertVisualizer(const matrix4<Real>& transformation, FrameDescription &frame) const;

	};

	typedef Sphere<double> Sphered;
	typedef Sphere<float> Spheref;

};
