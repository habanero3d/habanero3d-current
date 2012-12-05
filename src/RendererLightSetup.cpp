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

#include <GL/glew.h>

#include "RendererLightSetup.h"
#include "OpenGL.h"
#include "Renderer.h"
#include "DirectionalShadowMap.h"

namespace Habanero
{

	RendererLightSetup::RendererLightSetup() :
	buffersGenerated(false)
	{
		/*
		 these assertions makes sure that std140 alignment is met.
		 without it, memory packing gets more complicated.
		 
		 you can find std140 description here:
		 http://www.opengl-redbook.com/appendices/AppL.pdf
		 
		 TODO: these can be reduced to compile time static assertions.
		 */
		HASSERT(sizeof (AmbientLight) % (4 * sizeof (GLfloat)) == 0);
		HASSERT(sizeof (DirectionalLight) % (4 * sizeof (GLfloat)) == 0);
		HASSERT(sizeof (PointLight) % (4 * sizeof (GLfloat)) == 0);
	}

	RendererLightSetup::~RendererLightSetup()
	{
		if (areBuffersGenerated())
		{
			NEWLOG("[RendererLightSetup] destructor: freeing light buffers.");
			freeUBOs();
		};
	}

	bool RendererLightSetup::__checkForErrors()
	{
		return Renderer::getInstance().__checkForErrors();
	}

	void RendererLightSetup::setLights(const std::vector<LightDrawTask*>& a_lights, const matrix4f& viewMatrix)
	{
		HASSERT(buffersGenerated);

		numAmbientLights = 0;
		numDirectionalLights = 0;
		numPointLights = 0;

		for (uint i = 0; i < a_lights.size(); ++i)
		{
			LightDrawTask * task = a_lights[i];
			switch (task->type)
			{
				case LightDrawTask::LightType::Ambient:
				{
					AmbientLightDrawTask * a = static_cast<AmbientLightDrawTask*> (task);
					ambientLights[numAmbientLights++].fromDrawTask(a, viewMatrix);
				}
					break;
				case LightDrawTask::LightType::Directional:
				{
					DirectionalLightDrawTask * d = static_cast<DirectionalLightDrawTask*> (task);
					directionalLights[numDirectionalLights].fromDrawTask(d, viewMatrix);
					DirectionalShadowMaps[numDirectionalLights].generateMatrices(d->position, d->direction, d->radius);
					numDirectionalLights++;
				}
					break;
				case LightDrawTask::LightType::Point:
				{
					PointLightDrawTask * p = static_cast<PointLightDrawTask*> (task);
					pointLights[numPointLights++].fromDrawTask(p, viewMatrix);
				}
					break;
				default:
					HASSERT(false);
			}
		}

		/*
		 WARNING:
		 please do not try to update only a prefix of light buffer, since
		 shader iterates and applies all maxLights, no matter how many lights
		 are actually on scene. This reduces number of conditional statements
		 in fragment shader, and applying "zero" lights does not affect the
		 image.
		 
		 The glBufferSubData is used though not to reduce bandwitch cost, but
		 to avoid a buffer reallocation caused by glBindBufferData.
		 */

		for (uint i = numAmbientLights; i < maxLights; ++i)
			ambientLights[i] = AmbientLight::zero;
		for (uint i = numDirectionalLights; i < maxLights; ++i)
			directionalLights[i] = DirectionalLight::zero;
		for (uint i = numPointLights; i < maxLights; ++i)
			pointLights[i] = PointLight::zero;

		glBindBufferBase(GL_UNIFORM_BUFFER, RendererLightSetup::ambientBindingIndex, uboAmbient);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, maxLights * sizeof (AmbientLight), ambientLights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());

