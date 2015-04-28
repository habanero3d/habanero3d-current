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
@brief Stan pojedynczej animacji
@author akuda, qfel
*/
#include "LogManager.h"
#include "matrix4.h"
#include "Skeleton.h"
#include "SkeletalAnimation.h"
#include "SkeletalAnimationState.h"
#include "SkeletonJointKeyframeSequence.h"

namespace Habanero
{
	SkeletalAnimationState::SkeletalAnimationState(const SkeletalAnimation &animation) :
		animation(animation),
		time(0),
		weight(0),
		loop(false),
		autoRewind(false)
	{
	}

	void SkeletalAnimationState::getSkeletonPose(matrix4<float> *pose) const
	{
		for(uint j = 0, i; j < animation.skeleton.numJoints; j++)
		{
			i = animation.skeleton.getJointTopologicalIndex(j);
			const SkeletonJoint &joint = animation.skeleton.getJoint(i);
			RT<float> desc = animation.getKeyframeSequence(i).getInterpolatedJointPose(time);
			pose[i] = matrix4<float>::identity;
			pose[i].
				rotate(desc.rotation).
				translate(desc.translation).
				rotate(joint.bindPose.rotation).
				translate(joint.bindPose.translation);
			if (joint.parent != SkeletonJoint::noParent)
				pose[i] *= pose[joint.parent];
		}
	}

	void SkeletalAnimationState::correctTime()
	{
		if (time <= animation.totalTime)
			return;
		if (loop)
		{
			do
				time -= animation.totalTime;
			while (time > animation.totalTime);
		}
		else if (autoRewind)
		{
			time = 0;
			weight = 0;
		}
		else
			time = animation.totalTime;
	}

	float SkeletalAnimationState::getTimePosition() const
	{
		return time;
	}

	void SkeletalAnimationState::setTimePosition(float newTime)
	{
		HASSERT(newTime >= 0);
		time = newTime;
		correctTime();
	}

	void SkeletalAnimationState::animate(float deltaTime)
	{
		HASSERT(deltaTime >= 0);
		time += deltaTime;
		correctTime();
	}

	void SkeletalAnimationState::start(float weight)
	{
		this->weight = weight;
		setTimePosition(0);
	}
}