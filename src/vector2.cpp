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
#include "vector2.h"

namespace Habanero
{
	template class vector2<float>;
	template vector2<float> operator+ (const vector2<float> &l, const vector2<float> &r);
	template vector2<float> operator- (const vector2<float> &l, const vector2<float> &r);
	template vector2<float> operator* (const vector2<float> &l, float r);
	template vector2<float> operator* (float l, const vector2<float> &r);
	template float dot(const vector2<float> &a, const vector2<float> &b);

	template class vector2<double>;
	template vector2<double> operator+ (const vector2<double> &l, const vector2<double> &r);
	template vector2<double> operator- (const vector2<double> &l, const vector2<double> &r);
	template vector2<double> operator* (const vector2<double> &l, double r);
	template vector2<double> operator* (double l, const vector2<double> &r);
	template double dot(const vector2<double> &a, const vector2<double> &b);
	

	template <typename Real>
	vector2<Real>::vector2(Real x, Real y) :
		x(x),
		y(y)
	{
	};

	template <typename Real>
	vector2<Real>::vector2(const Real *v)
	{
		x = v[0];
		y = v[1];
	}

	template <typename Real>
    void vector2<Real>::normalize()
	{
		Real len = length();
		x/=len;
		y/=len;
	};

	template <typename Real>
    vector2<Real> vector2<Real>::normalized() const
	{
		vector2<Real> v = *this;
		v.normalize();
		return v;
	};

	template <typename Real>
	vector2<Real>::operator Real*()
	{
		return &x;
	}

	template <typename Real>
	vector2<Real>::operator const Real*() const
	{
		return &x;
	}

	template <typename Real>
	vector2<Real> vector2<Real>::operator- () const
	{
		return vector2<Real>(-x, -y);
	}

	template <typename Real>
	vector2<Real> vector2<Real>::operator/ (Real v) const
	{
		return vector2<Real>(x/v, y/v);
	};

	template <typename Real>
	vector2<Real>& vector2<Real>::operator+= (const vector2<Real>& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	};

	template <typename Real>
	vector2<Real>& vector2<Real>::operator-= (const vector2<Real>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	};

	template <typename Real>
	vector2<Real>& vector2<Real>::operator*= (Real v)
	{
		x *= v;
		y *= v;
		return *this;
	};

	template <typename Real>
	vector2<Real>& vector2<Real>::operator/= (Real v)
	{
		x /= v;
		y /= v;
		return *this;
	};

	template <typename Real>
	Real vector2<Real>::length() const
	{
		return sqrt(x*x + y*y);
	};

	template <typename Real>
	vector2<Real> operator+ (const vector2<Real> &l, const vector2<Real> &r)
	{
		return vector2<Real>(l.x + r.x, l.y + r.y);
	}

	template <typename Real>
    vector2<Real> operator- (const vector2<Real> &l, const vector2<Real> &r)
	{
		return vector2<Real>(l.x - r.x, l.y - r.y);
	}

	template <typename Real>
	vector2<Real> operator* (const vector2<Real> &l, Real r)
	{
		return vector2<Real>(l.x * r, l.y * r);
	}

	template <typename Real>
	vector2<Real> operator* (Real l, const vector2<Real> &r)
	{
		return r * l;
	}

	template <typename Real>
	Real dot(const vector2<Real> &l, const vector2<Real> &r)
	{
		return l.x * r.x + l.y * r.y;
	}

}; //exiting namespace Habanero;
