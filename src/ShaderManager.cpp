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

//author: Biały, modifications by akuda

#include "ShaderManager.h"
#include "default.h"
#include "OpenGL.h"

namespace Habanero
{

	ShaderManager::ShaderManager()
	: Singleton<ShaderManager>()
	{
		NEWLOG("[ShaderManager] Starting ShaderManager");
		skeletalShadowProgram = NULL;
		staticShadowProgram = NULL;
	}

	void ShaderManager::disableShaderProgram()
	{
		glUseProgram(0);
	}
	
	ShaderProgram * ShaderManager::getRunningShaderProgram() const
	{
		GLuint activeProgramID;
		glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&activeProgramID);
		if(activeProgramID == 0u)
			return NULL;
		return allPrograms.at(activeProgramID);
	}
	
	bool ShaderManager::isShaderProgramActive() const
	{
		GLuint activeProgramID;
		glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&activeProgramID);
		return activeProgramID != 0u;
	}
	
	ShaderManager::~ShaderManager()
	{
		uint numShadowShaders = 0;
		if(staticShadowProgram != NULL)
			numShadowShaders++;
		if(skeletalShadowProgram != NULL)
			numShadowShaders++;
		
		NEWLOG("[ShaderManager] Stopping ShaderManager. It contains %d static and %d skeletal shaders + %u for shadows. Total %u shaders known.",
			staticShaderPrograms.size(),
			skeletalShaderPrograms.size(),
			 numShadowShaders,
			 allPrograms.size());

		for(auto it = allPrograms.begin(); it != allPrograms.end(); ++it)
			delete it->second;
	}

	LightShader *ShaderManager::getLightShaderProgram(const ShaderFlags &flags)
	{
		if (staticShaderPrograms.count(flags) != 0)
			return staticShaderPrograms[flags];
		LightShader * newShader = new LightShader("Shaders/vertex.glsl", "Shaders/fragment.glsl", flags);
		staticShaderPrograms[flags] = newShader;
		allPrograms[newShader->getProgramHandler()] = newShader;
		return newShader;
	}

	SkeletalShader *ShaderManager::getSkeletalShaderProgram(const ShaderFlags &flags)
	{
		if (skeletalShaderPrograms.count(flags) != 0)
			return skeletalShaderPrograms[flags];
		SkeletalShader * newShader = new SkeletalShader("Shaders/vertex-skinning.glsl", "Shaders/fragment.glsl", ShaderFlags(flags));
		skeletalShaderPrograms[flags] = newShader;
		allPrograms[newShader->getProgramHandler()] = newShader;
		return newShader;
	}
	
	LightShader * ShaderManager::getStaticShadowShaderProgram()
	{
		if(staticShadowProgram != NULL)
			return staticShadowProgram;
		
		staticShadowProgram = new LightShader("Shaders/shadow-vertex.glsl", "Shaders/shadow-fragment.glsl");
		allPrograms[staticShadowProgram->getProgramHandler()] = staticShadowProgram;
		return staticShadowProgram;
	}

	SkeletalShader * ShaderManager::getSkeletalShadowShaderProgram()
	{
		if(skeletalShadowProgram != NULL)
			return skeletalShadowProgram;
		
		skeletalShadowProgram = new SkeletalShader("Shaders/shadow-vertex-skinning.glsl", "Shaders/shadow-fragment.glsl");
		allPrograms[skeletalShadowProgram->getProgramHandler()] = skeletalShadowProgram;
		return skeletalShadowProgram;
	}
	
}