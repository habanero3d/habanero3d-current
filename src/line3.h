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

/* author: akuda*/

#pragma once

#include "vector3.h"

namespace Habanero
{
	//TODO: add missing methods
	template <typename Real>
	class line3
	{
	public:
		vector3<Real> p1, p2;
		line3(){};
		
		template <typename Real2>
		line3(const line3<Real2>& rhs)
		{
			p1 = (vector3<Real>)rhs.p1;
			p2 = (vector3<Real>)rhs.p2;
		}
		
		line3(vector3<Real> p1, vector3<Real> p2);
	};
	
	typedef line3<float> line3f;
	typedef line3<double> line3d;
}