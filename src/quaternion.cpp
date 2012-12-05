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

namespace Habanero
{
	template <typename Real>
	const quaternion<Real> quaternion<Real>::identity(1, 0, 0, 0);
	template <typename Real>
	const quaternion<Real> quaternion<Real>::zero(0, 0, 0, 0);

	template <typename Real>
	quaternion<Real>::quaternion(Real w, Real x, Real y, Real z) :
		w(w),
		x(x),
		y(y),
		z(z)
	{
	}

	template <typename Real>
	quaternion<Real>& quaternion<Real>::normalize()
	{
		Real len = length();
		w /= len;
		x /= len;
		y /= len;
		z /= len;
		return *this;
	}

	template <typename Real>
	quaternion<Real> quaternion<Real>::normalized() const
	{
		quaternion<Real> q = *this;
		q.normalize();
		return q;
	}

	template <typename Real>
	Real quaternion<Real>::rotationAngle() const
	{
		HASSERT(abs(w) <= 1 || isAbout(w, (Real)1));
		if (w >= 1)
			return 0;
		else if (w <= -1)
			return Real(2 * pi);
		else
			return 2 * acos(w);
	}

	template <typename Real>
	vector3<Real> quaternion<Real>::rotationAxis() const
	{
		Real t = 1 - w * w;
		if (t <= 0)
			return vector3<Real>(1, 0, 0);
		t = 1 / sqrt(t);
		return vector3<Real>(x * t, y * t, z * t);
	}

	template <typename Real>
	Real quaternion<Real>::length() const
	{
		return sqrt(dot(*this, *this));
	}

