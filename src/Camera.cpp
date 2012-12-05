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


#include "Memory.h"
#include "HabMath.h"
#include "Camera.h"
#include "Sphere.h"
#include "Spatial.h"

#include "line3.h"

namespace Habanero
{

	Camera::Camera(const vector3f &refPos, const vector3f &upVector, float f, float r, float nDist, float fDist) :
	Spatial(new Sphere<float>(vector3<float>::zero, 0.0f)),
	fov(f), ratio(r), nearDist(nDist), farDist(fDist)
	{
		lookAt(refPos, upVector);
	}

	void Camera::setLookAtMatrix(const vector3f& in_refPos, const vector3f& in_upVector, matrix4f& out_viewMatrix)
	{
		vector3f front = in_refPos.normalized();
		vector3f up = in_upVector.normalized();
		vector3f s = cross(front, up);
		vector3f u = cross(s, front);
		out_viewMatrix = matrix4f(
						s.x, u.x, -front.x, 0,
						s.y, u.y, -front.y, 0,
						s.z, u.z, -front.z, 0,
						0, 0, 0, 1
						);
	}
	
	void Camera::lookAt(const vector3f &refPos, const vector3f &upVector)
	{
		front = refPos.normalized();
		up = upVector.normalized();
		setLookAtMatrix(refPos, upVector, view);
	}

	matrix4f Camera::getViewMatrix() const
	{
		return getWorldToLocal() * view;
	}

	Frustum<float> Camera::getFrustum() const
	{
		float hnear, wnear;
		float hfar, wfar;

		hnear = 2 * tanf(fov / 2) * nearDist;
		wnear = hnear * ratio;

		hfar = 2 * tanf(fov / 2) * farDist;
		wfar = hfar * ratio;

		vector3f farCenter = front * farDist;
		vector3f nearCenter = front * nearDist;

		vector3f right = cross(front, up);

		//tworzę wierzchołki
		vector3f ftl = farCenter + (up * hfar / 2) - (right * wfar / 2);
		vector3f ftr = farCenter + (up * hfar / 2) + (right * wfar / 2);
		vector3f fbl = farCenter - (up * hfar / 2) - (right * wfar / 2);
		vector3f fbr = farCenter - (up * hfar / 2) + (right * wfar / 2);

		vector3f ntl = nearCenter + (up * hnear / 2) - (right * wnear / 2);
		vector3f ntr = nearCenter + (up * hnear / 2) + (right * wnear / 2);
		vector3f nbl = nearCenter - (up * hnear / 2) - (right * wnear / 2);
		vector3f nbr = nearCenter - (up * hnear / 2) + (right * wnear / 2);

		//tworzę płaszczyzny
		plane<float> nearPlane(nearCenter, front);
		plane<float> farPlane(farCenter, -front);

		plane<float> rightPlane(vector3f(0, 0, 0), cross(up, (nearCenter + right * wnear / 2).normalized()));
		plane<float> leftPlane(vector3f(0, 0, 0), cross((nearCenter - right * wnear / 2).normalized(), up));

		plane<float> topPlane(vector3f(0, 0, 0), cross((nearCenter + up * hnear / 2).normalized(), right));
		plane<float> bottomPlane(vector3f(0, 0, 0), cross(right, (nearCenter - up * hnear / 2).normalized()));

		/* tu może być komszmarnie dużo błędów, to najbardziej "ludzki" fragment
		 kodu
									  top=3
									   |                                                            
									   v                                                            
                                                                                 
							7___________________6                                               
							/:                 /|                                               
						   / :                / |                                               
						  /  :               /  |                                               
						 /   :              /   |                                               
						/__________________/    |                                               
					   4|    :            5|    |                                               
				4=left->|    :             |    |  <- right =5                                    
						|    :   far=\     |    |                                               
						|    :  rear=1     |    |                                               
						|    :.............|....|                                               
						|   . 3            |   / 2                                              
						|  .               |  /                                                 
						| .                | /                                                  
						|.  front=0=near   |/                                                   
						|__________________/                                                    
						0                  1                                                                        
								^                                                               
								|                                                               
							   bottom=2        

		 */
		std::vector < plane < float >> planes;
		std::vector < vector3f> vertices;
		std::vector<Polyhedron<float>::PolyhedronEdge> edges;


		//KOLEJNOŚĆ ŚCIAN MA ZNACZENIE!
#define pp(_plane) planes.push_back(_plane)
		pp(nearPlane);
		pp(farPlane);
		pp(bottomPlane);
		pp(topPlane);
		pp(leftPlane);
		pp(rightPlane);
#undef pp

		//KOLEJNOŚĆ WIERZCHOŁKÓW MA ZNACZENIE!
#define pv(_vert) vertices.push_back(_vert)
		pv(nbl);
		pv(nbr);
		pv(fbr);
		pv(fbl);
		pv(ntl);
		pv(ntr);
		pv(ftr);
		pv(ftl);
#undef pv

		//kolejność krawędzi nie ma znaczenia
#define pe(v1, v2, p1, p2) edges.push_back(Polyhedron<float>::PolyhedronEdge(v1, v2, p1, p2))

		pe(0, 1, 2, 0); //bottom-front
		pe(1, 2, 2, 5); //bottom-right
		pe(2, 3, 2, 1); //bottom-far
		pe(3, 0, 2, 4); //bottom-left

		pe(0, 4, 4, 0); //left-front
		pe(1, 5, 0, 5); //front-right
		pe(2, 6, 5, 1); //right-far
		pe(3, 7, 1, 4); //far-left

		pe(4, 5, 0, 3); //front-top
		pe(5, 6, 5, 3); //right-top
		pe(6, 7, 1, 3); //far-top
		pe(7, 4, 4, 3); //left-top
#undef pe

		return Frustum<float>(planes, vertices, edges, vector3f(0, 0, 0));
	}

