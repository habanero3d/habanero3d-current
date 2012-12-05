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

// author: akuda

#pragma once
#include "IBoundingVolume.h"
#include <vector>

namespace Habanero
{

	template <typename Real> class plane;
	template <typename Real> class Frustum;
	template <typename Real> class vector3;
	template <typename Real> class IBoundingVolume;
	class ProcessedScene;

	template <typename Real>
	class Polyhedron
	{
	public:

		struct PolyhedronEdge
		{
			uint num_v1, num_v2;
			uint num_p1, num_p2;

			PolyhedronEdge(uint _num_v1, uint _num_v2, uint _num_p1, uint _num_p2)
			: num_v1(_num_v1), num_v2(_num_v2), num_p1(_num_p1), num_p2(_num_p2)
			{
			}
		};

	protected:
		std::vector<plane<Real> > planes;
		std::vector<vector3<Real> > vertices;
		std::vector<PolyhedronEdge> edges;

		static void addConvexPointToPolyhedron(
											const std::vector<plane<Real >> &planes,
											const std::vector<vector3<Real >> &vertices,
											const std::vector<PolyhedronEdge> &edges,
											const vector3<Real> &spot,
											std::vector<plane<Real >> &new_planes,
											std::vector<vector3<Real >> &new_vertices,
											std::vector<PolyhedronEdge> &new_edges);

	private:
		static const Real error;// = 1e-3;

	public:
		Polyhedron(const Polyhedron<Real> &b);

		Polyhedron(const std::vector<plane<Real >> &_planes,
				const std::vector<vector3<Real >> &_vertices,
				const std::vector<PolyhedronEdge>& _edges);

		//Polyhedron(const Frustum<Real> &frustum, const vector3<Real> &spot);

		void addConvexPoint(const vector3<Real>& spot);
		Polyhedron<Real> getExpandedWithConvexPoint(const vector3<Real>& spot) const;

		const std::vector<plane<Real >> &getPlanes() const
		{
			return planes;
		}

		const std::vector<vector3<Real> >& getVertices() const
		{
			return vertices;
		}

		const std::vector<PolyhedronEdge>& getEdges() const
		{
			return edges;
		}


		/*! @brief Zwraca wartość logiczną, która określa, czy dana bryła okalająca znajduje się bądź przecina z frustum.
		@param ibv bryła okalająca, której zawieranie/przecinanie ma zostać sprawdzone.
		@returns true, jeśli dana bryła okalająca NIE jest zawarta/przecina się z frustum, wpp false. Możliwe fałszywe negatywy (wartość false mimo tego, iż obiekt nie znajduje/przecina się z frustum).
		 */
		bool isCulling(const IBoundingVolume<Real> &ibv) const;

		bool isPointInside(const vector3<Real> &p) const;

		/*! @brief Zwraca wartość InterferenceType, która określa, czy dana bryła okalająca zawiera się, przecina, lub mija z frustą.
		@param ibv bryła okalająca, której zawieranie/przecinanie ma zostać sprawdzone.
		@returns Contains, Collides lub NoInterference, odpowiednio jeśli zawiera, przecina się lub mija
		 */
		InterferenceType howIsCulling(const IBoundingVolume<Real> &ibv) const;

		virtual void transform(const matrix4<Real>& matrix);


		/*! @brief zwraca średnią arytmetyczną wierzchołków
		 */
		inline vector3<Real> getAveragePoint() const;

		/*! @brief Sprawdza, czy środek ciężkości bryły zawiera się w jej środku
		@returns true, jeśli tak, false wpp.
		 */
		bool checkIfAveragePointIsInside() const;

	};


	typedef Polyhedron<float> Polyhedronf;
	typedef Polyhedron<double> Polyhedrond;
}
