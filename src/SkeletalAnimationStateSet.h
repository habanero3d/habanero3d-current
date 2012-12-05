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


/*!
@file
@author akuda, qfel
*/
#pragma once

#include "SkeletalAnimationState.h"
#include "Skeleton.h"
#include "ref.h"

namespace Habanero
{
	template <typename Real> class matrix4;

	class SkeletalAnimationStateSet
	{
	private:
		SkeletalAnimationStateSet(const SkeletalAnimationStateSet&);
		void operator =(const SkeletalAnimationStateSet&);

		SkeletalAnimationState * animationStates;
		mutable matrix4<float> * skeletonPoseBuffer;
	
	public:
		const ref<Skeleton>skeleton;

		SkeletalAnimationStateSet(ref<Skeleton> a_skeleton);
		~SkeletalAnimationStateSet();

		void animate(float deltaTime);
		SkeletalAnimationState& getAnimationState(uint index);
		SkeletalAnimationState& getAnimationState(const char *name);
		const SkeletalAnimationState& getAnimationState(const char *name) const;
		const SkeletalAnimationState& getAnimationState(uint index) const;

		/* this functin returns a final skeleton pose, composing it from
		 multiple animations in temporaryBuffer, overwriting it's previous
		 values.
		 */
		void getSkeletonPose(matrix4<float> * out_pose) const;
	};
}
