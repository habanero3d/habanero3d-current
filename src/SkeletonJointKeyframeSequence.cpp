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


#include "SkeletonJointKeyframeSequence.h"
#include "SkeletonJointKeyframe.h"
#include "Algorithm.h"
#include "matrix4.h"
#include "Memory.h"
#include "HabMath.h"
#include "LogManager.h"

namespace Habanero
{

	SkeletonJointKeyframeSequence::SkeletonJointKeyframeSequence(const SkeletonJointKeyframeSequence::InitInfo &initInfo) :
		numFrames(initInfo.frames.size())
	{
//		HASSERT(numFrames > 0);
		frames = uninitializedAlloc<SkeletonJointKeyframe>(initInfo.frames.size());
		for (uint i = 0; i < initInfo.frames.size(); i++)
			new(&frames[i]) SkeletonJointKeyframe(initInfo.frames[i]);
	}

	SkeletonJointKeyframeSequence::~SkeletonJointKeyframeSequence()
	{
		uninitializedDelete(frames, numFrames);
	}

	const SkeletonJointKeyframe& SkeletonJointKeyframeSequence::getKeyframe(uint index) const
	{
		HASSERT(index < numFrames);
		return frames[index];
	}

	RT<float> SkeletonJointKeyframeSequence::getInterpolatedJointPose(float time) const
	{
		HASSERT(time >= 0);
		if (numFrames == 0)
			return RT<float>(quaternionf::identity, vector3f::zero);

		class Comparer
		{
		private:
			float time;
		public:
			Comparer(float time) :time(time) {}
			Ordering operator ()(const SkeletonJointKeyframe &elem) const
			{
				if (time < elem.time)
					return Ordering::Less;
				else if (time > elem.time)
					return Ordering::Greater;
				else
					return Ordering::Equal;
			}
		};
		SkeletonJointKeyframe *left, *right = binarySearchLower(frames, frames + numFrames, Comparer(time));
		// Moze jestemy na granicy i nie ma co interpolowac
		if (right == frames)
			return right->pose;
		else if (right == frames + numFrames)
			return right[-1].pose;
		left = right - 1;
		HASSERT(left->time <= time && time <= right->time);

		float w = (time - left->time) / (right->time - left->time);
		HASSERT(0.0f <= w && w <= 1.0f);

		RT<float> result;
		result.rotation = slerp(left->pose.rotation, right->pose.rotation, w);
		//result.translation = w * left->pose.translation + (1- w) * right->pose.translation;
		
		// Interpolacja Hermita zamiast liniowej
		vector3f
			vec = right->pose.translation - left->pose.translation,
			vin = w * vec,
			vout = (1 - w) * vec;
		float
			t1 = w,
			t2 = t1 * t1,
			t3 = t1 * t2,
			h1 =  2 * t3 - 3 * t2 + 1,
			h2 = -2 * t3 + 3 * t2,
			h3 =      t3 - 2 * t2 + t1,
			h4 =      t3 -     t2;
		result.translation = h1 * left->pose.translation + h3 * vout + h2 * right->pose.translation + h4 * vin;
		return result;
	}
}