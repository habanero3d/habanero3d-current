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

#include "Spatial.h"

#include <vector>

#include "Memory.h"
#include "Sphere.h"

#include "IBoundingVolume.h"
#include "FrameDescription.h"

#include "Camera.h"

#include "HabMath.h"
#include "AABB.h"
#include "Sphere.h"
#include "line3.h"
#include "CullingVolumesSet.h"
#include "Light.h"

namespace Habanero
{
	Spatial::Spatial(IBoundingVolume<float> * boundingVolume) :
		parentToLocal(matrix4f::identity),
		parentToLocalR(matrix4f::identity),
		worldToLocal(matrix4f::identity),
		worldToLocalR(matrix4f::identity),
		dirty(false), dirtyR(false), growBV(false), rebuildBV(false), parentChanged(false),
		boundingVolume(boundingVolume),
		visualizeBoundingVolume(false),
		//visualizeBoundingVolume(true),
		_isShadowCaster(true)
	{
	}
	
	Spatial::~Spatial()
	{
		delete boundingVolume;
	}

	const IBoundingVolume<float>& Spatial::getBoundingVolume() const
	{
		return *boundingVolume;
	}

	const IBoundingVolume<float>* Spatial::getPickableBoundingVolume() const
	{
		return NULL;
	}
		
	bool Spatial::isShadowCaster() const
	{
		return _isShadowCaster;
	}
		
	void Spatial::setShadowCaster(bool value)
	{
		_isShadowCaster = value;
	}
	
	const matrix4f& Spatial::getParentToLocal() const
	{
		HASSERT(!dirty);
		return parentToLocal;
	}

	const matrix4f& Spatial::getParentToLocalR() const
	{
		HASSERT(!dirtyR);
		return parentToLocalR;
	}

	const matrix4f& Spatial::getWorldToLocal() const
	{
		HASSERT(!dirty);
		return worldToLocal;
	}

	const matrix4f& Spatial::getWorldToLocalR() const
	{
		HASSERT(!dirtyR);
		return worldToLocalR;
	}

	void Spatial::setParentToLocal(const matrix4f &m)
	{
		dirty = false;
		dirtyR = true;

		if(getParent())
			getParent()->rebuildBV = true;
		parentToLocal = m;
	}

	void Spatial::setParentToLocalR(const matrix4f &m)
	{
		dirty = true;
		dirtyR = false;

		if(getParent())
			getParent()->rebuildBV = true;
		parentToLocalR = m;
	}

	void Spatial::attach(Spatial *what)
	{
		HASSERT(what);
		HASSERT(!what->parent);
		what->parentChanged = true;
		Node<Spatial>::attach(what);
	}

	void Spatial::detach()
	{
		parentChanged = true;
		if (parent != NULL)
			getParent()->rebuildBV = true;
		Node<Spatial>::detach();
	}

	/*
	 * browBV to bool który mówi MOJEMU parent, że MI URÓSŁ BV, i trzeba
	 * wywołać parentowi growToContain na wszelki wypadek.
	 * rebuildBV mówi, że BV mi się zmniejszył i trzeba robić rebuilda
	 */
	void Spatial::updateTreeInternal(float time, bool updateTransform)
	{
		HASSERT(!(dirty && dirtyR));
		
		bool dirtyEither = dirty || dirtyR;
		if (dirty)
		{
			dirty = false;
			parentToLocal =	parentToLocalR.inversed();
		}
		else if (dirtyR)
		{
			dirtyR = false;
			parentToLocalR = parentToLocal.inversed();
		}
		if (dirtyEither || parentChanged || updateTransform)
		{
			updateTransform = true;

			Spatial *parent = getParent();
			if (parent)
			{
				worldToLocal = parent->worldToLocal * parentToLocal;
				worldToLocalR = parentToLocalR * parent->worldToLocalR;
			}
			else
			{
				worldToLocal = parentToLocal;
				worldToLocalR = parentToLocalR;
			}
		}

		updateSelf(time, updateTransform);
		
		
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
		{
			nd->updateTreeInternal(time, updateTransform);
			rebuildBV = rebuildBV || nd->rebuildBV;
			nd->rebuildBV = false;
			growBV = growBV || nd->growBV;
		}
		if (rebuildBV)
			rebuildBoundingVolume();
		else if (growBV)
		{
			growBV = false;
			growBoundingVolume();
		}
		growBV = growBV || dirtyEither || parentChanged;
		parentChanged = false;
	}

