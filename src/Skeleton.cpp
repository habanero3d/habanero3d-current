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
@author akuda, qfel
 */
#include <algorithm>
#include <string.h>

#include "Memory.h"
#include "matrix4.h"
#include "vector3.h"
#include "Skeleton.h"
#include "StringUtil.h"
#include "Exception.h"
#include "Algorithm.h"

#include "SAF.h"
#include "File.h"


#include <set>
namespace Habanero
{

	template <typename T>
	class NameFieldComparer
	{
	private:
		const char *name;
	public:

		NameFieldComparer(const char *name) : name(name)
		{
		}

		Ordering operator()(const T &elem) const
		{
			int cmp = strcmp(name, elem.getName());
			if (cmp < 0)
				return Ordering::Less;
			else if (cmp > 0)
				return Ordering::Greater;
			else
				return Ordering::Equal;
		}
	};

	/*template <typename T>
	static bool lessByName(const typename T::InitInfo *l, const typename T::InitInfo *r)
	{
		return strcmp(l->name, r->name) < 0;
	}*/

	Skeleton::Skeleton(const std::string& a_filename) :
	IResource(ResourceTypeEnum::Skeleton, a_filename),
	IManagedObject(),
	numAnimations(0),
	numJoints(0)
	{
		setLoadState(LoadStateType::Discovered);
		topologicalIndices = NULL;
		this->joints = NULL;
		this->animations = NULL;
		this->reversePose = NULL;
	}

	Skeleton::~Skeleton()
	{
		/*delete[] topologicalIndices;
		delete[] reversePose;
		uninitializedDelete(joints, numJoints);
		uninitializedDelete(animations, numAnimations);*/
		unload();
	}

	void Skeleton::preprocessJoints()
	{
		reversePose = new matrix4<float>[numJoints];
		for (uint j = 0, i; j < numJoints; j++)
		{
			i = getJointTopologicalIndex(j);
			const SkeletonJoint &joint = getJoint(i);
			reversePose[i] = matrix4<float>::identity;
			reversePose[i].
				rotate(joint.bindPose.rotation).
				translate(joint.bindPose.translation);
			if (joint.parent != SkeletonJoint::noParent)
				reversePose[i] *= reversePose[joint.parent];
		}
		for (uint j = 0; j < numJoints; j++)
			reversePose[j].inverse();
	}

	void Skeleton::initJoints(const std::vector<SkeletonJoint::InitInfo> &elems)
	{
		/*std::vector<const SkeletonJoint::InitInfo*> sortedElems;
		sortedElems.reserve(elems.size());
		for (uint i = 0; i < elems.size(); i++)
			sortedElems.push_back(&elems[i]);
		std::sort(sortedElems.begin(), sortedElems.end(), lessByName<SkeletonJoint>);
		animations = uninitializedAlloc<T>(sortedElems.size());
		for (uint i = 0; i < sortedElems.size(); i++)
			new (&joints[i]) SkeletonJoint(*sortedElems[i], *this);*/

		std::vector<byte> visited(elems.size(), 0);
		topologicalIndices = new uint[elems.size()];
		for (uint i = 0, k = 0, j, first; i < elems.size(); i++)
			if (!visited[i])
			{
				first = k;
				j = i;
				do
				{
					visited[j] = 1;
					topologicalIndices[k++] = j;
					j = elems[j].parent;
				}
				while (j != SkeletonJoint::noParent && !visited[j]);
				std::reverse(topologicalIndices + first, topologicalIndices + k);
			}

		//TODO: Wywalic (debug)
#ifndef NDEBUG
		std::set<uint> used;
		used.insert(SkeletonJoint::noParent);
		for (uint i = 0; i < elems.size(); i++)
		{
			HASSERT(used.find(elems[topologicalIndices[i]].parent) != used.end());
			used.insert(topologicalIndices[i]);
		}
#endif

		joints = uninitializedAlloc<SkeletonJoint > (elems.size());
		for (uint i = 0; i < elems.size(); i++)
			new (&joints[i]) SkeletonJoint(elems[i]);

		preprocessJoints();
	}

