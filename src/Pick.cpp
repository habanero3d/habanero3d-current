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


#include "Spatial.h"
#include "vector3.h"
#include "Camera.h"
#include "Sphere.h"
#include "HabMath.h"
#include "Pick.h"
#include "AABB.h"
#include "plane.h"

#include <typeinfo>

namespace Habanero
{
	static bool pickSphere(const Spheref &sphere, const vector3f &p, const vector3f &dir, float *distance)
	{
		vector3f s = sphere.center - p;
		float proj = dot(dir, s) / dir.length(), b = sqr(sphere.radius) - sqr((s - proj * dir).length());
		if (b < 0)
			return false;
		b = sqrt(b);
		b = abs(proj - b);
		*distance = b;
		return true;
	}

	static bool pickAABB(const AABBf &aabb, const vector3f &p, const vector3f &dir, float *distance)
	{
		vector3f m, base[3] =
		{
			vector3f(aabb.xmax - aabb.xmin, 0, 0),
			vector3f(0, aabb.ymax - aabb.ymin, 0),
			vector3f(0, 0, aabb.zmax - aabb.zmin),
		};

		bool picked = false;
		auto pick = [&](const vector3f &b1, const vector3f &b2)
		{
			auto det = [](const vector3f &a, const vector3f &b, const vector3f &c)
			{
				return
				  a.x * b.y * c.z
				+ a.y * b.z * c.x
				+ a.z * b.x * c.y
				- c.x * b.y * a.z
				- c.y * b.z * a.x
				- c.z * b.x * a.y;
			};
			float d = det(dir, -b1, -b2);
			if (d == 0)
				return;
			float x = det(dir, m, -b2), y = det(dir, -b1, m), t = det(m, -b1, -b2);
			if (d < 0)
			{
				d = -d;
				x = -x;
				y = -y;
				t = -t;
			}
			if (x < 0 || x > d || y < 0 || y > d)
				return;
			t /= d;
			if (!picked || t < *distance)
			{
				picked= true;
				*distance = t;
			}
		};

		m = vector3f(aabb.xmin, aabb.ymin, aabb.zmin) - p;
		pick(base[0], base[1]);
		pick(base[0], base[2]);
		pick(base[1], base[2]);
		m = vector3f(aabb.xmax, aabb.ymax, aabb.zmax) - p;
		pick(-base[0], -base[1]);
		pick(-base[0], -base[2]);
		pick(-base[1], -base[2]);
		return picked;
	}

	bool pick(const Spatial *spatial, const vector3f &p, const vector3f &dir, float *distance)
	{
		//const char *against = typeid(*spatial).name();
		switch (spatial->getBoundingVolume().getType())
		{
		case BoundingVolumeType::Sphere: return pickSphere(static_cast<const Spheref&>(spatial->getBoundingVolume()), p, dir, distance);
		case BoundingVolumeType::AABB: return pickAABB(static_cast<const AABBf&>(spatial->getBoundingVolume()), p, dir, distance);
		}
	}

	static void recursivePick(std::vector<std::pair<const Spatial*, float>> &out, const Spatial *spatial, vector3f p, vector3f dir)
	{
		// Schodzac w dol drzewa sceny transformuje *punkty* do lokalnego ukladu wspolrzednych (w ktorym znajduje sie BV)
		float dist;
		p = spatial->getParentToLocal().transformVertex(p);
		dir = spatial->getParentToLocal().transformNormal(dir);
		if (pick(spatial, p, dir, &dist))
		{
			out.push_back(std::make_pair(spatial, dist));
			for (spatial = spatial->getFirstChild(); spatial; spatial = spatial->getNextSibling())
				recursivePick(out, spatial, p, dir);
		}
	}

	std::vector<std::pair<const Spatial*, float>> pick(const Spatial *spatial, const vector3f &p, const vector3f &dir)
	{
		std::vector<std::pair<const Spatial*, float>> found;
		recursivePick(found, spatial, p, dir);
		return found;
	}

	void pick(const Camera *camera, float x, float y, vector3f *p, vector3f *dir)
	{
		float height = float(2.0 * tan(0.5 * camera->getFOV()) * camera->getNearDist());
		float width = height * camera->getRatio();
		vector3f right = cross(camera->getFront(), camera->getUp());
		vector3f nearPt = camera->getFront() * camera->getNearDist();
		nearPt += right * (x - 0.5f) * width + camera->getUp() * (0.5f - y) * height;
		matrix4f transform = camera->getWorldToLocalR();
		*p = transform.transformVertex(nearPt);
		*dir = transform.transformNormal(nearPt);
	}

	std::vector<std::pair<const Spatial*, float>> pick(const Camera *camera, float x, float y)
	{
		vector3f p, dir;
		pick(camera, x, y, &p, &dir);
		dir.normalize();
		const Spatial *root = camera;
		while (root->getParent())
			root = root->getParent();
		return pick(root, p, dir);
	}

	bool pick(const Camera *camera, float x, float y, planef plane, vector3f *point)
	{
		vector3f p0, l0, dir;
		pick(camera, x, y, &l0, &dir);
		dir.normalize();
		p0 = plane.normal * plane.d;
		if (dot(dir, plane.normal) == 0)
			return false;
		float d = dot(p0-l0, plane.normal) / dot(dir, plane.normal);
		*point = l0 + d * dir;
		return true;
	}
};