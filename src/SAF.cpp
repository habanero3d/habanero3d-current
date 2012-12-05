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
#include "Exception.h"
#include "File.h"
#include "SAF.h"
#include "ref.h"

#include "SkeletonJoint.h"

namespace Habanero
{
	namespace SAF2
	{
		const SkeletonJointKeyframeSequence* SkeletonJointKeyframeSequence::getNext() const
		{
			return (SkeletonJointKeyframeSequence*)(frames + numFrames);
		}

		const SkeletonJointKeyframeSequence *SkeletalAnimation::getFirstKeyframeSequence() const
		{
			return (SkeletonJointKeyframeSequence*)this;
		}

		const SkeletalAnimation* SkeletalAnimation::getNext(uint numJoints) const
		{
			const SkeletonJointKeyframeSequence *sequence = getFirstKeyframeSequence();
			for (uint i = 0; i < numJoints; i++)
				sequence = sequence->getNext();
			return (SkeletalAnimation*)sequence;
		}

		const SkeletonJoint *Skeleton::getJoints() const
		{
			return (SkeletonJoint*)(this + 1);
		}

		const SkeletalAnimation *Skeleton::getFirstAnimation() const
		{
			return (SkeletalAnimation*)(getJoints() + numJoints);
		}

		static void loadJoints(const Skeleton &skel, std::vector<Habanero::SkeletonJoint::InitInfo> &joints)
		{
			// Wydaje sie ze SkeletonJoint w silniku jest reprezentowany tak samo jak w pliku (po ew. zmianie kolejnosci pol), ale to w
			// dalszym ciagu po stronie silnika nie jest POD (private i public), wiec formalnie nie wiemy jaka jest kolejnosc pol
			Habanero::SkeletonJoint::InitInfo joint;
			joints.reserve(skel.numJoints);
			for (uint i = 0; i < skel.numJoints; i++)
			{
				const SkeletonJoint &fileJoint = skel.getJoints()[i];
				joint.bindPose = fileJoint.bindPose;
				joint.parent = fileJoint.parentIndex;
				joints.push_back(joint);
			}
		}

		static void loadKeyframeSequences(const SkeletalAnimation &anim,
			std::vector<Habanero::SkeletonJointKeyframeSequence::InitInfo> &seqs,
			uint numJoints)
		{
			seqs.resize(numJoints);
			const SkeletonJointKeyframeSequence *fileSeq = anim.getFirstKeyframeSequence();
			for (uint j = 0; j < numJoints; j++, fileSeq = fileSeq->getNext())
			{
				Habanero::SkeletonJointKeyframeSequence::InitInfo &seq = seqs[j];
				seq.frames.resize(fileSeq->numFrames);
				std::copy(fileSeq->frames, fileSeq->frames + fileSeq->numFrames, seq.frames.begin());
			}
		}

		static void loadAnimations(const Skeleton &skel, std::vector<Habanero::SkeletalAnimation::InitInfo> &animations)
		{
			animations.resize(skel.numAnimations);
			const SkeletalAnimation *fileAnim = skel.getFirstAnimation();
			for (uint i = 0; i < skel.numAnimations; i++, fileAnim = fileAnim->getNext(skel.numJoints))
			{
				Habanero::SkeletalAnimation::InitInfo &animation = animations[i];
				loadKeyframeSequences(*fileAnim, animation.keyframeSequences, skel.numJoints);
			}
		}

		Habanero::Skeleton::InitInfo loadFromFile(const char *fileName)
		{
			uint64 size;
                        /* Kuflowe obejście buga gcc - akuda */
			std::unique_ptr<const File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid SAF signature");
			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");

			Habanero::Skeleton::InitInfo skeleton;
			loadJoints(file->skeleton, skeleton.joints);
			loadAnimations(file->skeleton, skeleton.animations);
			return skeleton;
		}
	}
}