		glBindBufferBase(GL_UNIFORM_BUFFER, RendererLightSetup::directionalBindingIndex, uboDirectional);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, maxLights * sizeof (DirectionalLight), directionalLights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());

		glBindBufferBase(GL_UNIFORM_BUFFER, RendererLightSetup::pointBindingIndex, uboPoint);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, maxLights * sizeof (PointLight), pointLights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());
		
		for(uint i = 0 ; i < numDirectionalLights; ++i)
		{
			directionalShadowMatrices[i] =\
				DirectionalShadowMaps[i].getLightViewMatrix() * \
				DirectionalShadowMaps[i].getLightProjMatrix() * \
				matrix4f::bias;
		}
		
		glBindBufferBase(GL_UNIFORM_BUFFER, RendererLightSetup::directionalShadowMatricesBindingIndex, uboDirectionalShadowMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, maxLights * sizeof (matrix4f), directionalShadowMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());
		
	}

	bool RendererLightSetup::areBuffersGenerated() const
	{
		return buffersGenerated;
	}

	void RendererLightSetup::generateUBOs()
	{
		HASSERT(__checkForErrors());

		glGenBuffers(1, &uboAmbient);
		glBindBuffer(GL_UNIFORM_BUFFER, uboAmbient);
		glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof (AmbientLight), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());

		glGenBuffers(1, &uboDirectional);
		glBindBuffer(GL_UNIFORM_BUFFER, uboDirectional);
		glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof (DirectionalLight), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		HASSERT(__checkForErrors());

		glGenBuffers(1, &uboPoint);
		glBindBuffer(GL_UNIFORM_BUFFER, uboPoint);
		glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof (PointLight), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// creating shadow maps
		for (uint i = 0; i < maxLights; ++i)
		{
			DirectionalShadowMaps[i].generateBuffers(1024, 1024);
		}
		
		glGenBuffers(1, &uboDirectionalShadowMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, uboDirectionalShadowMatrices);
		glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof(matrix4f), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		HASSERT(__checkForErrors());

		buffersGenerated = true;

		NEWLOG("[RendererLightSetup] generated light buffers.");
	}
	
	void RendererLightSetup::bindShadowMaps(const ShaderProgram * shader)
	{
		static char uniformNameBuffer[32];
		for(uint shadowMapNumber = 0 ; shadowMapNumber < numDirectionalLights; ++shadowMapNumber)
		{	
			sprintf(uniformNameBuffer, "shadowMaps[%u]", shadowMapNumber);

			GLint smloc = glGetUniformLocation(shader->getProgramHandler(), uniformNameBuffer);
			//NEWLOG("[RendererLightSetup] inserting shadow map %s at %d", uniformNameBuffer, smloc);
			
			glActiveTexture(GL_TEXTURE8 + shadowMapNumber);
			glBindTexture(GL_TEXTURE_2D, DirectionalShadowMaps[shadowMapNumber].getDepthTextureID());
			glUniform1i(smloc, 8 + shadowMapNumber);
			glActiveTexture(GL_TEXTURE0);
		}
		HASSERT(__checkForErrors());
	}

	void RendererLightSetup::freeUBOs()
	{
		glDeleteBuffers(1, &uboAmbient);
		glDeleteBuffers(1, &uboDirectional);
		glDeleteBuffers(1, &uboPoint);
		glDeleteBuffers(1, &uboDirectionalShadowMatrices);
		
		HASSERT(__checkForErrors());

		buffersGenerated = false;
		NEWLOG("[RendererLightSetup] freed light buffers.");

		//destroying shadow maps
		for (uint i = 0; i < maxLights; ++i)
		{
			DirectionalShadowMaps[i].deleteBuffers();
		}
	}

	RendererLightSetup::AmbientLight::AmbientLight() :
	position(vector3f::zero),
	radius(0.0f),
	color(Color::Zero)
	{
	}

	void RendererLightSetup::AmbientLight::fromDrawTask(const AmbientLightDrawTask* task, const matrix4f& viewMatrix)
	{
		position = viewMatrix.transformVertex(task->position);
		radius = viewMatrix.transformNormal(vector3f(task->radius, 0, 0)).length();
		color = task->color;
	}

	RendererLightSetup::PointLight::PointLight() :
	position(vector3f::zero),
	radius(0.0f),
	color(Color::Zero)
	{
	}

	void RendererLightSetup::PointLight::fromDrawTask(const PointLightDrawTask* task, const matrix4f& viewMatrix)
	{
		position = viewMatrix.transformVertex(task->position);
		radius = viewMatrix.transformNormal(vector3f(task->radius, 0, 0)).length();
		color = task->color;
	}

	RendererLightSetup::DirectionalLight::DirectionalLight() :
	position(vector3f::zero),
	direction(vector3f::zero),
	radius(0.0f),
	color(Color::Zero)
	{
	}

	void RendererLightSetup::DirectionalLight::fromDrawTask(const DirectionalLightDrawTask* task, const matrix4f& viewMatrix)
	{
		position = viewMatrix.transformVertex(task->position);
		radius = viewMatrix.transformNormal(vector3f(task->radius, 0, 0)).length();
		color = task->color;
		direction = viewMatrix.transformNormal(task->direction).normalized();
	}

	uint RendererLightSetup::getNumAmbientLights() const
	{
		return numAmbientLights;
	}

	uint RendererLightSetup::getNumDirectionalLights() const
	{
		return numDirectionalLights;
	}

	uint RendererLightSetup::getNumPointLights() const
	{
		return numPointLights;
	}

	/*
	void RendererLightSetup::setUpShadowMaps()
	{
		for(uint i = 0 ; i < numDirectionalLights ; ++i)
			DirectionalShadowMaps[i].setMatrices(directionalLights[i].position, directionalLights[i].direction);
	}*/
/*
	void RendererLightSetup::bindDirectionalShadowMap(uint light_no)
	{
		//NEWLOG("[DirectionalShadowMap] %u bound for drawing.", light_no);
		DirectionalShadowMaps[light_no].bindForDrawing(directionalLights[light_no].position,
													directionalLights[light_no].direction);
	}

	void RendererLightSetup::unbindDirectionalShadowMap(uint light_no)
	{
		DirectionalShadowMaps[light_no].unbind();
	}*/

	const RendererLightSetup::AmbientLight RendererLightSetup::AmbientLight::zero;
	const RendererLightSetup::PointLight RendererLightSetup::PointLight::zero;
	const RendererLightSetup::DirectionalLight RendererLightSetup::DirectionalLight::zero;

	constexpr char RendererLightSetup::uboAmbientName[];
	constexpr char RendererLightSetup::uboDirectionalName[];
	constexpr char RendererLightSetup::uboPointName[];
	constexpr char RendererLightSetup::uboDirectionalShadowMatricesName[];

}
