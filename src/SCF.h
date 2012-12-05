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
#include "HeightMapObject.h"
#include "quaternion.h"
#include "vector3.h"

namespace Habanero
{
	class Spatial;

	namespace SCF
	{

		const uint signature = '1FCS';

		struct LightData
		{
			Color color;
			float range;
		};

		struct SRT
		{
			quaternionf rotation;
			vector3f translation;
			float scale;
		};

		struct File
		{
			uint signature;
			uint childrenLen;
		};

		Spatial *loadFromFile(const char *filename);

		Spatial *getSpatial(char *&filePtr);
	}
}