	template <typename Real>
	quaternion<Real>& quaternion<Real>::operator*=(const quaternion<Real> &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template <typename Real>
	quaternion<Real> quaternion<Real>::buildRotationX(Real angle)
	{
		return quaternion<Real>((Real)cos(0.5 * angle), (Real)sin(0.5 * angle), 0, 0);
	}

	template <typename Real>
	quaternion<Real> quaternion<Real>::buildRotationY(Real angle)
	{
		return quaternion<Real>((Real)cos(0.5 * angle), 0, (Real)sin(0.5 * angle), 0);
	}

	template <typename Real>
	quaternion<Real> quaternion<Real>::buildRotationZ(Real angle)
	{
		return quaternion<Real>((Real)cos(0.5 * angle), 0, 0, (Real)sin(0.5 * angle));
	}

	template <typename Real>
	quaternion<Real> quaternion<Real>::buildRotation(const vector3<Real> &axis, Real angle)
	{
		HASSERT(isAbout<Real>(1, axis.length()));
		Real sina2 = (Real)sin(0.5 * angle);
		return quaternion<Real>((Real)cos(0.5 * angle), axis.x * sina2, axis.y * sina2, axis.z * sina2);
	}


	template <typename Real>
	quaternion<Real> quaternion<Real>::fromAnglesObjectToInertial(Real pitch, Real bank, Real heading)
	{
		//return quaternion<Real>::buildRotationZ(pitch) * quaternion<Real>::buildRotationY(bank) * quaternion<Real>::buildRotationX(heading);
//		Real		angle;
		Real		sr, sp, sy, cr, cp, cy;

		// FIXME: rescale the inputs to 1/2 angle
		sy = sin(heading / 2);
		cy = cos(heading / 2);
		sp = sin(bank / 2);
		cp = cos(bank / 2);
		sr = sin(pitch / 2);
		cr = cos(pitch / 2);

		return quaternion<Real>(cr*cp*cy+sr*sp*sy,
		sr*cp*cy-cr*sp*sy, // X
		cr*sp*cy+sr*cp*sy, // Y
		cr*cp*sy-sr*sp*cy);

		/*Real sp, sb, sh, cp, cb, ch;
		sincos(pitch, &sp, &cp);
		sincos(bank, &sb, &cb);
		sincos(heading, &sh, &ch);
		return quaternion<Real>(
			cp*cb*ch + sp*sb*sh,
			ch*sp*cb + sh*cp*sb,
			-ch*sp*sb + sh*cp*cb,
			-sh*sp*cb + ch*cp*sb);*/
	}


	template <typename Real>
	quaternion<Real> quaternion<Real>::fromAnglesInertialToObject(Real pitch, Real bank, Real heading)
	{
		Real sp, sb, sh, cp, cb, ch;
		sincos(pitch, &sp, &cp);
		sincos(bank, &sb, &cb);
		sincos(heading, &sh, &ch);
		return quaternion<Real>(
			cp*cb*ch + sp*sb*sh,
			-ch*sp*cb - sh*cp*sb,
			ch*sp*sb - sh*cp*cb,
			sh*sp*cb - ch*cp*sb);
	}

	template <typename Real>
	quaternion<Real> operator*(const quaternion<Real> &l, const quaternion<Real> &r)
	{
		// Wersja "normalna" (czyli obrot o A a potem o B to obrot o BA)
		// Tu oryginalnie byla literowka (polowa byla przepisana z normalnej wersji,
		// polowa z wersji odwrotnej
		return quaternion<Real>(
			l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z,
			l.w * r.x + l.x * r.w + l.z * r.y - l.y * r.z,
			l.w * r.y + l.y * r.w + l.x * r.z - l.z * r.x,
			l.w * r.z + l.z * r.w + l.y * r.x - l.x * r.y);
	}

	template <typename Real>
	Real dot(const quaternion<Real> &q1, const quaternion<Real> &q2)
	{
		return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
	}

	template <typename Real>
	quaternion<Real> cross(const quaternion<Real> &l, const quaternion<Real> &r)
	{
		return quaternion<Real>(
			l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z,
			l.w * r.x + l.x * r.w + l.z * r.y - l.y * r.z,
			l.w * r.y + l.y * r.w + l.x * r.z - l.z * r.x,
			l.w * r.z + l.z * r.w + l.y * r.x - l.x * r.y);
	}

	template <typename Real>
	quaternion<Real> slerp(const quaternion<Real> &q1, const quaternion<Real> &q2, Real weight)
	{
		if(weight <= 0.0)
			return q1;
		if(weight >= 1.0)
			return q2;

		Real cosAngle = dot(q1,q2);
		Real q2w = q2.w;
		Real q2x = q2.x;
		Real q2y = q2.y;
		Real q2z = q2.z;

		if(cosAngle < 0)
		{
			q2w = -q2w;
			q2x = -q2x;
			q2y = -q2y;
			q2z = -q2z;
			cosAngle = -cosAngle;
		}

		/// here, in book was a HASSERT statement, chcecking if the cosAngle is less than 1.1f
		/// my "tolerance" value could be to restritive, so I let it be.

		Real k1, k2;
		if(cosAngle > 0.9999f) /// to close for SLERP, linear interpolation
		{
			k1 = 1.f - weight;
			k2 = weight;
		}
		else /// The famous, almighty quaternion SLERP
		{
			Real sinAngle = sqrt(1.f - cosAngle*cosAngle);
			Real angle = atan2(sinAngle, cosAngle); /// interesting...

			Real oneOverSinAngle = 1.f/sinAngle;
			k1 = sin((1.f - weight) * angle) * oneOverSinAngle;
			k2 = sin(weight * angle) * oneOverSinAngle;
		}

		return quaternion<Real>(
			k1*q1.w + k2*q2.w,
			k1*q1.x + k2*q2.x,
			k1*q1.y + k2*q2.y,
			k1*q1.z + k2*q2.z);
	}

	template <typename Real>
	quaternion<Real> conjugate(const quaternion<Real> &q)
	{
		return quaternion<Real>(q.w, -q.x, -q.y, -q.z);
	}

	template <typename Real>
	quaternion<Real> pow(const quaternion<Real> &q, Real exponent)
	{
		if (abs(q.w) > 1 - tolerance)
			return q;
		Real alpha = acos(q.w),
			newAlpha = alpha * exponent,
			s, c, t;
		sincos(newAlpha, &s, &c);
		t = s / sin(alpha);
		return quaternion<Real>(c, q.x * t, q.y * t, q.z * t);
	}

	template class quaternion<float>;
	template float dot(const quaternion<float> &l, const quaternion<float> &r);
	template quaternion<float> operator*(const quaternion<float> &l, const quaternion<float> &r);
	template quaternion<float> cross(const quaternion<float> &l, const quaternion<float> &r);
	template quaternion<float> slerp(const quaternion<float> &q1, const quaternion<float> &q2, float weight);
	template quaternion<float> conjugate(const quaternion<float> &q);
	template quaternion<float> pow(const quaternion<float> &q, float exponent);

	template class quaternion<double>;
	template double dot(const quaternion<double> &l, const quaternion<double> &r);
	template quaternion<double> operator*(const quaternion<double> &l, const quaternion<double> &r);
	template quaternion<double> cross(const quaternion<double> &l, const quaternion<double> &r);
	template quaternion<double> slerp(const quaternion<double> &q1, const quaternion<double> &q2, double weight);
	template quaternion<double> conjugate(const quaternion<double> &q);
	template quaternion<double> pow(const quaternion<double> &q, double exponent);

}; ///exiting namespace Habanero;
