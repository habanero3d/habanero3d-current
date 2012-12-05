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

#include "DirectionalShadowMap.h"
#include "Singleton.h"
#include "Renderer.h"
#include <climits>
#include <GL/glew.h>

namespace Habanero
{

	DirectionalShadowMap::DirectionalShadowMap() :
	width(0),
	height(0)
	{
	}

	DirectionalShadowMap::~DirectionalShadowMap()
	{
	}

	bool DirectionalShadowMap::isBufferGenerated() const
	{
		return bufferGenerated;
	}

	uint DirectionalShadowMap::getHeight() const
	{
		return height;
	}

	uint DirectionalShadowMap::getWidth() const
	{
		return width;
	}

	bool DirectionalShadowMap::generateBuffers(uint a_width, uint a_height)
	{
		// OpenGL uses integers, while I prefer using uints, since negative
		// values makes no sense. I therefore assert for incorrect input.
		HASSERT(a_width <= LONG_MAX);
		HASSERT(a_height <= LONG_MAX);


		// portions of code taken from http://fabiensanglard.net/shadowmapping/index.php
		// special thanks to Fabien Sanglard for sharing he's awesome, clean and correct code.

		GLenum FBOstatus;

		// creating and setting texture for storage of depth buffer
		glGenTextures(1, &depthTextureID);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); 	
		*/
		GLint shadowMapWidth = a_width, shadowMapHeight = a_height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// creating framebuffer object and binding depth texture
		glGenFramebuffers(1, &framebufferObjectID);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectID);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

		// checks
		FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool correct = Renderer::getInstance().__checkForErrors();
		if ((FBOstatus != GL_FRAMEBUFFER_COMPLETE) || !correct)
		{
			glDeleteTextures(1, &depthTextureID);
			bufferGenerated = false;
			width = 0;
			height = 0;
			NEWLOG("[DirectionalShadowMap] error creating framebuffer object for depth storage. No fallback implemented.");
		}
		else
		{
			bufferGenerated = true;
			width = a_width;
			height = a_height;
			NEWLOG("[DirectionalShadowMap] successful creation of shadow map (%u, %u) @ %u.", width, height, framebufferObjectID);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return bufferGenerated;
	}

	void DirectionalShadowMap::generateMatrices(const vector3f& light_position, const vector3f& light_direction, const float radius)
	{
		vector3f middle_scene = light_position + light_direction * radius;
		
		vector3f up = cross(light_position, light_position + light_direction);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, double(width) / double(height), 0.2 * radius, 2*radius);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(light_position.x, light_position.y, light_position.z, middle_scene.x, middle_scene.y, middle_scene.z, up.x, up.y, up.z);

		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*) & view);
		glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*) & projection);

	}

	void DirectionalShadowMap::bindForDrawing()
	{
		HASSERT(this->isBufferGenerated());
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectID);
		HASSERT(Renderer::getInstance().isShaderProgramActive() == false);

		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0,0,0,1.0f);
		
		// always watch your back
		glClear(GL_DEPTH_BUFFER_BIT);
		// -buffer for uninitialized video data!

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(reinterpret_cast<GLfloat*> (&projection));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(reinterpret_cast<GLfloat*> (&view));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//ready for drawing
	}
	
	const matrix4f& DirectionalShadowMap::getLightViewMatrix() const
	{
		return view;
	}
	
	const matrix4f& DirectionalShadowMap::getLightProjMatrix() const
	{
		return projection;
	}

	void DirectionalShadowMap::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	GLuint DirectionalShadowMap::getDepthTextureID() const
	{
		return depthTextureID;
	}

	void DirectionalShadowMap::deleteBuffers()
	{
		if (bufferGenerated)
		{
			glDeleteTextures(1, &depthTextureID);
			glDeleteFramebuffers(1, &framebufferObjectID);
			NEWLOG("[DirectionalShadowMap] deleting shadow map @ %u.", framebufferObjectID);
		}
		else
		{
			NEWLOG("[DirectionalShadowMap] WARNING: trying to delete empty buffers!");
		}
	}
}