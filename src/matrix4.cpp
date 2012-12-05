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


#include "HabMath.h"
#include "vector3.h"
#include "quaternion.h"
#include "plane.h"
#include "matrix4.h"

#include <string>
#include "cstdio"

namespace Habanero
{   
	template <typename Real>
	const matrix4<Real> matrix4<Real>::identity(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	template <typename Real>
	const matrix4<Real> matrix4<Real>::zero(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
	
	template <typename Real>
	const matrix4<Real> matrix4<Real>::bias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	template <typename Real>
	matrix4<Real>::matrix4(
		Real m11, Real m12, Real m13, Real m14,
		Real m21, Real m22, Real m23, Real m24,
		Real m31, Real m32, Real m33, Real m34,
		Real m41, Real m42, Real m43, Real m44) :
		m11(m11), m12(m12), m13(m13), m14(m14),
		m21(m21), m22(m22), m23(m23), m24(m24),
		m31(m31), m32(m32), m33(m33), m34(m34),
		m41(m41), m42(m42), m43(m43), m44(m44)
	{
	}

	template <typename Real>
	matrix4<Real>::matrix4(const matrix3<Real> &linear, const vector3<Real> &pos)
	{
		HASSERT(false);
		/*for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				(*this)(i, j) = (Real)linear(i, j);
			(*this)(i, 3) = 0;
		}
		(*this)(3, 0) = pos.x;
		(*this)(3, 1) = pos.y;
		(*this)(3, 2) = pos.z;
		(*this)(3, 3) = 1;*/
	}

	template <typename Real>
	Real matrix4<Real>::subdet(uint row, uint col) const
	{
		uint ri[3] = {0, 1, 2},
			ci[3] = {0, 1, 2};
		for (uint i = 0; i < 3; i++)
			if (ri[i] >= row)
				ri[i]++;
		for (uint i = 0; i < 3; i++)
			if (ci[i] >= col)
				ci[i]++;
		const matrix4<Real> &m = *this;
		return
			m(ri[0], ci[0]) * m(ri[1], ci[1]) * m(ri[2], ci[2]) +
			m(ri[1], ci[0]) * m(ri[2], ci[1]) * m(ri[0], ci[2]) +
			m(ri[2], ci[0]) * m(ri[0], ci[1]) * m(ri[1], ci[2]) -
			m(ri[0], ci[2]) * m(ri[1], ci[1]) * m(ri[2], ci[0]) -
			m(ri[1], ci[2]) * m(ri[2], ci[1]) * m(ri[0], ci[0]) -
			m(ri[2], ci[2]) * m(ri[0], ci[1]) * m(ri[1], ci[0]);
	}

// Funkcje modyfikujace macierz

	template <typename Real>
	matrix4<Real>& matrix4<Real>::transpose()
	{
		return *this = transposed();
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::inverse()
	{
		return *this = inversed();
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::translate(const vector3<Real> &vec)
	{
		return *this = translated(vec);
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::translate(Real dx, Real dy, Real dz)
	{
		return *this = translated(dx, dy, dz);
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::scale(Real s)
	{
		for (uint i = 0; i < 4; i++)
			for (uint j = 0; j < 3; j++)
				(*this)(i, j) *= s;
		return *this;
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::scale(const vector3<Real> &axis, Real s)
	{
		return *this *= buildScaling(axis, s);
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::rotate(const quaternion<Real> &q)
	{
		return *this *= buildRotation(q);
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::rotate(const vector3<Real> &axis, Real a)
	{
		return *this *= buildRotation(axis, a);
	}

// Funkcje zwracajace zmodyfikowana macierz

	template <typename Real>
	matrix4<Real> matrix4<Real>::transposed() const
	{
		return matrix4<Real>(
			m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::inversed() const
	{
		Real det = m11 * subdet(0, 0) - m21 * subdet(1, 0) + m31 * subdet(2, 0) - m41 * subdet(3, 0);
		return matrix4<Real>(
			 subdet(0, 0) / det, -subdet(1, 0) / det,  subdet(2, 0) / det, -subdet(3, 0) / det,
			-subdet(0, 1) / det,  subdet(1, 1) / det, -subdet(2, 1) / det,  subdet(3, 1) / det,
			 subdet(0, 2) / det, -subdet(1, 2) / det,  subdet(2, 2) / det, -subdet(3, 2) / det,
			-subdet(0, 3) / det,  subdet(1, 3) / det, -subdet(2, 3) / det,  subdet(3, 3) / det);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::translated(const vector3<Real> &vec) const
	{
		return (*this) * buildTranslation(vec);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::translated(Real dx, Real dy, Real dz) const
	{
		return (*this) * buildTranslation(dx, dy, dz);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::scaled(Real s) const
	{
		matrix4<Real> m = *this;
		m.scale(s);
		return m;
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::scaled(const vector3<Real> &axis, Real s) const
	{
		return (*this) * buildScaling(axis, s);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::rotated(const quaternion<Real> &q) const
	{
		return (*this) * buildRotation(q);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::rotated(const vector3<Real> &axis, Real a) const
	{
		return (*this) * buildRotation(axis, a);
	}

// Operatory i inne
	template <typename Real>
	matrix4<Real>& matrix4<Real>::operator +=(const matrix4<Real> &rhs)
	{
		for (uint i = 0; i < 4; i++)
			for (uint j = 0; j < 4; j++)
				(*this)(i, j) += rhs(i, j);
		return *this;
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::operator *=(Real s)
	{
		for (uint i = 0; i < 4; i++)
			for (uint j = 0; j < 4; j++)
				(*this)(i, j) *= s;
		return *this;
	}

	template <typename Real>
	matrix4<Real>& matrix4<Real>::operator *=(const matrix4<Real> &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template <typename Real>
	Real& matrix4<Real>::operator()(uint row, uint col)
	{
		return *(&m11 + 4 * row + col);
	}

	template <typename Real>
	Real matrix4<Real>::operator()(uint row, uint col) const
	{
		return *(&m11 + 4 * row + col);
	}

	template <typename Real>
	matrix4<Real> operator *(Real s, matrix4<Real> m)
	{
		m *= s;
		return m;
	}

	template <typename Real>
	matrix4<Real> operator *(matrix4<Real> m, Real s)
	{
		m *= s;
		return m;
	}

	template <typename Real>
	matrix4<Real> operator *(const matrix4<Real> &l, const matrix4<Real> &r)
	{
		matrix4<Real> m;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m(i, j) = 0;
				for (int k = 0; k < 4; k++)
					m(i, j) += l(i, k) * r(k, j);
			}
		return m;
	}

	template <typename Real>
	vector3<Real> operator*(const matrix4<Real> &l, const vector3<Real> &r)
	{
		// Byc moze zbyt silne pilnowanie precyzji
		//HASSERT(l(3, 0) == 0 && l(3, 1) == 0 && l(3, 2) == 0 && l(3, 3) == 1);
		return vector3<Real>(
			l(0, 0) * r.x + l(0, 1) * r.y + l(0, 2) * r.z + l(0, 3),
			l(1, 0) * r.x + l(1, 1) * r.y + l(1, 2) * r.z + l(1, 3),
			l(2, 0) * r.x + l(2, 1) * r.y + l(2, 2) * r.z + l(2, 3));

	}

	template <typename Real>
	vector3<Real> operator*(const vector3<Real> &l, const matrix4<Real> &r)
	{
		// Byc moze zbyt silne pilnowanie precyzji
		//HASSERT(r(0, 3) == 0 && r(1, 3) == 0 && r(2, 3) == 0 && r(3, 3) == 1);
		return vector3<Real>(
			l.x * r(0, 0) + l.y * r(1, 0) + l.z * r(2, 0) + r(3, 0),
			l.x * r(0, 1) + l.y * r(1, 1) + l.z * r(2, 1) + r(3, 1),
			l.x * r(0, 2) + l.y * r(1, 2) + l.z * r(2, 2) + r(3, 2));
	}

	#pragma warning(push)
	#pragma warning(disable:4996)

	template <typename Real>
	vector3<Real> matrix4<Real>::transformVertex(const vector3<Real> &v) const
	{
		//HASSERT(m14 == 0 && m24 == 0 && m34 == 0 && m44 == 1);
		return vector3<Real>(
			v.x * m11 + v.y * m21 + v.z * m31 + m41,
			v.x * m12 + v.y * m22 + v.z * m32 + m42,
			v.x * m13 + v.y * m23 + v.z * m33 + m43
		);
	}

	template <typename Real>
	vector3<Real> matrix4<Real>::transformNormal(const vector3<Real> &v) const
	{
		//HASSERT(m14 == 0 && m24 == 0 && m34 == 0 && m44 == 1);
		return vector3<Real>(
			v.x * m11 + v.y * m21 + v.z * m31,
			v.x * m12 + v.y * m22 + v.z * m32,
			v.x * m13 + v.y * m23 + v.z * m33
		);
	}

	#pragma warning(pop)

	template <typename Real>
	plane<Real> matrix4<Real>::transformPlane(const plane<Real> &p) const
	{
		matrix4<Real> m = inversed();
		const vector3<Real> &n = p.normal;
		return plane<Real>(m * n, m(3, 0) * n.x + m(3, 1) * n.y + m(3, 2) * n.z + m(3, 3) * p.d);
		//HASSERT(!"Jeszcze nie dziala");
		//return plane<Real>();
	}

// Funkcje statyczne tworzace specyficzne macierze

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildTranslation(const vector3<Real> &vec)
	{
		return buildTranslation(vec.x, vec.y, vec.z);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildTranslation(Real dx, Real dy, Real dz)
	{
		return matrix4<Real>(
			1,  0,  0,  0,
			0,  1,  0,  0,
			0,  0,  1,  0,
			dx, dy, dz, 1);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildScaling(Real s)
	{
		return identity.scaled(s);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildScaling(const vector3<Real> &axis, Real s)
	{
		Real a = s - 1,
			ax = a * axis.x,
			ay = a * axis.y,
			az = a * axis.z;
		return matrix4<Real>(
			ax * axis.x + 1, ax * axis.y,     ax * axis.z,     0,
			ax * axis.y,     ay * axis.y + 1, ay * axis.z,     0,
			ax * axis.z,     ay * axis.z,     az * axis.z + 1, 0,
			0,               0,               0,               1);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildRotation(const quaternion<Real> &q)
	{
		// W kodzie do Primer'a (nie ksiazce) jest literowka przy m31 (po transponowaniu)
		Real ww = 2 * q.w,
			xx = 2 * q.x,
			yy = 2 * q.y,
			zz = 2 * q.z;
		// Transponowane
		/*return matrix4<Real>(
			1.0f - yy*q.y - zz*q.z, xx*q.y - ww*q.z,        xx*q.z + ww*q.y,        0,
			xx*q.y + ww*q.z,        1.0f - xx*q.x - zz*q.z, yy*q.z - ww*q.x,        0,
			xx*q.z - ww*q.y,        yy*q.z + ww*q.x,        1.0f - xx*q.x - yy*q.y, 0,
			0,                      0,                      0,                      1);*/
		return matrix4<Real>(
			1.0f - yy*q.y - zz*q.z, xx*q.y + ww*q.z,        xx*q.z - ww*q.y,        0,
			xx*q.y - ww*q.z,        1.0f - xx*q.x - zz*q.z, yy*q.z + ww*q.x,        0,
			xx*q.z + ww*q.y,        yy*q.z - ww*q.x,        1.0f - xx*q.x - yy*q.y, 0,
			0,                      0,                      0,                      1);
	}

	template <typename Real>
	matrix4<Real> matrix4<Real>::buildRotation(const vector3<Real> &axis, Real theta)
	{
		HASSERT(isAbout(axis.length(), (Real)1));
		Real s, c;
		sincos(theta, &s, &c);
		Real a = 1 - c,
			ax = a * axis.x,
			ay = a * axis.y,
			az = a * axis.z;
		// Transponowane
		/*return matrix4<Real>(
			ax*axis.x + c, ay*axis.x - axis.z*s, az*axis.x + axis.y*s, 0,
			ax*axis.y + axis.z*s, ay*axis.y + c, az*axis.y - axis.x*s, 0,
			ax*axis.z - axis.y*s, ay*axis.z + axis.x*s, az*axis.z + c, 0,
			0, 0, 0, 1);*/
		return matrix4<Real>(
			ax*axis.x + c,        ax*axis.y + axis.z*s, ax*axis.z - axis.y*s, 0,
			ay*axis.x - axis.z*s, ay*axis.y + c,        ay*axis.z + axis.x*s, 0,
			az*axis.x + axis.y*s, az*axis.y - axis.x*s, az*axis.z + c,        0,
			0,                    0,                    0,                    1);
	}
	
	template <typename Real>
	std::string matrix4<Real>::toString() const
	{
		char buffer[256];
		sprintf(buffer, "m44:\n[%f\t%f\t%f\t%f]\n[%f\t%f\t%f\t%f]\n[%f\t%f\t%f\t%f]\n[%f\t%f\t%f\t%f]",
			 m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
		return std::string(buffer);
	}

	template class matrix4<float>;
	template matrix4<float> operator *(matrix4<float> m, float s);
	template matrix4<float> operator *(float s, matrix4<float> m);
	template matrix4<float> operator *(const matrix4<float> &l, const matrix4<float> &r);
	template vector3<float> operator *(const matrix4<float> &l, const vector3<float> &r);
	template vector3<float> operator *(const vector3<float> &l, const matrix4<float> &r);

	template class matrix4<double>;
	template matrix4<double> operator *(matrix4<double> m, double s);
	template matrix4<double> operator *(double s, matrix4<double> m);
	template matrix4<double> operator *(const matrix4<double> &l, const matrix4<double> &r);
	template vector3<double> operator *(const matrix4<double> &l, const vector3<double> &r);
	template vector3<double> operator *(const vector3<double> &l, const matrix4<double> &r);
}
