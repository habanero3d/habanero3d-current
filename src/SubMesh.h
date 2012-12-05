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


/*
Author: Andrzej "akuda" Skalski
*/

#pragma once
#include <vector>

#include "Material.h"
#include "Texture.h"
#include "Memory.h"

namespace Habanero
{
	class SubMesh
	{
	private:
		SubMesh(const SubMesh&);
		void operator =(const SubMesh&);

	protected:
		SubMesh();
		
		uint numIndices;
		uint * indices;

		// materiały nie są zarządzane, tak więc shared_ptr
		std::shared_ptr<Material> material;

	public:
		struct InitInfo
		{
			std::shared_ptr<Material> material;
			std::vector<uint> indices;
		};
		
		uint getNumIndices() const;
		std::shared_ptr<Material> getMaterial() const;

		SubMesh(const InitInfo &initInfo);
		virtual ~SubMesh();

		uint getIndex(uint index) const;
		uint * getIndices() const;
	};
}
