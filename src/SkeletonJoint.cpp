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
#include <string.h>

#include "SkeletonJoint.h"
#include "StringUtil.h"
#include "Exception.h"

namespace Habanero
{
    const uint SkeletonJoint::noParent = -1;

	SkeletonJoint::SkeletonJoint(const SkeletonJoint::InitInfo &initInfo) :
		parent(initInfo.parent),
		bindPose(initInfo.bindPose)
	{
	}
	
	const char* SkeletonJoint::getName() const
	{
		return name;
	}

	void SkeletonJoint::setName(const char * newName)
	{
		HASSERT(newName != NULL);

		if (strlen(newName) >= sizeof(name))
			RAISE(Exception, format("SkeletonJoint name too long: '%s'", newName));
		strcpy(name, newName);
	}
}