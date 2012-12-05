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


/* @file
@author qfel
*/
#pragma once

namespace Habanero
{
	template <typename Real> class vector3;

	/*!
		\brief Kwaternion
		@tparam Real Typ liczb rzeczywistych reprezentujacych kwaternion.
		Standardowa definicja (czyli nieco inna niz w Primer - tam jest nieco inne mnozenie, zeby kolejnosc byla intuicyjna).
		Poniewaz uzywamy OpenGL i macierze aplikujemy od konca, to aplikowanie kwaternionow od konca bedzie bardziej zgodne.
	*/
	template <typename Real>
	class quaternion
	{
	public:
		static const quaternion<Real> identity, zero;
		Real w, x, y, z;

		quaternion() {}
		template <typename Real2>
		quaternion(const quaternion<Real2> &r)
		{
			w = (Real)r.w;
			x = (Real)r.x;
			y = (Real)r.y;
			z = (Real)r.z;
		}
		quaternion(Real w, Real x, Real y, Real z);

		quaternion<Real>& normalize();

		quaternion<Real> normalized() const;
		vector3<Real> rotationAxis() const;
		Real rotationAngle() const;
		Real length() const;

		quaternion<Real>& operator*=(const quaternion<Real> &rhs);

		static quaternion<Real> buildRotationX(Real angle);
		static quaternion<Real> buildRotationY(Real angle);
		static quaternion<Real> buildRotationZ(Real angle);
		static quaternion<Real> buildRotation(const vector3<Real> &axis, Real angle);

		static quaternion<Real> fromAnglesObjectToInertial(Real pitch, Real bank, Real heading);
		static quaternion<Real> fromAnglesInertialToObject(Real pitch, Real bank, Real heading);
	};

	typedef quaternion<float> quaternionf;
	typedef quaternion<double> quaterniond;

	template <typename Real>
	quaternion<Real> operator*(const quaternion<Real> &l, const quaternion<Real> &r);
	template <typename Real>
	Real dot(const quaternion<Real> &l, const quaternion<Real> &r);
	template <typename Real>
	quaternion<Real> cross(const quaternion<Real> &l, const quaternion<Real> &r);
	template <typename Real>
	quaternion<Real> slerp(const quaternion<Real> &q1, const quaternion<Real> &q2, Real weight);
	template <typename Real>
	quaternion<Real> conjugate(const quaternion<Real> &q);
	template <typename Real>
	quaternion<Real> pow(const quaternion<Real> &q, Real exponent);
}
