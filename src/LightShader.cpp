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


#include <GL/glew.h>

#include "LightShader.h"
#include "Singleton.h"
#include "Renderer.h"
#include "Material.h"

namespace Habanero
{

	LightShader::LightShader(const char *vertex, const char *fragment, const ShaderFlags &flags) : ShaderProgram(vertex, fragment, flags), flags(flags)
	{
		NEWLOG("[LightShader] setting header to :\n%s\n", flags.getHeader().c_str());
		setupLights();
	}

	LightShader::LightShader(const char *vertex, const char *fragment) : ShaderProgram(vertex, fragment), flags(0)
	{
		// NO SETUP LIGHTS HERE!
		NEWLOG("[LightShader] ignoring lightning.");
	}

	void LightShader::setupLights()
	{
		
		GLuint programHandler = getProgramHandler();

		diffuseLocation = glGetUniformLocation(programHandler, "diffuse");
		normalMapLocation = glGetUniformLocation(programHandler, "normalMap");
		modelMatrixLocation = glGetUniformLocation(programHandler, "in_ModelMatrix");

		//TODO these are over-reactive :)
		HASSERT(Renderer::getInstance().getLightSetup().areBuffersGenerated());

		GLuint ambientBlockIndex = glGetUniformBlockIndex(programHandler, RendererLightSetup::uboAmbientName);
		GLuint directionalBlockIndex = glGetUniformBlockIndex(programHandler, RendererLightSetup::uboDirectionalName);
		GLuint pointBlockIndex = glGetUniformBlockIndex(programHandler, RendererLightSetup::uboPointName);

		//NEWLOG("[LightShader] starting binding lights buffers to program %u", programHandler);

		glUniformBlockBinding(programHandler, ambientBlockIndex, RendererLightSetup::ambientBindingIndex);
		glUniformBlockBinding(programHandler, directionalBlockIndex, RendererLightSetup::directionalBindingIndex);
		glUniformBlockBinding(programHandler, pointBlockIndex, RendererLightSetup::pointBindingIndex);

		GLuint directionalSMBlockIndex = glGetUniformBlockIndex(programHandler, RendererLightSetup::uboDirectionalShadowMatricesName);
		glUniformBlockBinding(programHandler, directionalSMBlockIndex, RendererLightSetup::directionalShadowMatricesBindingIndex);
		
		
		HASSERT(Renderer::getInstance().__checkForErrors());

		
		
		//NEWLOG("[LightShader] lights buffers bound.");
	}

	void LightShader::setDiffuseTexture(const GLuint textureId)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(diffuseLocation, 1);
	}

	void LightShader::setDiffuseColor(const Color &color)
	{
		glUniform4fv(diffuseLocation, 1, (float *) &color);
	}

	void LightShader::setModelMatrix(const matrix4f& in_matrix)
	{
		glUniformMatrix4fv(modelMatrixLocation, 1, false, (float*) &in_matrix);
	}
	
	void LightShader::setNormalMap(GLuint textureId)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(normalMapLocation, 4);
	}

	void LightShader::setAllFromMaterial(const Material* material)
	{
		HASSERT(this->flags == material->getFlags());

		if (material->getFlags().getTextureDiffuse())
		{
			this->setDiffuseTexture(material->getDiffuseTexture()->getOpenGLID());
		}
		else
			this->setDiffuseColor(material->getDiffuseColor());

		if (material->getFlags().getNormalMap())
		{
			this->setNormalMap(material->getNormalMap()->getOpenGLID());
		}
	}
	
	void LightShader::disable()
	{
		//unbind any textures
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		ShaderProgram::disable();
		
	}
}
