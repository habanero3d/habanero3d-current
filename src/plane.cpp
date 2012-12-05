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


#include "plane.h"
#include "LUP.h"
#include "cstring"

namespace Habanero
{
	template <typename Real>
	plane<Real>::plane()
	{
	}
	
		
	template <typename Real>
	plane<Real>::plane(Real x, Real y, Real z, Real w) : 
		normal(x, y, z)
	{
		Real len = normal.length();
		normal /= len;
		d = w / len;
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &n, Real w) :
		normal(n)
	{
		Real len = normal.length();
		normal /= len;
		d = w / len;
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &p, const vector3<Real> &n) :
		normal(n.normalized())
	{
		d = - dot(p, normal);
	}

	template <typename Real>
	plane<Real>::plane(const vector3<Real> &p1, const vector3<Real> &p2, const vector3<Real> &p3)
	{
		/*
		vector3<Real> normal1 = cross((p2-p1), (p3-p2));
		vector3<Real> normal2 = cross((p3-p2), (p1-p3));
		
		if(normal1.length() > normal2.length())
			normal = normal1;
		else
			normal = normal2;
		*/
		
		normal = cross((p2 - p1), (p3 - p2));
		normal.normalize();
		d = - dot(p1, normal);
	}

	template <typename Real>
	Real plane<Real>::distance(const vector3<Real> &p) const
	{
		return (dot(normal, p) + d);
	}
	/*
	template <typename Real>
	bool plane<Real>::safePositiveDistance(const vector3<Real>& p) const
	{
		return (distance(p) >= -std::max(1e-3, std::fabs(1e-3 * d)));
	}*/

	template <typename Real>
	void plane<Real>::flip()
	{
		normal *= -1;
		d = -d;
	}
	
	template <typename Real>
	plane<Real> plane<Real>::stablePlaneFromPoints(const vector3<Real>& p1, const vector3<Real>& p2, const vector3<Real>& p3)
	{
		Real m[16], res[4], dist[4];
		
		memcpy(&m[0], &p1, sizeof(vector3<Real>));
		m[3] = 1.0;
		memcpy(&m[4], &p2, sizeof(vector3<Real>));
		m[7] = 1.0;
		memcpy(&m[8], &p3, sizeof(vector3<Real>));
		for(int i = 11; i < 16; ++i)
			m[i] = 1.0;
		for(int i = 0 ; i < 4; ++i)
			dist[i] = 0.0f;
		
		LUP<Real, 4> lup(m);
		lup.solve(dist, res);
		
		return plane<Real>(res[0], res[1], res[2], res[3]);
	}
	
	template class plane<float>;
	template class plane<double>;

};