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
@author: qfel
 modifications by everyone :)
 */

#include "Memory.h"
#include "StaticMesh.h"
#include "IBoundingVolume.h"
#include "TMF.h"

namespace Habanero
{

	StaticMesh::StaticMesh(const std::string& a_filename,
						ResourceMapping &materials,
						ResourceMapping &textures) :
	IResource(ResourceTypeEnum::StaticMesh, a_filename),
	IManagedObject(),
	materials(materials),
	textures(textures)
	{
		setLoadState(IResource::LoadStateType::Discovered);
		geometry = NULL;
		boundingVolume = NULL;
	}

	StaticMesh::~StaticMesh()
	{
		//delete boundingVolume;
		unload();
	}

	GenericGeometry<StaticVertex> *StaticMesh::getGeometry() const
	{
		return geometry;
	}

	const StaticVertex& StaticMesh::getVertex(uint index) const
	{
		return geometry->getVertex(index);
	}

	uint StaticMesh::getResourceSize() const
	{
		return geometry->getVertexDataSize() + geometry->getIndicesSize();
	}

	
	const SubMesh * StaticMesh::getSubMesh(uint index) const
	{
		return geometry->getSubMesh(index);
	}

	bool StaticMesh::load()
	{
		if (getLoadState() != IResource::LoadStateType::Discovered)
			return false;
		InitInfo initInfo = TMF2::loadFromFile(filename.c_str(), materials, textures);
		boundingVolume = initInfo.boundingVolume;
		geometry = new GenericGeometry<StaticVertex > (initInfo.geometry);
		setLoadState(IResource::LoadStateType::Loaded);
		return true;
	}

	bool StaticMesh::unload()
	{
		if (getLoadState() == IResource::LoadStateType::Discovered)
			return false;
		delete geometry;
		delete boundingVolume;
		setLoadState(IResource::LoadStateType::Discovered);
		return true;
	}
	
	uint StaticMesh::getNumIndicesSum() const
	{
		return geometry->getNumIndicesSum();
	}
}