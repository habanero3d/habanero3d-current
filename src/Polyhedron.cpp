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


#include <vector>
#include <map>

#include "Polyhedron.h"
#include "plane.h"
#include "Frustum.h"
#include "HabMath.h"

namespace Habanero
{

	template <typename Real>
	const Real Polyhedron<Real>::error = 1e-3;
	
	template <typename Real>
	Polyhedron<Real>::Polyhedron(const Polyhedron<Real> &b) :
	planes(b.planes), vertices(b.vertices), edges(b.edges)
	{
	}

	template <typename Real>
	Polyhedron<Real>::Polyhedron(const std::vector<plane<Real >> &_planes,
								const std::vector<vector3<Real >> &_vertices,
								const std::vector<PolyhedronEdge>& _edges) :
	planes(_planes), vertices(_vertices), edges(_edges)
	{
	}

	template <typename Real>
	bool Polyhedron<Real>::isCulling(const IBoundingVolume<Real> &ibv) const
	{
		for (uint i = 0; i < planes.size(); ++i)
			if (ibv.planeInterference(planes[i]) == PlaneInterferenceType::Under)
				return true;

		return false;
	}

	template <typename Real>
	bool Polyhedron<Real>::isPointInside(const vector3<Real>& p) const
	{
		// bez tutoriala nigdy bym tego nie wyhackował.
		for (uint i = 0; i < planes.size(); ++i)
			if (planes[i].distance(p) < -std::max(error, std::fabs(error * planes[i].d)))
			{
				return false;
			}

		return true;
	}

	template <typename Real>
	InterferenceType Polyhedron<Real>::howIsCulling(const IBoundingVolume<Real> &ibv) const
	{
		bool fullyContains = true;

		for (uint i = 0; i < planes.size(); ++i)
		{
			switch (ibv.planeInterference(planes[i]))
			{
				case PlaneInterferenceType::Under:
					return InterferenceType::NoInterference;
				case PlaneInterferenceType::Through:
					fullyContains = false;
				case PlaneInterferenceType::Over:
					break;
			}
		}

		if (fullyContains)
			return InterferenceType::Contains;
		else
			return InterferenceType::Collides;

	}

	template <typename Real>
	void Polyhedron<Real>::transform(const matrix4<Real>& matrix)
	{
		for (uint i = 0; i < planes.size(); ++i)
			planes[i] = matrix.transformPlane(planes[i]);

		for (uint i = 0; i < vertices.size(); ++i)
			vertices[i] = matrix.transformVertex(vertices[i]);

	}

	template <typename Real>
	vector3<Real> Polyhedron<Real>::getAveragePoint() const
	{
		vector3<Real> averagePoint(vector3<Real>::zero);

		Real scale = ((Real) 1.0) / vertices.size();
		for (uint i = 0; i < vertices.size(); ++i)
			averagePoint += vertices[i] * scale;

		return averagePoint;
	}

