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

#include "vector3.h"

namespace Habanero
{
	/*!
	@brief Płaszyzna, wzór Ax + By + Cz + D = 0, reprezentowana jako normalna do płaszczyzny i D.
	*/
	template <typename Real>
	class plane
	{
	public:
		vector3<Real> normal;
		Real d;
		//@PRIMERDiff@ u nas jest +D zamiast -D

		plane();

		/*!
		@brief Konstruktor płaszczyzny, przekazywane wartości Ax + By + Cz + D = 0.
		*/
		plane(Real a, Real b, Real c, Real d);
		/*!
		@brief Konstruktor, przyjmuje wektor prostopadły do płaszczyzny i D.
		@param n wektor prostopadły do tworzonej płaszczyzny
		@param d wartość współczynnika D tworzonej płaszczyzny
		*/
		plane(const vector3<Real> &n, Real d);
		/*!
		@brief Konstruktor, przyjmuje wektor prostopadły do płaszczyzny i punkt.
		@param p punkt należący do tworzonej płaszczyzny
		@param n wektor prostopadły do tworzonej płaszczyzny
		*/
		plane(const vector3<Real> &p, const vector3<Real> &n);
		/*!
		@brief Konstruktor, przyjmuje 3 punkty, które mają należeć do płaszczyzny.
		*/
		plane(const vector3<Real> &p1, const vector3<Real> &p2, const vector3<Real> &p3);

		/*!
		@brief Oblicza odległość płaszczyzny od podanego puntku.
		@param p puntk, od którego będzie liczona odległość
		@result odległość podanego punktu p od płaszczyzny. Może być ujemna - to znaczy, że punkt znajduje się "pod" płaszczyzną - nie w kierunku normalnej.
		*/
		Real distance(const vector3<Real> &p) const;
		
		//inline bool safePositiveDistance(const vector3<Real> &p) const;
		
		void flip();
		
		static plane<Real> stablePlaneFromPoints(const vector3<Real> &p1, const vector3<Real> &p2, const vector3<Real> &p3);
	};

	
	typedef plane<float> planef;
	typedef plane<double> planed;
}
