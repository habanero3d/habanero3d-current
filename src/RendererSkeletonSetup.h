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

#pragma once

#include "GeometryDrawTask.h"

namespace Habanero
{

	class RendererSkeletonSetup
	{
	private:
		friend class Renderer;
	
		RendererSkeletonSetup();
		~RendererSkeletonSetup();
		
		GLuint skeletonUbo;
		bool bufferGenerated;
		
		bool __checkForErrors();
	public:
		/*!@brief
		 Creates Uniform Buffer Objects for caching skeleton.
		 Set's buffer's size and memory usage policy.
		 */
		void generateBuffer();
		void freeBuffer();
		
		/*!@brief
		 This method sets a current matrix palette from DrawTask. It overrides
		 only the prefix of full array, assuming, that only valid matrix indices
		 will be used while drawing a skinned mesh.
		 */
		void setPaletteFromTask(const SkinnedObjectDrawTask * task);
		void unsetPalette();

		bool isBufferGenerated() const;
		
		static const uint maxJoints = 64;
		static constexpr char skeletonUboName[] = "MatrixPalette";
		static const GLuint skeletonBindingIndex = 4u;

	};
}