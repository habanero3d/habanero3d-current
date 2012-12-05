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

#pragma once
#include <vector>
#include <string>

#include "SkeletonJoint.h"
#include "SkeletalAnimation.h"
#include "IResource.h"
#include "Mapping.h"
#include "IManagedObject.h"

/* NOTKI
w książkach i silnikach widuję silną sugestię, by jeden szkielet mógł być
współdzielony przez wiele *różnych* modeli. Ze szkieletem są związane animacje,
więc może to być sensowne (np. wszyscy ludzie mają takie same animacje, niezależnie
od tego jakie noszą spodnie).

Stąd wniosek, że należy uznać szkielet za oddzielny resource od modelu.

    do jointów powinno dać się dostawać per-ID i per-nazwa.
	przykład:
	miecz->attachTo(koleś->getSkeleton()->getJointByName("lewa_reka"));

qfel: Wywalilem zwracanie wskaznikow, rownie dobrze mozna uzywac getCostam(index), a bedzie przy okazji HASSERT.
*/


namespace Habanero
{
	template <typename Real> class matrix4;

	class Skeleton : public IResource, public IManagedObject
	{
	public:
		struct InitInfo
		{
			std::vector<SkeletonJoint::InitInfo> joints;
			std::vector<SkeletalAnimation::InitInfo> animations;
		};
	private:
		Skeleton(const Skeleton&);
		void operator =(const Skeleton&);

		uint *topologicalIndices;
		SkeletonJoint *joints;
		SkeletalAnimation *animations;
		matrix4<float> *reversePose;
		char name[32];

		void preprocessJoints();
		void initJoints(const std::vector<SkeletonJoint::InitInfo> &elems);
		void initAnimations(const std::vector<SkeletalAnimation::InitInfo> &elems);

	public:
		//TODO: prywatyzacja i gettery (bo nie mogą być constowe)
		uint numJoints,
	         numAnimations;
		
		Skeleton(const std::string& a_filename);
		~Skeleton();
		
		void setName(const char * newName);
		const char* getName() const;
		uint getJointTopologicalIndex(uint index) const;
		const SkeletonJoint& getJoint(uint index) const;
		const SkeletonJoint& getJoint(const char *name) const;
		const SkeletalAnimation& getAnimation(uint index) const;
		const SkeletalAnimation& getAnimation(const char *name) const;
		uint getAnimationIndex(const SkeletalAnimation &animation) const;
		uint getJointIndex(const char *name) const;
		const matrix4<float> *getReversePose() const;

		virtual uint getResourceSize() const;

		bool load();
		bool unload();
	};
}
