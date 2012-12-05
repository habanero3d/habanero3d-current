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


/*!
@file
@brief Wspólna postać dla obiektów geometrycznych.
@author qfel
 */
#pragma once
#include <vector>
#include "SubMesh.h"
#include "StaticVertex.h"
#include "GeometryMemorySetting.h"
#include "SkinVertex.h"

namespace Habanero
{
	class GenericGeometryBase
	{
	public:
		GeometryMemorySetting MemorySetting;
	private:
		const uint numVertices;
		SubMesh **subMeshes; // array of pointers to subMesh
	protected:

		struct InitInfo
		{
			std::vector<SubMesh::InitInfo> subMeshes;
		};

		void * vertexData; // TODO: Czy to powinny byc float'y? (na fixed chyba tylko kolor nie jest)
		size_t vertexDataSize;
		uint numSubMeshes;

		GenericGeometryBase(const GenericGeometryBase::InitInfo &initInfo, const void * a_vertexData, size_t a_vertexDataSize, const uint a_numVertices);

	public:
		~GenericGeometryBase();

		const void *getVertexData() const;
		/*!@brief
		 returns vertex data size in bytes.
		 */
		size_t getVertexDataSize() const;
		uint getNumSubMeshes() const;
		
		virtual bool isSkinned() const = 0;
		
		/*!@brief
		 returns sum of numbers of indices in all sub-meshes
		 */
		uint getNumIndicesSum() const;
		
		/*!@brief
		 returns size of all indices data in all sub-meshes, in bytes. NOTE:
		 in current implementation, no single array with all indices exists.
		 This is a TODO/BUG
		 */
		size_t getIndicesSize() const;
		
		uint getNumVertices() const;
		SubMesh * getSubMesh(uint index) const;
	};

	template <typename T>
	class GenericGeometry : public GenericGeometryBase
	{
	private:

	public:

		struct InitInfo : public GenericGeometryBase::InitInfo
		{
			std::vector<T> vertices;
		};

		GenericGeometry(const InitInfo &initInfo) : GenericGeometryBase(initInfo, &initInfo.vertices[0], initInfo.vertices.size() * sizeof (T), initInfo.vertices.size())
		{
		}

		const T& getVertex(uint index) const
		{
			HASSERT( (uint64)((T*)vertexData + index) < (uint64)((byte*) vertexData + vertexDataSize) );
			return *((T*) vertexData + index);
		}

		const T* getVertexData() const
		{
			return (T*) GenericGeometryBase::getVertexData();
		}
		
		// TODO: this is the worst RTTI quickfix ever
		bool isSkinned() const
		{
			return sizeof(T) == sizeof(SkinVertex);
		}
	};
}