	template <typename Real>
	void Polyhedron<Real>::addConvexPointToPolyhedron(
													const std::vector<plane<Real >> &planes,
													const std::vector<vector3<Real >> &vertices,
													const std::vector<PolyhedronEdge> &edges,
													const vector3<Real> &spot,
													std::vector<plane<Real >> &new_planes,
													std::vector<vector3<Real >> &new_vertices,
													std::vector<PolyhedronEdge> &new_edges)
	{
		//TODO wyprofilować i zoptymalizować

		/*
		 założenia:
		 1) żaden wierzchołek się nie powtarza
		 2) żadna płaszczyzna się nie powtarza
		 */

		vector3<Real> averagePoint(vector3<Real>::zero);

		Real scale = ((Real) 1.0) / vertices.size();
		for (uint i = 0; i < vertices.size(); ++i)
			averagePoint += vertices[i] * scale;

		new_vertices.reserve(vertices.size() + 1);
		new_vertices.clear();
		new_vertices.push_back(spot);
		int num_spot = 0; // new_vertices.size() -1;

		new_edges.clear();
		new_planes.clear();

		std::vector<bool> isPlaneUseful(planes.size());
		std::vector<int> oldNewVertexMapping(vertices.size());
		std::vector<int> oldNewPlaneMapping(planes.size());

		/*
		 klucz - indeks starego wierzchołka (!= spot) nowej krawędzi
		 wartość - druga ściana która wspóldzieli nową krawędź lub pusta gdy nieznana
		 */
		std::map<int, int> oldEdgesToNewWalls;

		for (uint i = 0; i < oldNewVertexMapping.size(); ++i)
			oldNewVertexMapping[i] = -1; //unused
		for (uint i = 0; i < oldNewPlaneMapping.size(); ++i)
			oldNewPlaneMapping[i] = -1; //unused


		for (uint i = 0; i < planes.size(); ++i)
		{
			bool planeUseful = (planes[i].distance(spot) >= -std::max(error, std::fabs(error * planes[i].d)));
			isPlaneUseful[i] = planeUseful;
			if (planeUseful)
			{
				new_planes.push_back(planes[i]);
				oldNewPlaneMapping[i] = new_planes.size() - 1;
			}
		}

		for (uint i = 0; i < edges.size(); ++i)
		{
			char usefulness = 0;
			if (isPlaneUseful[edges[i].num_p1])
				usefulness++;
			if (isPlaneUseful[edges[i].num_p2])
				usefulness++;

			if (usefulness == 0)
				continue;

			/*
			 wierczhołkom mogły pozmieniać się numery, następne 10 linijek to
			 załatwia
			 */
			int new_num_v1 = oldNewVertexMapping[edges[i].num_v1];
			if (new_num_v1 == -1)
			{
				new_vertices.push_back(vertices[edges[i].num_v1]);
				new_num_v1 = new_vertices.size() - 1;
				oldNewVertexMapping[edges[i].num_v1] = new_num_v1;
			}

			int new_num_v2 = oldNewVertexMapping[edges[i].num_v2];
			if (new_num_v2 == -1)
			{
				new_vertices.push_back(vertices[edges[i].num_v2]);
				new_num_v2 = new_vertices.size() - 1;
				oldNewVertexMapping[edges[i].num_v2] = new_num_v2;
			}

			/*
			 płaszczyznom też mogły się zmienić numery, ale tu test na -1
			 (wyrzucenie złej płaszczyzny) zanużymy w podprzypadku.
			 */

			int new_num_p1 = oldNewPlaneMapping[edges[i].num_p1];
			int new_num_p2 = oldNewPlaneMapping[edges[i].num_p2];

			if (usefulness == 2) // obie płaszczyzny istnieją, super.
			{
				//PolyhedronEdge(uint _num_v1, uint _num_v2, uint _num_p1, uint _num_p2)
				new_edges.push_back(PolyhedronEdge(new_num_v1, new_num_v2, new_num_p1, new_num_p2));
			}
			else
			{ // jedna istnieje, druga nie.
				if (new_num_p1 == -1) // dla ułatwienia, zawsze druga nieistnieje
				{
					new_num_p1 = new_num_p2;
					new_num_p2 = -1;
				}

				// tworzę nową ścianę
				plane<Real> new_plane(new_vertices[new_num_v1], new_vertices[new_num_v2], spot);
				
				/*
				 plane<Real> new_plane2(plane<Real>::stablePlaneFromPoints(new_vertices[new_num_v1], new_vertices[new_num_v2], spot));
				
				NEWLOG("p1:[%f\t][%f\t][%f\t][%f\t]\np2:[%f\t][%f\t][%f\t][%f\t]",
					 new_plane.normal.x, new_plane.normal.y, new_plane.normal.z, new_plane.d,
					 new_plane2.normal.x, new_plane2.normal.y, new_plane2.normal.z, new_plane2.d);
				*/
				
				if (new_plane.distance(averagePoint) < -std::max(error, std::fabs(error * new_plane.d)))
					new_plane.flip();

				new_planes.push_back(new_plane);
				new_num_p2 = new_planes.size() - 1;

				//dodaję STARĄ krawędź

				new_edges.push_back(PolyhedronEdge(new_num_v1, new_num_v2, new_num_p1, new_num_p2));

				/*
				 próbuję dodać od 0 do 2 NOWYCH KRAWĘDZI
				 TUTAJ new_num_p1 jest już BEZUŻYTECZNE, (wskazuje starą ścianę, już załatwioną)
				 new_num_p2 wskazuje nowoutworzoną ścianę 
				 new_num_v1 i new_num_v2 wskazują stare wierzchołki
				 
				 poniższy kod działa tak:
				 dla każdego ze starych wierzchołków (new_num_v1, new_num_v2)
				 mam krawędź (stary_wierzchołek, spot) którą identyfikuję
				 za pomocą indeksu starego wierzchołka (wallKey)
				 
				 sprawdzam w mapie: jeśli druga ściana przyległa do tej krawędzi
				 już istnieje, to mogę dodać krawędź do zbioru nowych
				 (bo wreszcie znam wszystkie arg. konstruktora)
				 jeśli jeszcze nie istenieje, to zostawiam informacje o tej ścianie
				 i czekam, aż krawędź zostanie dodana przy okazji dodawania ściany
				 przyległej.
				 */
				std::map<int, int>::iterator secondWall;

				// najpierw sprawdzamy ścianę przyległą z new_num_v1
				int wallKey = new_num_v1;
				secondWall = oldEdgesToNewWalls.find(wallKey);

				if (secondWall == oldEdgesToNewWalls.end()) // nie znamy drugiej ściany dla tej krawędzi.
				{
					oldEdgesToNewWalls[wallKey] = new_num_p2; // to ją dodajemy i kiedyś do tego wrócimy
					//NEWLOG("pkt %d czeka", wallKey);
				}
				else // wiemy z którą ścianą ta krawędź będzie współdzielona
				{
					//NEWLOG("pkt %d się doczekał", secondWall->first);
					new_edges.push_back(PolyhedronEdge(new_num_v1, num_spot, new_num_p2, secondWall->second));
				}
				// teraz sprawdzamy ścianę przyległa z new_num_v2

				wallKey = new_num_v2;
				secondWall = oldEdgesToNewWalls.find(wallKey);

				if (secondWall == oldEdgesToNewWalls.end()) // nie znamy drugiej ściany dla tej krawędzi.
				{
					oldEdgesToNewWalls[wallKey] = new_num_p2; // to ją dodajemy i kiedyś do tego wrócimy
					//NEWLOG("pkt %d czeka", wallKey);
				}
				else // wiemy z którą ścianą ta krawędź będzie współdzielona
				{
					//NEWLOG("pkt %d się doczekał", secondWall->first);
					new_edges.push_back(PolyhedronEdge(new_num_v2, num_spot, new_num_p2, secondWall->second));
				}
			}
		}
	}

	template <typename Real>
	void Polyhedron<Real>::addConvexPoint(const vector3<Real>& spot)
	{
		std::vector < plane < Real >> new_planes;
		std::vector < vector3 < Real >> new_vertices;
		std::vector<PolyhedronEdge> new_edges;

		addConvexPointToPolyhedron(planes, vertices, edges, spot,
								new_planes, new_vertices, new_edges);

		planes = new_planes;
		vertices = new_vertices;
		edges = new_edges;
	}

	template <typename Real>
	Polyhedron<Real> Polyhedron<Real>::getExpandedWithConvexPoint(const vector3<Real>& spot) const
	{
		std::vector < plane < Real >> new_planes;
		std::vector < vector3 < Real >> new_vertices;
		std::vector<PolyhedronEdge> new_edges;

		addConvexPointToPolyhedron(planes, vertices, edges, spot,
								new_planes, new_vertices, new_edges);

		return Polyhedron<Real > (new_planes, new_vertices, new_edges);
	}

	template <typename Real>
	bool Polyhedron<Real>::checkIfAveragePointIsInside() const
	{
		vector3<Real> averagePoint = getAveragePoint();

		return isPointInside(averagePoint);
	}

	template class Polyhedron<float>;
	template class Polyhedron<double>;
}
