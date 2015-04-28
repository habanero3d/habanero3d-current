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


/*! @file
@author qfel
*/
#pragma once
#include "default.h"
#include <cstdio>
#include "assert.h"

#define _USE_MATH_DEFINES
//#undef _MATH_DEFINES_DEFINED //WTF?
#include <cmath>

#define REDEF1(name) ALIAS1(name, name)
#define REDEF2(name) ALIAS2(name, name)
#define ALIAS1(oldname, newname) \
	inline float newname(float a1) { return ::oldname(a1); } \
	inline double newname(double a1) { return ::oldname(a1); }
#define ALIAS2(oldname, newname) \
	inline float newname(float a1, float a2) { return ::oldname(a1, a2); } \
	inline double newname(double a1, double a2) { return ::oldname(a1, a2); }

namespace Habanero
{
	const double
		pi = M_PI,
		piDiv2 = M_PI_2,
		piDiv4 = M_PI_4,
		oneDivPi = M_1_PI,
		twoDivPi = M_2_PI,
		sqrt2 = M_SQRT2,
		oneDivSqrt2 = M_SQRT1_2,
		twoDivSqrtPi = M_2_SQRTPI,
		tolerance = 0.0001;

	REDEF1(sin)
	REDEF1(cos)
	REDEF1(acos)
	REDEF1(asin)
	REDEF2(atan2)
	ALIAS1(fabs, abs)

	template <typename Real>
	inline void sincos(Real a, Real *sine, Real *cosine)
	{
		HASSERT(sine != NULL);
		HASSERT(cosine != NULL);
		*sine = sin(a);
		*cosine = cos(a);
	}

	template <typename Real>
	inline bool isAbout(Real v1, Real v2)
	{
		return std::fabs(v1 - v2) < tolerance;
	};
	

	template <typename T>
	inline T clamp(T value, T min, T max)
	{
		if (value <= min)
			return min;
		else if (value >= max)
			return max;
		else
			return value;
	}

	template <typename T>
	inline T sqr(T x)
	{
		return x * x;
	}

	template <typename T>
	inline T round(T x)
	{
		return floor(x + 0.5);
	}

}

#undef ALIAS1
#undef ALIAS2
#undef REDEF1
#undef REDEF2
