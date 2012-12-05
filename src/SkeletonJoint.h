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


/*! @file
@author akuda
*/

#pragma once

#include "RT.h"

namespace Habanero
{
	class SkeletonJoint
	{
	private:
		char name[32];
	public:
		struct InitInfo
		{
			uint parent;
			RTf bindPose;
		};

		static const uint noParent;// = -1; // To jest UINT_MAX

		const uint parent; //TODO: Byc moze lepiej zamienic to na wskaznik?
		const RTf bindPose;

		SkeletonJoint(const InitInfo &initInfo);

		void setName(const char * newName);
		const char* getName() const;
	};
}
