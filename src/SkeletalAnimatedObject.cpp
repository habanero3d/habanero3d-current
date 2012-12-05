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
@author akuda
 */
#include <vector>

#include "Memory.h"
#include "Sphere.h"
#include "IBoundingVolume.h"
#include "SkeletalAnimatedObject.h"

#include "AABB.h"
#include <stddef.h>

namespace Habanero
{

	SkeletalAnimatedObject::SkeletalAnimatedObject(ref<Skeleton>skeleton) :
	Spatial(new AABBf(0, 0, 0, 0, 0, 0)),
	animationSet(skeleton),
	skeleton(skeleton),
	jointSpatials(new Spatial*[skeleton->numJoints]),
	skeletonPoseBuffer(new matrix4f[skeleton->numJoints])
	{
		for (uint i = 0; i < skeleton->numJoints; ++i)
			jointSpatials[i] = NULL;
	}

	SkeletalAnimatedObject::SkeletalAnimatedObject(ref<SkinnedMesh> mesh) :
	Spatial(mesh->boundingVolume->clone()),
	animationSet(skeleton),
	skeleton(mesh->skeleton),
	jointSpatials(new Spatial*[mesh->skeleton->numJoints])
	{
		skeletonPoseBuffer = new matrix4f[skeleton->numJoints];
		for (uint i = 0; i < skeleton->numJoints; ++i)
			jointSpatials[i] = NULL;
		addMesh(mesh);
	}

	SkeletalAnimatedObject::~SkeletalAnimatedObject()
	{
		for (uint i = 0; i < skeleton->numJoints; i++)
			if (jointSpatials[i])
				delete jointSpatials[i];
		delete [] jointSpatials;
		delete [] skeletonPoseBuffer;
	}

	void SkeletalAnimatedObject::initBoundingVolume()
	{
		if (!m_Meshes.empty())
			for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
				boundingVolume->growToContain(*(*it)->boundingVolume);
	}

	void SkeletalAnimatedObject::addMesh(ref<SkinnedMesh> mesh)
	{
		HASSERT(mesh->skeleton == skeleton);
		m_Meshes.push_back(mesh);
		if (boundingVolume->growToContain(*mesh->boundingVolume))
			growBV = true;
	}

	uint SkeletalAnimatedObject::getNumMeshes() const
	{
		return m_Meshes.size();
	}

	ref<SkinnedMesh> SkeletalAnimatedObject::getMeshAt(uint index) const
	{
		HASSERT(index < m_Meshes.size());
		return m_Meshes[index];
	}

	void SkeletalAnimatedObject::removeMeshAt(uint index)
	{
		m_Meshes.erase(m_Meshes.begin() + index);

		HASSERT(m_Meshes.size() > 0); // jak będzie zaślepka, to dopiszę resztę.
		rebuildBV = true;
	}

	void SkeletalAnimatedObject::fillFrameDescription(const CullingVolumesSet& cullingVolumes,
													FrameDescription& scene,
													const SceneMembership membership) const
	{
		/*
		 this method got overcomplicated, so here's a quick summary
		 SkinnedObjectDrawTask takes (as it's final argument) pointer to
		 scene, which is a StackAllocator, so it can allocate a variable-lenght
		 arrays for matrix-pallete and meshes pointers. Later one these arrays
		 are being filled:
		 matrix-palette indirectly, via pose pointer, and
		 meshes-array via setGeometryAt(index, pointer to geometry)
		 
		 Sorry for all complications, I couldn't figure out a cleaner solution
		 for custom allocation.
		 */
		matrix4f transformation = getWorldToLocalR();
		SkinnedObjectDrawTask * task =
			new (scene) SkinnedObjectDrawTask(
											transformation,
											getNumMeshes(),
											skeleton->numJoints,
											&scene);

		const matrix4f * reversePose = skeleton->getReversePose();
		matrix4f * pose = task->getMatrixPalettePtr();
		animationSet.getSkeletonPose(pose);

		for (uint i = 0; i < skeleton->numJoints; i++)
		{
			pose[i] = reversePose[i] * pose[i];
			if (jointSpatials[i])
				jointSpatials[i]->recursiveFillFrameDescription(cullingVolumes, scene);
		}

		for (uint i = 0; i < getNumMeshes(); ++i)
			task->setGeometryAt(i, getMeshAt(i)->getGeometry());

		if (membership == SceneMembership::VisibleObject)
			scene.visibleObjects.push_back(task);
		else
			scene.shadowCasters.push_back(task);
	}

	void SkeletalAnimatedObject::fillLights(std::vector<const Light*> &lights) const
	{
		for (uint i = 0; i < skeleton->numJoints; ++i)
		{

			if (jointSpatials[i])
				jointSpatials[i]->getLights(lights);
		}
	}

	void SkeletalAnimatedObject::updateSelf(float time, bool parentUpdated)
	{
		if (time != 0)
			animationSet.animate(time);
		if (parentUpdated || time != 0)
		{
			const matrix4f *reversePose = skeleton->getReversePose();
			animationSet.getSkeletonPose(skeletonPoseBuffer);
			for (uint i = 0; i < skeleton->numJoints; i++)
				skeletonPoseBuffer[i] = skeletonPoseBuffer[i] * getWorldToLocalR();
			for (uint i = 0; i < skeleton->numJoints; i++)
				if (jointSpatials[i])
				{
					jointSpatials[i]->setParentToLocalR(skeletonPoseBuffer[i]);
					jointSpatials[i]->update(time);
					if (boundingVolume->growToContain(jointSpatials[i]->getBoundingVolume(), jointSpatials[i]->getParentToLocalR()))
						growBV = true;
				}
		}
		else
		{
			for (uint i = 0; i < skeleton->numJoints; i++)
				if (jointSpatials[i])
				{
					jointSpatials[i]->update(0);
					if (boundingVolume->growToContain(jointSpatials[i]->getBoundingVolume(), jointSpatials[i]->getParentToLocalR()))
						growBV = true;
				}
		}
	}

	const IBoundingVolume<float>* SkeletalAnimatedObject::getPickableBoundingVolume() const
	{

		return m_Meshes[0]->boundingVolume;
	}

	void SkeletalAnimatedObject::attachToJoint(uint index, Spatial *what)
	{

		HASSERT(jointSpatials[index] == NULL);
		jointSpatials[index] = what;
	}

	void SkeletalAnimatedObject::attachToJoint(const char *name, Spatial *what)
	{

		attachToJoint(skeleton->getJointIndex(name), what);
	}

	Spatial * SkeletalAnimatedObject::detachFromJoint(uint jointId)
	{

		Spatial * spatial = jointSpatials[jointId];
		jointSpatials[jointId] = NULL;
		return spatial;
	}

	Spatial * SkeletalAnimatedObject::detachFromJoint(const char *name)
	{
		return detachFromJoint(skeleton->getJointIndex(name));
	}
}