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

// author: akuda

#include "RendererSkeletonSetup.h"
#include "default.h"

#include "Renderer.h"

namespace Habanero
{

	RendererSkeletonSetup::RendererSkeletonSetup() :
	bufferGenerated(false)
	{
	}

	RendererSkeletonSetup::~RendererSkeletonSetup()
	{
		if (isBufferGenerated())
		{
			NEWLOG("[RendererSkeletonSetup] destructor: freeing skeleton buffer.");
			freeBuffer();
		}
	}

	bool RendererSkeletonSetup::__checkForErrors()
	{
		return Renderer::getInstance().__checkForErrors();
	}

	void RendererSkeletonSetup::setPaletteFromTask(const SkinnedObjectDrawTask* task)
	{
		HASSERT(bufferGenerated);
		HASSERT(task->getNumJoints() <= RendererSkeletonSetup::maxJoints);
		
		glBindBufferBase(GL_UNIFORM_BUFFER, RendererSkeletonSetup::skeletonBindingIndex, skeletonUbo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, task->getNumJoints() * sizeof(matrix4f), task->getMatrixPalettePtr());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void RendererSkeletonSetup::generateBuffer()
	{
		glGenBuffers(1, &skeletonUbo);
		glBindBuffer(GL_UNIFORM_BUFFER, skeletonUbo);
		glBufferData(GL_UNIFORM_BUFFER, maxJoints * sizeof (matrix4f), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if (__checkForErrors())
		{
			bufferGenerated = true;
			NEWLOG("[RendererSkeletonSetup] skeleton buffer created.");
		}
		else
		{
			NEWLOG("[RendererSkeletonSetup] ERROR: failure creating skeleton buffer.");
			HASSERT(false);
		}
	}

	void RendererSkeletonSetup::freeBuffer()
	{
		if (bufferGenerated)
		{
			glDeleteBuffers(1, &skeletonUbo);
			if (__checkForErrors())
			{
				bufferGenerated = false;
				NEWLOG("[RendererSkeletonSetup] skeleton buffer freed.");
			}
			else
			{
				NEWLOG("[RendererSkeletonSetup] ERROR: failure while freeing skeleton buffer.");
				HASSERT(false);
			}
		}
		else
		{
			NEWLOG("[RendererSkeletonSetup] WARNING: trying to free an unitialized skeleton buffer.");
		}
	}

	bool RendererSkeletonSetup::isBufferGenerated() const
	{
		return bufferGenerated;
	}

	constexpr char RendererSkeletonSetup::skeletonUboName[];
}