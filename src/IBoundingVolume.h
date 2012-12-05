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
#include <memory>

#include "Enum.h"
#include "matrix4.h"

// TODO growToContain dla IBV w różnych spatialach

namespace Habanero
{
	template <typename Real> class vector3;
	template <typename Real> class plane;

	class FrameDescription;

	/*! @brief Enumerator - typ bryły okalającej.
	 */
	ENUMT(BoundingVolumeType, byte, AABB = 1, /*OBB = 2 is obsolete, */ Sphere = 3);
	/*! @brief Enumerator - typ przecinania brył okalających.
	 */
	ENUM(InterferenceType, Contains, Collides, NoInterference);
	/*! @brief Enumerator - typ przecinania brył okalających z płaszczyznami.
	 */
	ENUM(PlaneInterferenceType, Over, Under, Through);

	/*! @brief Interfejs Bounding Volume - bryły okalającej obiekt.
	 */
	template <typename Real>
	class IBoundingVolume
	{
	public:

		virtual ~IBoundingVolume()
		{
		};

		/*! @brief Zwraca typ przecinania obecnej bryły okalającej z podaną płaszczyzną.
		@param p płaszczyzna, z którą ma być wykonany test przecinania.
		@returns typ przecinania bryły okalającej z podaną płaszczyzną.
		 */
		virtual PlaneInterferenceType planeInterference(const plane<Real> &p) const = 0;

		/*! @brief Zeruje obiętość bryły okalającej - obecnie środek bryły okalającej nie ulega przemieszczeniu
		 */
		virtual void zero() = 0;

		/*! @brief Zwraza wartość logiczną sygnalizującą, czy podany punkt jest zawarty w obecnej bryle okalającej.
		@param p punkt, którego zawieranie ma zostać sprawdzone
		@returns true, jeśli podany punkt jest zawarty, w przeciwnym przypadku false.
		 */
		virtual bool contains(const vector3<Real> &p) const = 0;
		/*! @brief Zwraca wartość logiczną sygnalizującą, czy podana bryła okalająca jest zawarta w obecnej.
		@param bbox bryła okalająca, której zawieranie ma zostać sprawdzone.
		@returns true, jeśli podana bryła okalająca jest w całości zawarta w obecnej, w przeciwnym przypadku false.
		 */
		virtual bool contains(const IBoundingVolume<Real> &bbox) const = 0;
		/*! @brief Zwraca wartość logiczną sygnalizującą, czy podana bryła okalająca przecina się z obecną.
		@param bbox bryła okalająca, której przecinanie ma zostać sprawdzone.
		@returns true, jeśli podana bryła okalająca przecina się z obecną, w przeciwnym przypadku false.
		 */
		virtual bool collides(const IBoundingVolume<Real> &bbox) const = 0;
		/*! @brief Zwraca typ przecinania się dwóch brył okalających.
		@param bbox bryła okalająca, której relacja z obecną bryłą ma zostać sprawdzona.
		@returns Contains, jeśli dana bryła jest zawarta w obecnej, Collides jeśli się przecinają, w przeciwnym przypadku NoInterference.
		 */
		virtual InterferenceType interferes(const IBoundingVolume<Real> &bbox) const = 0; /// jeśli A.interferes(B) == Contains, to A zawiera B
		/*! @brief Rośnie, by obecna bryła okalająca zaczęła zawierać podaną.
		Obecna bryła okalająca rośnie w taki sposób, by wszystko co było zawierane do tej pory dalej było w niej zawarte.
		@param bbox bryła okalająca, która ma być zawierana przez obecną.
		@returns true, jeśli obecna bryła okalająca musiała zwiększyć swoje rozmiary, w przeciwnym przypadku false.
		 */
		virtual bool growToContain(const IBoundingVolume<Real> &bbox) = 0;

		/*! @brief Rośnie, by obecna bryła okalająca zaczęła zawierać podaną przekształoną o macierz transformacji.
		Obecna bryła okalająca rośnie w taki sposób, by wszystko co było zawierane do tej pory dalej było w niej zawarte.
		@param bbox bryła okalająca, która ma być zawierana przez obecną.
		@param transformation transformacja, przez którą będzie przekształcony bbox.
		@returns true, jeśli obecna bryła okalająca musiała zwiększyć swoje rozmiary, w przeciwnym przypadku false.
		 */
		virtual bool growToContain(const IBoundingVolume<Real> &bbox, const matrix4<Real> &transformation) = 0;

		/*! @brief Zwraca typ obecnej bryły okalającej.
		 */
		virtual BoundingVolumeType getType() const = 0;

		/*! @brief Klonuje obecną bryłę okalającą i zwraca wskaźnik do klona.
		 */
		virtual IBoundingVolume<Real> * clone() const = 0;

		/*! @brief Umieszcza visualizer BV w opisie sceny
		 */
		virtual void insertVisualizer(const matrix4<Real>& transformation, FrameDescription &frame) const = 0;

	};

	typedef IBoundingVolume<float> IBoundingVolumef;
	typedef IBoundingVolume<double> IBoundingVolumed;
}