	void Skeleton::initAnimations(const std::vector<SkeletalAnimation::InitInfo> &elems)
	{
		std::vector<const SkeletalAnimation::InitInfo*> sortedElems;
		sortedElems.reserve(elems.size());
		for (uint i = 0; i < elems.size(); i++)
			sortedElems.push_back(&elems[i]);
		//std::sort(sortedElems.begin(), sortedElems.end(), lessByName<SkeletalAnimation>);
		animations = uninitializedAlloc<SkeletalAnimation > (sortedElems.size());
		for (uint i = 0; i < sortedElems.size(); i++)
			new (&animations[i]) SkeletalAnimation(*sortedElems[i], *this);
	}

	uint Skeleton::getJointTopologicalIndex(uint index) const
	{
		//TODO: Zamienic slowa w nazwie? Troche mylaca
		HASSERT(index < numJoints);
		return topologicalIndices[index];
	}

	const SkeletonJoint& Skeleton::getJoint(uint index) const
	{
		HASSERT(index < numJoints);
		return joints[index];
	}

	const SkeletonJoint& Skeleton::getJoint(const char *name) const
	{
		HASSERT(!"Jeszcze nie dziala");
		SkeletonJoint *joint = binarySearch(joints, joints + numJoints, NameFieldComparer<SkeletonJoint > (name));
		if (joint == joints + numJoints)
			RAISE(Exception, format("Joint not found: '%s'", name));
		return *joint;
	}

	uint Skeleton::getJointIndex(const char *name) const
	{
		//HASSERT(!"Jeszcze nie dziala");
		//SkeletonJoint *joint = binarySearch(joints, joints + numJoints, NameFieldComparer<SkeletonJoint>(name));
		SkeletonJoint *joint;
		for (joint = joints; joint != joints + numJoints; joint++)
			if (strcmp(joint->getName(), name) == 0)
				return joint - joints;
		RAISE(Exception, format("Joint not found: '%s'", name));
	}

	const SkeletalAnimation& Skeleton::getAnimation(uint index) const
	{
		HASSERT(index < numAnimations);
		return animations[index];
	}

	const SkeletalAnimation& Skeleton::getAnimation(const char *name) const
	{

		// zamienić na linear SkeletalAnimation *animation = binarySearch(animations, animations + numAnimations, NameFieldComparer<SkeletalAnimation>(name));
		for (uint i = 0; i < numAnimations; i++)
			if (strcmp(animations[i].getName(), name) == 0)
				return animations[i];
		RAISE(Exception, format("Animation not found: '%s'", name));
	}

	uint Skeleton::getAnimationIndex(const SkeletalAnimation &animation) const
	{
		HASSERT(animations <= &animation && &animation < animations + numAnimations);
		return &animation - animations;
	}

	void Skeleton::setName(const char * newName)
	{
		HASSERT(newName != NULL);

		if (strlen(newName) >= sizeof (name))
			RAISE(Exception, format("SkeletonJoint name too long: '%s'", newName));
		strcpy(name, newName);
	}

	const char* Skeleton::getName() const
	{
		return name;
	}

	const matrix4<float>* Skeleton::getReversePose() const
	{
		return reversePose;
	}

	uint Skeleton::getResourceSize() const
	{
		return 0;
	}
	
	bool Skeleton::load()
	{
		if (getLoadState() != LoadStateType::Discovered)
			return false;

		InitInfo initInfo = SAF2::loadFromFile(filename.c_str());
		numJoints = initInfo.joints.size();
		numAnimations = initInfo.animations.size();
		HASSERT(!initInfo.joints.empty());
		HASSERT(!initInfo.animations.empty());

		initJoints(initInfo.joints);
		initAnimations(initInfo.animations);

		setLoadState(IResource::LoadStateType::Loaded);
		return true;
	}

	bool Skeleton::unload()
	{
		if (getLoadState() == IResource::LoadStateType::Discovered)
			return false;
		delete[] topologicalIndices;
		delete[] reversePose;
		uninitializedDelete(joints, numJoints);
		uninitializedDelete(animations, numAnimations);
		setLoadState(IResource::LoadStateType::Discovered);
		return true;
	}
}