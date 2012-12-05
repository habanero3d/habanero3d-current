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


#include "AABB.h"

#include "Sphere.h"
#include "plane.h"
#include "StackAllocator.h"
#include "GeometryDrawTask.h"
#include "FrameDescription.h"

namespace Habanero
{
#pragma region Konstruktory

	template <typename Real>
	AABB<Real>::AABB()
	{
	}

	template <typename Real>
	AABB<Real>::AABB(const AABB<Real>& b) :
	xmin(b.xmin), xmax(b.xmax),
	ymin(b.ymin), ymax(b.ymax),
	zmin(b.zmin), zmax(b.zmax)
	{
	}

	template <typename Real>
	AABB<Real>::AABB(Real x1, Real y1, Real z1, Real x2, Real y2, Real z2)
	: xmin(x1), ymin(y1), zmin(z1), xmax(x2), ymax(y2), zmax(z2)
	{
	}

	template <typename Real>
	AABB<Real>::AABB(const vector3<Real> &p1, const vector3<Real> &p2)
	: xmin(p1.x), ymin(p1.y), zmin(p1.z), xmax(p2.x), ymax(p2.y), zmax(p2.z)
	{
	}
#pragma endregion

	template <typename Real>
	void AABB<Real>::calculatePoints(vector3<Real> *output) const
	{
		output[0] = vector3<Real > (xmin, ymin, zmin); // <dolne>
		output[1] = vector3<Real > (xmin, ymin, zmax);
		output[2] = vector3<Real > (xmax, ymin, zmin);
		output[3] = vector3<Real > (xmax, ymin, zmax); // </dolne>
		output[4] = vector3<Real > (xmin, ymax, zmin); // <górne>
		output[5] = vector3<Real > (xmin, ymax, zmax);
		output[6] = vector3<Real > (xmax, ymax, zmin);
		output[7] = vector3<Real > (xmax, ymax, zmax); // </górne>
	}

#pragma region IBoundingVolume

	template <typename Real>
	PlaneInterferenceType AABB<Real>::planeInterference(const plane<Real> &p) const
	{
		vector3<Real> points[8];
		calculatePoints(points);
		bool wasOver = false;
		bool wasUnder = false;

		for (int i = 0; i < 8; i++) // można spróbować optymalizować tego fora
			if (p.distance(points[i]) > 0)
			{
				wasOver = true;
			}
			else
			{
				wasUnder = true;
			}
		if (wasOver && wasUnder)
			return PlaneInterferenceType::Through;
		else if (wasUnder)
			return PlaneInterferenceType::Under;
		else
			return PlaneInterferenceType::Over;
	}

	template <typename Real>
	void AABB<Real>::zero()
	{
		xmin = xmax = 0;
		ymin = ymax = 0;
		zmin = zmax = 0;
	}

	template <typename Real>
	bool AABB<Real>::contains(const vector3<Real> &p) const
	{
		return (xmin <= p.x && p.x <= xmax)
			&& (ymin <= p.y && p.y <= ymax)
			&& (zmin <= p.z && p.z <= zmax);
	}

	template <typename Real>
	BoundingVolumeType AABB<Real>::getType() const
	{
		return BoundingVolumeType::AABB;
	}

	template <typename Real>
	bool AABB<Real>::contains(const IBoundingVolume<Real> &bbox) const
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
	bool AABB<Real>::collides(const IBoundingVolume<Real> &bbox) const
	{
		switch (bbox.getType())
		{
			case BoundingVolumeType::AABB:
				return collides(dynamic_cast<const AABB<Real>&> (bbox));
			case BoundingVolumeType::Sphere:
				return collides(dynamic_cast<const Sphere<Real>&> (bbox));
			default:
				return false;
		}
	}

	template <typename Real>
	InterferenceType AABB<Real>::interferes(const IBoundingVolume<Real> &bbox) const
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
	bool AABB<Real>::growToContain(const IBoundingVolume<Real> &bbox)
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
	bool AABB<Real>::growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation)
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
	IBoundingVolume<Real> * AABB<Real>::clone() const
	{
		return new AABB<Real > (*this);
	}
#pragma endregion

