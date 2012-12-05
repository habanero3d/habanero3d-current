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


#include "Sphere.h"
#include "plane.h"
#include "AABB.h"
#include "vector3.h"
#include "HabMath.h"
#include "GeometryDrawTask.h"
#include "FrameDescription.h"

namespace Habanero
{
#pragma region Konstruktory

	template <typename Real>
	Sphere<Real>::Sphere()
	{
	}

	template <typename Real>
	Sphere<Real>::Sphere(const vector3<Real> &c, Real r)
	: center(c), radius(r)
	{
	}

	template <typename Real>
	Sphere<Real>::Sphere(const vector3<Real> &c, const vector3<Real> &r)
	: center(c), radius(r.length())
	{
	}
#pragma endregion

#pragma region IBoundingVolume

	template <typename Real>
	PlaneInterferenceType Sphere<Real>::planeInterference(const plane<Real> &p) const
	{
		Real dist = dot(p.normal, center) + p.d;
		if (dist > 0)
			if (dist > radius)
				return PlaneInterferenceType::Over;
			else
				return PlaneInterferenceType::Through;
		else
			if (-dist > radius)
			return PlaneInterferenceType::Under;
		else
			return PlaneInterferenceType::Through;
		return PlaneInterferenceType::Over;
	}

	template <typename Real>
	void Sphere<Real>::zero()
	{
		radius = 0;
	}

	template <typename Real>
	bool Sphere<Real>::contains(const vector3<Real> &p) const
	{
		return (p - center).length() <= radius;
	}

	template <typename Real>
	BoundingVolumeType Sphere<Real>::getType() const
	{
		return BoundingVolumeType::Sphere;
	}

	template <typename Real>
	bool Sphere<Real>::contains(const IBoundingVolume<Real> &bbox) const
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return contains(dynamic_cast<const AABB<Real>&> (bbox));
			case BoundingVolumeType::Sphere:
				return contains(dynamic_cast<const Sphere<Real>&> (bbox));
		}
		return false;
	}

	template <typename Real>
	bool Sphere<Real>::collides(const IBoundingVolume<Real> &bbox) const
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return collides(dynamic_cast<const AABB<Real>&> (bbox));
			case BoundingVolumeType::Sphere:
				return collides(dynamic_cast<const Sphere<Real>&> (bbox));
		}
		return false;
	}

	template <typename Real>
	InterferenceType Sphere<Real>::interferes(const IBoundingVolume<Real> &bbox) const
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return interferes(dynamic_cast<const AABB<Real>&> (bbox));
			case BoundingVolumeType::Sphere:
				return interferes(dynamic_cast<const Sphere<Real>&> (bbox));
			default:
				return InterferenceType::NoInterference;
		}
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const IBoundingVolume<Real> &bbox)
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return growToContain(dynamic_cast<const AABB<Real>&> (bbox));
			case BoundingVolumeType::Sphere:
				return growToContain(dynamic_cast<const Sphere<Real>&> (bbox));
			default: HASSERT(false);
		}
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation)
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return growToContain(dynamic_cast<const AABB<Real>&> (bbox), transformation);
			case BoundingVolumeType::Sphere:
				return growToContain(dynamic_cast<const Sphere<Real>&> (bbox), transformation);
			default: HASSERT(false);
		}
	}

	template <typename Real>
	IBoundingVolume<Real> * Sphere<Real>::clone() const
	{
		return new Sphere<Real > (*this);
	}
#pragma endregion

#pragma region AABB

	template <typename Real>
	bool Sphere<Real>::contains(const AABB<Real> &aabb) const
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);

		for (int i = 0; i < 8; i++)
			if (!this->contains(points[i]))
				return false;
		return true;
	}

	template <typename Real>
	bool Sphere<Real>::collides(const AABB<Real> &aabb) const
	{
		return aabb.collides(*this);
	}

	template <typename Real>
	InterferenceType Sphere<Real>::interferes(const AABB<Real> &aabb) const
	{
		if (this->contains(aabb))
			return InterferenceType::Contains;
		if (this->collides(aabb))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const AABB<Real> &aabb)
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);
		bool result = false;

		for (int i = 0; i < 8; i++)
		{
			Real len = (center - points[i]).length();
			if (radius < len)
			{
				radius = len;
				result = true;
			}
		}
		return result;
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const AABB<Real> &aabb, const matrix4<Real> &transformation)
	{
		vector3<Real> points[8];
		aabb.calculatePoints(points);
		for (int i = 0; i < 8; i++)
			points[i] = transformation.transformVertex(points[i]);
		bool result = false;

		for (int i = 0; i < 8; i++)
		{
			Real len = (center - points[i]).length();
			if (radius < len)
			{
				radius = len;
				result = true;
			}
		}
		return result;
	}
#pragma endregion

#pragma region Sphere

	template <typename Real>
	bool Sphere<Real>::contains(const Sphere<Real> &sph) const
	{
		if (this->contains(sph.center))
		{
			Real len = (center - sph.center).length();
			return (len + sph.radius <= radius);
		}
		return false;
	}

	template <typename Real>
	bool Sphere<Real>::collides(const Sphere<Real> &sph) const
	{
		Real len = (center - sph.center).length();
		return (radius + sph.radius >= len);
	}

	template <typename Real>
	InterferenceType Sphere<Real>::interferes(const Sphere<Real> &sph) const
	{
		if (this->contains(sph))
			return InterferenceType::Contains;
		if (this->collides(sph))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const Sphere<Real> &sph)
	{
		Real len = (center - sph.center).length() + sph.radius;
		if (radius < len)
		{
			radius = len;
			return true;
		}
		return false;
	}

	template <typename Real>
	bool Sphere<Real>::growToContain(const Sphere<Real> &sph, const matrix4<Real> &transformation)
	{
		Sphere<Real> sphere = sph;
		sphere.center = transformation.transformVertex(sph.center);
		sphere.radius = transformation.transformNormal(vector3<Real > (0, 0, sph.radius)).length();
		Real len = (center - sphere.center).length() + sphere.radius;
		if (this->radius < len)
		{
			this->radius = len;
			return true;
		}
		return false;
	}
#pragma endregion

	template <typename Real>
	void Sphere<Real>::insertVisualizer(const matrix4<Real>& transformation, FrameDescription &frame) const
	{
		const int resolution = 20;
		
		float localRadius = radius;//transformation.transformNormal(vector3f(radius, 0, 0)).length();

		const int pointNum = (2 * resolution + 1) * (4 * (resolution + 1));
		vector3f * points = frame.allocArray<vector3f > (pointNum);
		int p = 0;

		float a = float(M_PI / (2 * (resolution + 1)));
		for (int yi = -resolution; yi <= resolution; yi++)
		{
			float y = sin(yi * a) * localRadius,
				r = cos(yi * a) * localRadius;
			for (int xi = 0; xi < 4 * (resolution + 1); xi++)
			{
				float x = sin(xi * a) * r,
					z = cos(xi * a) * r;
				points[p++] = (center + vector3<Real > (x, y, z));
				HASSERT(p <= pointNum);
			}
		}

		PointsDrawTask * task = new (frame) PointsDrawTask(transformation,
															points,
															pointNum,
															Color::CRed);
		frame.visibleObjects.push_back(task);
	}

	template class Sphere<float>;
	template class Sphere<double>;
}
