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
	template <typename Real> class Sphere;
	template <typename Real> class OBB;

	class FrameDescription;
	
	template <typename Real>
	class AABB : public IBoundingVolume<Real>
	{
	public:
		Real xmin, ymin, zmin;
		Real xmax, ymax, zmax;

		AABB();
		AABB(const AABB<Real>& b);
		AABB(Real x1, Real y1, Real z1, Real x2, Real y2, Real z2);
		AABB(const vector3<Real> &p1, const vector3<Real> &p2);

		// Zastanowić się, czy nie lepiej private
		void calculatePoints(vector3<Real> * outputArray) const;

		PlaneInterferenceType planeInterference(const plane<Real> &p) const;

		//DEPRECATED("Robi co innego niz nazwa sugeruje (wynik zalezy od aktualnego stanu obiektu)")
		virtual void zero();

		bool contains(const vector3<Real> &p) const;
		bool contains(const IBoundingVolume<Real> &bbox) const;
		bool collides(const IBoundingVolume<Real> &bbox) const;
		InterferenceType interferes(const IBoundingVolume<Real> &bbox) const;
		bool growToContain(const IBoundingVolume<Real> &bbox);
		bool growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation);
		BoundingVolumeType getType() const;
		IBoundingVolume<Real> * clone() const;

		bool contains(const AABB<Real> &aabb) const;
		bool collides(const AABB<Real> &aabb) const;
		InterferenceType interferes(const AABB<Real> &aabb) const;
		bool growToContain(const AABB<Real> &aabb);
		bool growToContain(const AABB<Real> &bbox, const matrix4<Real> &transformation);

		bool contains(const Sphere<Real> &sph) const;
		bool collides(const Sphere<Real> &sph) const;
		InterferenceType interferes(const Sphere<Real> &sph) const;
		bool growToContain(const Sphere<Real> &sph);
		bool growToContain(const Sphere<Real> &bbox, const matrix4<Real> &transformation);
		
		void insertVisualizer(const matrix4<Real>& transformation, FrameDescription &frame) const;
	};

	typedef AABB<float> AABBf;
	typedef AABB<double> AABBd;
}
