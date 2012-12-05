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
	template <typename Real> class matrix3;
	template <typename Real> class vector3;
	template <typename Real> class quaternion;
	template <typename Real> class plane;

	/*!
		\brief Macierz 4x4
		@tparam Real Typ liczb rzeczywistych przechowywanych w macierzy.
	*/
	template <typename Real>
	class matrix4
	{
	private:
		Real m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44;

		Real subdet(uint row, uint col) const;
	public:
		/*!
		 @brief Common matrices: identity, zero, and bias (used in ShadowMapping)
		 */
		static const matrix4<Real> identity, zero, bias; 
		

		matrix4() {};

		/*!
			@brief Buduje przekształcenie afiniczne na podstawie przekształcenia liniowego i przesunięcia
			@param matrix Macierz przekształcenia liniowego.
			@param pos Wektor przesunięcia.
		*/
		matrix4(const matrix3<Real> &matrix, const vector3<Real> &pos);

		matrix4(Real m11, Real m12, Real m13, Real m14,
			Real m21, Real m22, Real m23, Real m24,
			Real m31, Real m32, Real m33, Real m34,
			Real m41, Real m42, Real m43, Real m44);

		template <typename Real2>
		matrix4(const matrix4<Real2> &rhs)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					(*this)(i, j) = (Real)rhs(i, j);
		}

		template <typename Real2>
		matrix4(const matrix3<Real2> &rhs)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					(*this)(i, j) = (Real)rhs(i, j);
				(*this)(i, 3) = 0;
			}
			(*this)(3, 0) = (*this)(3, 1) = (*this)(3, 2) = 0;
			(*this)(3, 3) = 1;
		}

		// Funkcje modyfikujace macierz

		matrix4<Real>& transpose();
		matrix4<Real>& inverse();
		matrix4<Real>& translate(const vector3<Real> &vec);
		matrix4<Real>& translate(Real dx, Real dy, Real dz);
		matrix4<Real>& scale(Real scale);
		matrix4<Real>& scale(const vector3<Real> &axis, Real scale);
		matrix4<Real>& rotate(const quaternion<Real> &q);
		matrix4<Real>& rotate(const vector3<Real> &axis, Real angle);

		// Funkcje zwracajace zmodyfikowana macierz

		// Zwraca transponowaną kopię siebie
		matrix4<Real> transposed() const;
		// Zwraca odwróconą kopię siebie
		matrix4<Real> inversed() const;
		matrix4<Real> translated(const vector3<Real> &vector) const;
		matrix4<Real> translated(Real dx, Real dy, Real dz) const;
		matrix4<Real> scaled(Real scale) const;
		matrix4<Real> scaled(const vector3<Real> &axis, Real scale) const;
		matrix4<Real> rotated(const quaternion<Real> &q) const;
		matrix4<Real> rotated(const vector3<Real> &axis, Real angle) const;
		
		// Operatory i inne

		matrix4<Real>& operator *=(Real s);
		matrix4<Real>& operator *=(const matrix4<Real> &rhs);
		matrix4<Real>& operator +=(const matrix4<Real> &rhs);

		Real& operator()(uint row, uint col);
		Real  operator()(uint row, uint col) const;

		/*!
			@brief Aplikuje przekształcenie do wierzchołka
			@param vertex Wierzchołek
			Zwraca wynik mnożenia (vertex, 1) i this
		*/
		vector3<Real> transformVertex(const vector3<Real> &vertex) const;

		/*!
			@brief Aplikuje przekształcenie z wyzerowaną translacją do normalnej
			@param normal Normalna
			Zwraca wynik mnożenia (normal, 0) i this
		*/
		vector3<Real> transformNormal(const vector3<Real> &normal) const;

		/*!
			@brief Aplikuje przekształcenie do płaszczyzny
			@param normal Normalna
			Zwraca płaszczyznę @a p, której każdy punkt został przekształcony przez transformVertex
		*/
		plane<Real> transformPlane(const plane<Real> &p) const;

		// Funkcje statyczne tworzace specyficzne macierze

		static matrix4<Real> buildTranslation(const vector3<Real> &vec);
		static matrix4<Real> buildTranslation(Real dx, Real dy, Real dz);
		static matrix4<Real> buildScaling(Real s);
		static matrix4<Real> buildScaling(const vector3<Real> &axis, Real s);
		static matrix4<Real> buildRotation(const quaternion<Real> &q);
		static matrix4<Real> buildRotation(const vector3<Real> &axis, Real a);
		
		std::string toString() const;
	};

	template <typename Real>
	matrix4<Real> operator*(const matrix4<Real> &l, const matrix4<Real> &r);
	template <typename Real>
	matrix4<Real> operator*(Real l, matrix4<Real> r);
	template <typename Real>
	matrix4<Real> operator*(matrix4<Real> l, Real r);

	//! Mnoży macierz 4x4 przez wektor 3-wymiarowy uzupelniony o czwartą współrzędną równą 1.
	template <typename Real>
	//DEPRECATED("Tu jest obcinany wynik! Uzywaj transformVertex lub transformNormal")
	vector3<Real> operator*(const matrix4<Real> &l, const vector3<Real> &r);

	//! Mnoży wektor 3-wymiarowy uzupelniony o czwartą współrzędną równą 1 przez macierz 4x4.
	template <typename Real>
	//DEPRECATED("Tu jest obcinany wynik! Uzywaj transformVertex lub transformNormal")
	vector3<Real> operator*(const vector3<Real> &l, const matrix4<Real> &r);

	typedef matrix4<float> matrix4f;
	typedef matrix4<double> matrix4d;
	
}
