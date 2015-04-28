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

#include "OpenGL.h"
#include "matrix4.h"
#include "vector3.h"
#include <string>

namespace Habanero
{
	class DirectionalShadowMap
	{
	private:
		GLuint framebufferObjectID;
		GLuint depthTextureID;
		
		bool bufferGenerated;
		
		uint width, height;
		
		matrix4f view;
		matrix4f projection;
		
	public:
		DirectionalShadowMap();
		~DirectionalShadowMap();
		
		inline bool isBufferGenerated() const;
		inline uint getWidth() const;
		inline uint getHeight() const;
		bool generateBuffers(uint a_width, uint a_height);
		void deleteBuffers();
		
		void generateMatrices(const vector3f& light_position, const vector3f& light_direction, const float radius);
		void bindForDrawing();
		void unbind();
		
		GLuint getDepthTextureID() const;
		
		const matrix4f& getLightViewMatrix() const;
		const matrix4f& getLightProjMatrix() const;
	};
}