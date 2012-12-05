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

#include "HeightMapMesh.h"
#include "Memory.h"
#include "IBoundingVolume.h"
#include "HMF.h"

#include <cmath>

namespace Habanero
{

	HeightMapMesh::HeightMapMesh(const std::string &a_filename,
								Resource::Mapping &materials,
								Resource::Mapping &textures) :
	IResource(ResourceTypeEnum::HeightMapMesh, a_filename),
	IManagedObject(),
	materials(materials),
	textures(textures),
	boundingVolume(NULL)
	{
		geometry = NULL;
		setLoadState(IResource::LoadStateType::Discovered);
	}

	HeightMapMesh::~HeightMapMesh()
	{
		delete boundingVolume;
		unload();
	}

	GenericGeometry<StaticVertex> *HeightMapMesh::getGeometry() const
	{
		return geometry;
	}

	std::string HeightMapMesh::getTerrainTexture()
	{
		return terrainTex;
	}

	uint HeightMapMesh::getResourceSize() const
	{
		return geometry->getVertexDataSize() + geometry->getIndicesSize();
	}

	bool HeightMapMesh::load()
	{
		if (getLoadState() != IResource::LoadStateType::Discovered)
			return false;
		InitInfo initInfo = HMF2::loadFromFile(filename.c_str(), materials, textures);
		geometry = initInfo.geometry;
		width = initInfo.width;
		terrainTex = initInfo.terrainTex;
		heightToWidth = (float) (initInfo.height - 1) / (initInfo.width - 1);
		maxY = initInfo.maxY;
		boundingVolume = new AABBf(0, 0, 0, 1, maxY, (float) initInfo.height / width);
		setLoadState(IResource::LoadStateType::Loaded);
		return true;
	}

	bool HeightMapMesh::unload()
	{
		if (getLoadState() == IResource::LoadStateType::Discovered)
			return false;
		delete geometry;
		setLoadState(IResource::LoadStateType::Discovered);
		return true;
	}

	float HeightMapMesh::getHeightAt(double x, double y) const
	{
		float w = width - 1;
		uint x1 = floor(x * w), x2 = ceil(x * w);
		x = modf(x * w, &x);
		w *= heightToWidth;
		uint y1 = floor(y * w), y2 = ceil(y * w);
		y = modf(y * w, &y);
		auto at = [&](uint x, uint y){
			return geometry->getVertex(y * width + x).position.y;
		};
		if (x + y < 1)
			return (1 - y) * ((1 - x) * at(x1, y1) + x * at(x2, y1)) + y * at(x1, y2);
		else
			return (1 - y) * at(x2, y1) + y * ((1 - x) * at(x1, y2) + x * at(x2, y2));
	}
}