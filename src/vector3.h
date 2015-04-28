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
#include "default.h"
#include <string>

namespace Habanero
{
	template <typename Real>
	class vector3
	{
	private:
		//Kuchnia mac beztrosko przeciazac operator rzutowania
	public:
		static const vector3<Real> zero, nx, ny, nz;

		Real x, y, z;

		vector3() {}
		template <typename Real2>
		vector3(const vector3<Real2>& rhs)
		{
			x = (Real)rhs.x;
			y = (Real)rhs.y;
			z = (Real)rhs.z;
		}
		vector3(Real x, Real y, Real z);
		explicit vector3(const Real *v);

		void normalize();
		vector3<Real> normalized() const;
		Real length() const;

		vector3<Real> operator- () const;
		vector3<Real> operator/ (Real v) const;

		vector3<Real>& operator+= (const vector3<Real> &vector);
		vector3<Real>& operator-= (const vector3<Real> &vector);
		vector3<Real>& operator*= (Real v);
		vector3<Real>& operator/= (Real v);
		
		Real& operator[] (const int index);
		Real operator[] (const int index) const;
		
		bool operator< (const vector3<Real> &that) const;
		
		std::string toString() const;
	};

	typedef vector3<float> vector3f;
	typedef vector3<double> vector3d;

	template <typename Real>
	vector3<Real> operator+ (const vector3<Real> &l, const vector3<Real> &r);
	template <typename Real>
	vector3<Real> operator- (const vector3<Real> &l, const vector3<Real> &r);
	template <typename Real>
	vector3<Real> operator* (const vector3<Real> &l, Real r);
	template <typename Real>
	vector3<Real> operator* (Real l, const vector3<Real> &r);

	template <typename Real>
	Real dot(const vector3<Real> &a, const vector3<Real> &b);
	template <typename Real>
	vector3<Real> cross(const vector3<Real> &a, const vector3<Real> &b);
}
