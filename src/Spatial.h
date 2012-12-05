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

#include "Node.h"
#include "matrix4.h"
#include "IBoundingVolume.h"
#include "FrameDescription.h"
#include "SceneMembership.h"
#include <vector>

namespace Habanero
{
	template <typename Real> class Frustum;
	template <typename Real> class AABB;
	template <typename Real> class Sphere;

	class Camera;
	class Light;
	class FrameDescription;
	class CullingVolumesSet;

	class Spatial : public Node<Spatial>
	{
	private:
		matrix4f parentToLocalR;
		matrix4f parentToLocal;
		matrix4f worldToLocal;
		matrix4f worldToLocalR;
		bool dirty, dirtyR, parentChanged;

		bool visualizeBoundingVolume;
		bool _isShadowCaster;

		void updateTreeInternal(float time, bool updateTransform);
		void rebuildBoundingVolume();
		void growBoundingVolume();

	protected:
		bool growBV, rebuildBV;
		IBoundingVolume<float > * boundingVolume;

		virtual void updateSelf(float time, bool parentUpdated) {}
		virtual void initBoundingVolume() {}// Ze wzgledu na kamere domyslnie nic nie robi
		virtual void fillFrameDescription(const CullingVolumesSet &cullingVolumes, FrameDescription &frame, const SceneMembership membership) const {}
		virtual void fillLights(std::vector<const Light *> &lights) const {}
	
	protected:
		void setShadowCaster(bool value);
		
	public:
		bool isShadowCaster() const;

		void __forceTransformUpdate();
		
		/*
		 TODO: to da się zrobić już w przebiegu update.
		 */
		void getLights(std::vector<const Light*> &lights) const;
		std::vector<const Light*> getLights() const;

	public:
		/*! Ten konstruktor przejmuje boudingVolume na własność, Spatial usuwa
			go w destruktorze
		 */
		Spatial(IBoundingVolume<float > * boundingVolume);
		virtual ~Spatial();

		void setVisualizeBoudingVolume(bool value);
		bool getVisualizeBoundingVolume() const;

		void attach(Spatial *what);
		void detach();

		//! Uaktualnia wszystkie cache'owane macierze i BV w poddrzewie, propaguje zmianę czasu
		void update(float time);

		const matrix4f& getWorldToLocal() const;
		const matrix4f& getWorldToLocalR() const;
		const matrix4f& getParentToLocal() const;
		const matrix4f& getParentToLocalR() const;

		void setParentToLocal(const matrix4f &m);
		void setParentToLocalR(const matrix4f &m);

		const IBoundingVolume<float>& getBoundingVolume() const;
		virtual const IBoundingVolume<float>* getPickableBoundingVolume() const;

		void recursiveFillFrameDescription(const CullingVolumesSet &cullingVolumes, FrameDescription &scene) const;

		FrameDescription * getFrame(const Camera &camera) const;
	};
}