	float Camera::getFOV() const
	{
		return fov;
	}

	float Camera::getRatio() const
	{
		return ratio;
	}

	float Camera::getNearDist() const
	{
		return nearDist;
	}

	float Camera::getFarDist() const
	{
		return farDist;
	}
	
	const vector3f& Camera::getFront() const
	{
		return front;
	}

	const vector3f& Camera::getUp() const
	{
		return up;
	}
	
	
	/*
	 wizualizuje obszar widziany przez kamerę.
	 
	 TODO: ta funkcja jest WOLNA, nie powinna być używana w buildzie.
	 Da się ją przyśpieszyć używając VBO - vertices będzie po prostu tablicą
	 wierzchołków, a edges będzie tablicą indeksów (z dwoma polami do zignorowania)
	 */
	
	void Camera::fillFrameDescription(const CullingVolumesSet& cullingVolumes, FrameDescription& frame, const SceneMembership membership) const
	{
		if (this->getVisualizeBoundingVolume() == false)
			return;

		Frustum<float> thisFrustum = this->getFrustum();
		vector3f viewPoint = thisFrustum.viewPoint;

		const std::vector<Polyhedron<float>::PolyhedronEdge>& edges = thisFrustum.getEdges();
		const std::vector<vector3f>& vertices = thisFrustum.getVertices();

		if (edges.size() == 0)
			return;
	
		line3f * lines = frame.allocArray<line3f>(edges.size());
		uint line_it = 0;
		
		for (uint i = 0; i < edges.size(); ++i)
		{
			lines[line_it++] = line3f(vertices[edges[i].num_v1], vertices[edges[i].num_v2]);
			HASSERT(line_it <= edges.size());
		}

		LinesDrawTask * task1 = new (frame) LinesDrawTask(getWorldToLocalR(), lines, edges.size());
		frame.visibleObjects.push_back(task1);
		
		vector3f * point = frame.allocArray<vector3f>(1);
		point[0] = viewPoint;
		
		PointsDrawTask * task2 = new (frame) PointsDrawTask(getWorldToLocalR(), point, 1);
		frame.visibleObjects.push_back(task2);
	}
}