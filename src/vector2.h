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

namespace Habanero
{
	template <typename Real>
	class vector2
	{
	public:
		Real x, y;

		vector2() {};
		template <typename Real2>
		vector2(const vector2<Real2>& vector)
		{
                    // tu było jakieś dziwne "rhs" zamiast vector... wtf?
			x = (Real)vector.x;
			y = (Real)vector.y;
		}
		vector2(Real v1, Real v2);
		explicit vector2(const Real *v);

		void normalize();
		vector2<Real> normalized() const;
		Real length() const;

		operator Real*();
		operator const Real*() const;

		vector2<Real> operator- () const;
		vector2<Real> operator/ (Real v) const;

		vector2<Real>& operator+= (const vector2<Real>& vector);
		vector2<Real>& operator-= (const vector2<Real>& vector);
		vector2<Real>& operator*= (Real v);
		vector2<Real>& operator/= (Real v);
	};

	typedef vector2<float> vector2f;
	typedef vector2<double> vector2d;

	template <typename Real>
	vector2<Real> operator+ (const vector2<Real> &l, const vector2<Real> &r);
	template <typename Real>
	vector2<Real> operator- (const vector2<Real> &l, const vector2<Real> &r);
	template <typename Real>
	vector2<Real> operator* (const vector2<Real> &l, Real r);
	template <typename Real>
	vector2<Real> operator* (Real l, const vector2<Real> &r);

	template <typename Real>
	Real dot(const vector2<Real> &l, const vector2<Real> &r);
}
