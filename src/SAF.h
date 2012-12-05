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
@brief Skeleton and Animation Format
@author qfel
*/
#pragma once
#include "Skeleton.h"
#include "SkeletonJointKeyframe.h"
#include "ref.h"

// Zeby nie narzekal na T array[0]
#pragma warning(push)
#pragma warning(disable:4200)
namespace Habanero
{
	class ResourceManager;

	namespace SAF2
	{
		const uint signature = '2FAS',
		           maxStrLen = 32;

		struct SkeletonJoint
		{
			uint parentIndex;
			RTf bindPose;
		};

		struct SkeletonJointKeyframeSequence
		{
			uint numFrames;
			SkeletonJointKeyframe frames[0/*NumFrames*/];

			const SkeletonJointKeyframeSequence *getNext() const;
		};

		struct SkeletalAnimation
		{
			//uint numKeyframeSequences;
			//SkeletonJointKeyframeSequence keyframeSequences[0/*NumKeyframeSequences*/]; // NumKeyFrameSequences == NumJoints
			const SkeletonJointKeyframeSequence *getFirstKeyframeSequence() const;
			const SkeletalAnimation* getNext(uint numJoints) const;
		};

		struct Skeleton
		{
			uint numJoints,
			     numAnimations,
				 skeletonId;
			//SkeletonJoint Joints[NumJoints];
			//SkeletalAnimation Animations[NumAnimations];
			const SkeletonJoint *getJoints() const;
			const SkeletalAnimation *getFirstAnimation() const;
		};

		struct File
		{
			uint signature;
			Skeleton skeleton;
		};

		Habanero::Skeleton::InitInfo loadFromFile(const char *fileName);
	}
}
#pragma warning(pop)