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
#include <algorithm>
#include <string.h>

#include "SkeletonJointKeyframeSequence.h"
#include "SkeletonJointKeyframe.h"
#include "SkeletalAnimation.h"
#include "StringUtil.h"
#include "Exception.h"
#include "Skeleton.h"
#include "Memory.h"
#include "RT.h"

namespace Habanero
{
	SkeletalAnimation::SkeletalAnimation(const InitInfo &initInfo, const Skeleton &owner) :
		skeleton(owner),
		totalTime(getTotalTime(initInfo.keyframeSequences))
	{
		HASSERT(initInfo.keyframeSequences.size() == owner.numJoints);
		
		keyframeSequences = uninitializedAlloc<SkeletonJointKeyframeSequence>(initInfo.keyframeSequences.size());
		for (uint i = 0; i < initInfo.keyframeSequences.size(); i++)
			new(&keyframeSequences[i]) SkeletonJointKeyframeSequence(initInfo.keyframeSequences[i]);
	}

	SkeletalAnimation::~SkeletalAnimation()
	{
		uninitializedDelete(keyframeSequences, skeleton.numJoints);
	}

	float SkeletalAnimation::getTotalTime(const std::vector<SkeletonJointKeyframeSequence::InitInfo> &keyframeSequences)
	{
		float time = 0.0f;
		for (std::vector<SkeletonJointKeyframeSequence::InitInfo>::const_iterator it = keyframeSequences.begin(); it != keyframeSequences.end(); it++)
			if(it->frames.size() != 0)
				time = std::max(time, it->frames.back().time);
		return time;
	}

	void SkeletalAnimation::setName(const char * newName)
	{
		if (strlen(newName) >= sizeof(name))
			RAISE(Exception, format("Skeletal animation name too long: '%s'", newName));
		strcpy(name, newName);
	}

	const char* SkeletalAnimation::getName() const
	{
		return name;
	}

	uint SkeletalAnimation::getIndex() const
	{
		return skeleton.getAnimationIndex(*this);
	}

	const SkeletonJointKeyframeSequence& SkeletalAnimation::getKeyframeSequence(uint index) const
	{
		HASSERT(index < skeleton.numJoints);
		return keyframeSequences[index];
	}
}