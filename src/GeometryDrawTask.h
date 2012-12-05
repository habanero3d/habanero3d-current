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

#include "matrix4.h"
#include "Enum.h"
#include "StackAllocator.h"
#include "StaticVertex.h"
#include "GenericGeometry.h"

#include "line3.h"

namespace Habanero
{
	struct SkinVertex;

	class GeometryDrawTask : public IStackAllocatedUnit
	{
	public:
		ENUM(GeometryType,
			StaticMesh,
			SkinnedMesh,
			Lines,
			Points);

	public:
		const matrix4<float> transformation;
		const GeometryType type;

	protected:

		GeometryDrawTask(GeometryType a_type,
						matrix4f a_transformation) :
		type(a_type),
		transformation(a_transformation)
		{
		}

	public:

		virtual ~GeometryDrawTask()
		{
		}

	};

	class LinesDrawTask : public GeometryDrawTask
	{
	private:
		line3f * lines;
		uint linesNum;
		Color color;
	public:

		/*! @brief takes over in_lines pointed memory!
		 */
		LinesDrawTask(const matrix4f &a_transformation,
					line3f * in_lines,
					uint a_linesNum,
					const Color &a_color = Color::Black) :
		GeometryDrawTask(GeometryType::Lines, a_transformation),
		lines(in_lines),
		linesNum(a_linesNum),
		color(a_color)
		{
		}

		const line3f * getLinesPtr() const
		{
			return lines;
		}

		const uint getLinesNum() const
		{
			return linesNum;
		}

		const Color& getColor() const
		{
			return color;
		}

	};

	class PointsDrawTask : public GeometryDrawTask
	{
	private:
		vector3f * points;
		uint pointsNum;
		Color color;
	public:

		/*! @brief takes over in_points pointed memory!
		 */
		PointsDrawTask(const matrix4f &a_transformation,
					vector3f * in_points,
					uint a_pointsNum,
					const Color &a_color = Color::Black) :
		GeometryDrawTask(GeometryType::Points, a_transformation),
		points(in_points),
		pointsNum(a_pointsNum),
		color(a_color)
		{
		}

		const vector3f * getPointsPtr() const
		{
			return points;
		}

		const uint getPointsNum() const
		{
			return pointsNum;
		}

		const Color& getColor() const
		{
			return color;
		}
	};

	class StaticObjectDrawTask : public GeometryDrawTask
	{
	private:
		GenericGeometry<StaticVertex> * geometry;
	public:

		StaticObjectDrawTask(const matrix4f &a_transformation,
							GenericGeometry<StaticVertex> * a_geometry) :
		GeometryDrawTask(GeometryType::StaticMesh, a_transformation),
		geometry(a_geometry)
		{
		}

		GenericGeometry<StaticVertex> * getGeometry() const
		{
			return geometry;
		}

		virtual ~StaticObjectDrawTask()
		{
		}


	};

	class SkinnedObjectDrawTask : public GeometryDrawTask
	{
	private:
		GenericGeometry<SkinVertex> ** geometries;
		matrix4f * matrixPalette;
		uint numJoints;
		uint numMeshes;
	public:

		SkinnedObjectDrawTask(const matrix4f &a_transformation,
							uint a_numMeshes,
							uint a_numJoints,
							StackAllocator * a_stack
							) :
		GeometryDrawTask(GeometryType::SkinnedMesh, a_transformation),
		numJoints(a_numJoints),
		numMeshes(a_numMeshes),
		geometries(a_stack->allocArray<GenericGeometry<SkinVertex> *>(a_numMeshes)),
		matrixPalette(a_stack->allocArray<matrix4f>(a_numJoints))
		{
		}

		virtual ~SkinnedObjectDrawTask()
		{
		}

		matrix4f * getMatrixPalettePtr() const
		{
			return matrixPalette;
		}

		uint getNumJoints() const
		{
			return numJoints;
		}

		uint getNumMeshes() const
		{
			return numMeshes;
		}

		void setGeometryAt(uint index, GenericGeometry<SkinVertex> * geometry)
		{
			geometries[index] = geometry;
		}

		GenericGeometry<SkinVertex> * getGeometryAt(uint index) const
		{
			return geometries[index];
		}
	};
}
