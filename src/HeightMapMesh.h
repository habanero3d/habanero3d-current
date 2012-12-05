/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Bia�ecki, Micha� �ochowski, and Micha� Szczepaniak
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
@author: michauzo
*/

#pragma once
#include "Mapping.h"
#include "GenericGeometry.h"
#include "StaticVertex.h"
#include "IResource.h"
#include "AABB.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class HeightMapMesh : public IResource, public IManagedObject
	{
	public:
		struct InitInfo
		{
			GenericGeometry<StaticVertex> * geometry;
			uint width;
			std::string terrainTex;
			float height, maxY;
		};

	private:
		HeightMapMesh(const HeightMapMesh&);
		void operator =(const HeightMapMesh&);

		GenericGeometry<StaticVertex> *geometry;
		std::string terrainTex;
		Resource::Mapping materials, textures;
		uint width;
		float heightToWidth, maxY;

	public:
		//TODO: Prywatyzacja i gettery
		IBoundingVolume<float> *boundingVolume;

		HeightMapMesh(const std::string &a_filename, 
					Resource::Mapping &materials, 
					Resource::Mapping &textures);
		~HeightMapMesh();

		GenericGeometry<StaticVertex> *getGeometry() const;
		float getHeightAt(double x, double y) const;
		std::string getTerrainTexture();

		virtual uint getResourceSize() const;

		
		bool load();
		bool unload();
	};
}