	void Spatial::update(float time)
	{
		updateTreeInternal(time, false);
		growBV = false;
		rebuildBV = false;
	}

	void Spatial::__forceTransformUpdate()
	{
		updateTreeInternal(0.0, true);
	}
	
	void Spatial::getLights(std::vector<const Light*> &lights) const
	{
		fillLights(lights);
		
		for (const Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
			nd->getLights(lights);
	}
	
	std::vector<const Light*> Spatial::getLights() const
	{
		std::vector<const Light *> lights;
		this->getLights(lights);
		return lights;
	}
	
	void Spatial::rebuildBoundingVolume()
	{
		boundingVolume->zero();
		initBoundingVolume();
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
		{
			nd->growBV = false;
			boundingVolume->growToContain(nd->getBoundingVolume(), nd->getParentToLocalR());
		}
	}

	void Spatial::growBoundingVolume()
	{
		for (Spatial *nd = getFirstChild(); nd; nd = nd->getNextSibling())
			if (nd->growBV)
			{
				nd->growBV = false;
				if (boundingVolume->growToContain(nd->getBoundingVolume(), nd->getParentToLocalR()))
					growBV = true;
			}
	}
	
	void Spatial::recursiveFillFrameDescription(const CullingVolumesSet &cullingVolumes, FrameDescription &scene) const
	{
		CullingVolumesSet localVolumes(cullingVolumes);
		localVolumes.transform(this->getParentToLocal());
		
		SceneMembership membership = localVolumes.getSceneMembership(*boundingVolume);
		
		if(membership == SceneMembership::NotPresent)
			return;

		if(visualizeBoundingVolume)
			boundingVolume->insertVisualizer(this->getWorldToLocalR(), scene);
		
		fillFrameDescription(cullingVolumes, scene, membership);
		
		for (const Spatial *child = getFirstChild(); child; child = child->getNextSibling())
			child->recursiveFillFrameDescription(localVolumes, scene);
		
	}
	
	FrameDescription * Spatial::getFrame(const Camera& camera) const
	{
		//HASSERT(this->getParent() == NULL);
		/*
		 TODO: tu są problemy
		 */
		CullingVolumesSet cvs(camera.getFrustum());
		cvs.transform(camera.getWorldToLocalR());
		
		std::vector<const Light *> lights = this->getLights();
		
		//NEWLOG("lights sizeof: %u", lights.size());
		
		for(uint i = 0 ; i < lights.size(); ++i)
		{
			vector3f lightPos;
			matrix4f transformation = lights[i]->getWorldToLocalR();
			lightPos = transformation.transformVertex(vector3f::zero);
			cvs.addPointLightSource(lightPos);
		}
		
		//NEWLOG("number of planes in unified culling: %u, volumes %u", cvs.getNumberOfPlanes(), cvs.getNumberOfVolumes());
		
		FrameDescription * frame = new FrameDescription();
		recursiveFillFrameDescription(cvs, *frame);
		
		return frame;
	}
	
	/// 4 metody poniżej słóżą wyłącznie wizualizacji
	
	void Spatial::setVisualizeBoudingVolume(bool value)
	{
		this->visualizeBoundingVolume = value;
	}
	
	bool Spatial::getVisualizeBoundingVolume() const
	{
		return this->visualizeBoundingVolume;
	}
	
}