#pragma region AABB

	template <typename Real>
	bool AABB<Real>::contains(const AABB<Real> &aabb) const
	{
		return (xmin <= aabb.xmin && aabb.xmax <= xmax)
			&& (ymin <= aabb.ymin && aabb.ymax <= ymax)
			&& (zmin <= aabb.zmin && aabb.zmax <= zmax);
	}

	template <typename Real>
	bool AABB<Real>::collides(const AABB<Real> &aabb) const
	{
		if (xmax < aabb.xmin)
			return false;
		if (ymax < aabb.ymin)
			return false;
		if (zmax < aabb.zmin)
			return false;

		if (xmin > aabb.xmax)
			return false;
		if (ymin > aabb.ymax)
			return false;
		if (zmin > aabb.zmax)
			return false;

		return true;
	}

	template <typename Real>
	InterferenceType AABB<Real>::interferes(const AABB<Real> &aabb) const
	{
		if (this->contains(aabb))
			return InterferenceType::Contains;
		if (this->collides(aabb))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool AABB<Real>::growToContain(const AABB<Real> &aabb)
	{
		bool result = false;
		if (xmin > aabb.xmin)
		{
			xmin = aabb.xmin;
			result = true;
		}

		if (ymin > aabb.ymin)
		{
			ymin = aabb.ymin;
			result = true;
		}

		if (zmin > aabb.zmin)
		{
			zmin = aabb.zmin;
			result = true;
		}

		if (xmax < aabb.xmax)
		{
			xmax = aabb.xmax;
			result = true;
		}

		if (ymax < aabb.ymax)
		{
			ymax = aabb.ymax;
			result = true;
		}

		if (zmax < aabb.zmax)
		{
			zmax = aabb.zmax;
			result = true;
		}
		return result;
	}

	template <typename Real>
	bool AABB<Real>::growToContain(const AABB<Real> &aabb, const matrix4<Real> &transformation)
	{
		vector3<Real> min(aabb.xmin, aabb.ymin, aabb.zmin);
		vector3<Real> max(aabb.xmax, aabb.ymax, aabb.zmax);

		min = transformation.transformVertex(min);
		max = transformation.transformVertex(max);

		bool result = false;
		if (xmin > min.x)
		{
			xmin = min.x;
			result = true;
		}

		if (ymin > min.y)
		{
			ymin = min.y;
			result = true;
		}

		if (zmin > min.z)
		{
			zmin = min.z;
			result = true;
		}

		if (xmax < max.x)
		{
			xmax = max.x;
			result = true;
		}

		if (ymax < max.y)
		{
			ymax = max.y;
			result = true;
		}

		if (zmax < max.z)
		{
			zmax = max.z;
			result = true;
		}
		return result;
	}
#pragma endregion

#pragma region Sphere

	template <typename Real>
	bool AABB<Real>::contains(const Sphere<Real> &sph) const
	{
		if (this->contains(sph.center))
		{
			if (abs(xmax - sph.center.x) <= sph.radius &&
				abs(xmin - sph.center.x) <= sph.radius &&
				abs(ymax - sph.center.y) <= sph.radius &&
				abs(ymin - sph.center.y) <= sph.radius &&
				abs(zmax - sph.center.z) <= sph.radius &&
				abs(zmin - sph.center.z) <= sph.radius)
				return true;
		}
		return false;
	}

	template <typename Real>
	bool AABB<Real>::collides(const Sphere<Real> &sph) const
	{
		// algorytm Avro - nieznaczna optymalizacja - Biały
		Real s, d = 0;
		if (sph.center.x < xmin)
		{
			s = sph.center.x - xmin;
			d += s * s;
		}
		else if (sph.center.x > xmax)
		{
			s = sph.center.x - xmax;
			d += s * s;
		}

		if (sph.center.y < ymin)
		{
			s = sph.center.y - ymin;
			d += s * s;
		}
		else if (sph.center.y > ymax)
		{
			s = sph.center.y - ymax;
			d += s * s;
		}

		if (sph.center.z < zmin)
		{
			s = sph.center.z - zmin;
			d += s * s;
		}
		else if (sph.center.z > zmax)
		{
			s = sph.center.z - zmax;
			d += s * s;
		}

		return d <= sph.radius * sph.radius;
	}

	template <typename Real>
	InterferenceType AABB<Real>::interferes(const Sphere<Real> &sph) const
	{
		if (this->contains(sph))
			return InterferenceType::Contains;
		if (this->collides(sph))
			return InterferenceType::Collides;
		return InterferenceType::NoInterference;
	}

	template <typename Real>
	bool AABB<Real>::growToContain(const Sphere<Real> &sph)
	{
		vector3<Real> points[] = {
			sph.center + vector3<Real > (0, 0, sph.radius),
			sph.center + vector3<Real > (0, sph.radius, 0),
			sph.center + vector3<Real > (sph.radius, 0, 0),
			sph.center - vector3<Real > (0, 0, sph.radius),
			sph.center - vector3<Real > (0, sph.radius, 0),
			sph.center - vector3<Real > (sph.radius, 0, 0)
		};
		bool result = false;

		for (int i = 0; i < array_length(points); i++)
		{
			if (xmin > points[i].x)
			{
				xmin = points[i].x;
				result = true;
			}
			if (ymin > points[i].y)
			{
				ymin = points[i].y;
				result = true;
			}
			if (zmin > points[i].z)
			{
				zmin = points[i].z;
				result = true;
			}

			if (xmax < points[i].x)
			{
				xmax = points[i].x;
				result = true;
			}
			if (ymax < points[i].y)
			{
				ymax = points[i].y;
				result = true;
			}
			if (zmax < points[i].z)
			{
				zmax = points[i].z;
				result = true;
			}
		}
		return result;
	}

	template <typename Real>
	bool AABB<Real>::growToContain(const Sphere<Real> &sph, const matrix4<Real> &transformation)
	{
		vector3<Real> points[] = {
			sph.center + vector3<Real > (0, 0, sph.radius),
			sph.center + vector3<Real > (0, sph.radius, 0),
			sph.center + vector3<Real > (sph.radius, 0, 0),
			sph.center - vector3<Real > (0, 0, sph.radius),
			sph.center - vector3<Real > (0, sph.radius, 0),
			sph.center - vector3<Real > (sph.radius, 0, 0)
		};
		for (int i = 0; i < array_length(points); i++)
			points[i] = transformation.transformVertex(points[i]);
		bool result = false;

		for (int i = 0; i < 6; i++)
		{
			if (xmin > points[i].x)
			{
				xmin = points[i].x;
				result = true;
			}
			if (ymin > points[i].y)
			{
				ymin = points[i].y;
				result = true;
			}
			if (zmin > points[i].z)
			{
				zmin = points[i].z;
				result = true;
			}

			if (xmax < points[i].x)
			{
				xmax = points[i].x;
				result = true;
			}
			if (ymax < points[i].y)
			{
				ymax = points[i].y;
				result = true;
			}
			if (zmax < points[i].z)
			{
				zmax = points[i].z;
				result = true;
			}
		}
		return result;
	}
#pragma endregion

	template <typename Real>
	void AABB<Real>::insertVisualizer(const matrix4<Real>& transformation, FrameDescription &frame) const
	{
		line3f * lines = frame.allocArray<line3f>(12);
		
		/*
		line3<Real> lines[12] = {
			line3<Real > (vector3<Real > (xmin, ymin, zmin), vector3<Real > ()),
		};
		
		lines[0] = vector3f(xmin, ymin, zmin);
		*/
		
		Real x[] = {xmin, xmax};
		Real y[] = {ymin, ymax};
		Real z[] = {zmin, zmax};
		
		auto makeVector = [&](unsigned mask){
			return vector3<Real > (x[mask & 1], y[mask >> 1 & 1], z[mask >> 2 & 1]);
		};
		for (uint i = 0, l = 0; i != 8; i++)
			for (uint j = 0; j < 3; j++)
				if (i & (1u << j))
				{
					HASSERT(l < 12);
					lines[l++] = line3<Real > (makeVector(i), makeVector(i ^ (1u << j)));
				}

		LinesDrawTask * task = new (frame) LinesDrawTask(transformation, lines, 12);
		frame.visibleObjects.push_back(task);
	}

	template class AABB<float>;
	template class AABB<double>;
}
