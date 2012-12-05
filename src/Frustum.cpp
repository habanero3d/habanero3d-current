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


#include "Frustum.h"
#include "IBoundingVolume.h"

namespace Habanero
{

	template <typename Real>
	Frustum<Real>::Frustum(const std::vector<plane<Real >> &_planes,
					const std::vector<vector3<Real >> &_vertices,
					const std::vector<struct Polyhedron<Real>::PolyhedronEdge>& _edges,
					const vector3<Real>& _viewPoint) :
	Polyhedron<Real>(_planes, _vertices, _edges), viewPoint(viewPoint)
	{
	}

	template <typename Real>
	void Frustum<Real>::transform(const matrix4<Real>& matrix)
	{
		Polyhedron<Real>::transform(matrix);
		
		viewPoint = matrix.transformVertex(viewPoint);
	}
	

	template class Frustum<float>;
	template class Frustum<double>;
};