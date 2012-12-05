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
@author akuda
 */

#pragma once
#include "GenericGeometry.h"
#include "StaticVertex.h"
#include "SubMesh.h"
#include "IResource.h"
#include "Mapping.h"

namespace Habanero
{
	template <typename Real> class IBoundingVolume;

	class StaticMesh : public IResource, public IManagedObject
	{
	public:

		struct InitInfo
		{
			IBoundingVolume<float> *boundingVolume; // TODO: Brzyydkie (wskaznik zmienia wlasciciela)
			GenericGeometry<StaticVertex>::InitInfo geometry;
		};

	private:
		StaticMesh(const StaticMesh&);
		void operator =(const StaticMesh&);

		GenericGeometry<StaticVertex> *geometry;

		ResourceMapping materials, textures;

	public:
		//TODO: Prywatyzacja i getter
		IBoundingVolume<float> *boundingVolume;

		StaticMesh(const std::string &a_filename,
				Resource::Mapping &materials,
				Resource::Mapping &textures);
		~StaticMesh();

		GenericGeometry<StaticVertex> *getGeometry() const;
		uint getNumVertices() const;
		uint getNumIndicesSum() const;
		uint getNumSubMeshes() const;
		const StaticVertex& getVertex(uint index) const;
		const SubMesh * getSubMesh(uint index) const;
		virtual uint getResourceSize() const;


		bool load();
		bool unload();
	};
}
