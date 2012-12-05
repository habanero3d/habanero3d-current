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


#pragma once
#include <vector>

#include "SkeletonJointKeyframeSequence.h"

namespace Habanero
{
	class SkeletonJointKeyframeSequence;
	class Skeleton;

	class SkeletalAnimation
	{		
	private:
		SkeletonJointKeyframeSequence *keyframeSequences;
		char name[32];

		static float getTotalTime(const std::vector<SkeletonJointKeyframeSequence::InitInfo> &keyframeSequences);

	public:
		struct InitInfo
		{
			std::vector<SkeletonJointKeyframeSequence::InitInfo> keyframeSequences;
		};

		const Skeleton &skeleton;
		const float totalTime;

		SkeletalAnimation(const InitInfo &initInfo, const Skeleton &owner);
		~SkeletalAnimation();

		void setName(const char * newName);
		const char *getName() const;
		uint getIndex() const;

		const SkeletonJointKeyframeSequence& getKeyframeSequence(uint index) const;
	};

}
