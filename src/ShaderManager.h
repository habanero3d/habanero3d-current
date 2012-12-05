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

#include <map>

#include "ShaderFlags.h"
#include "LightShader.h"
#include "ShaderProgram.h"
#include "SkeletalShader.h"
#include "Singleton.h"

namespace Habanero
{
	class ShaderManager : public Singleton<ShaderManager>
	{
		friend class RootObject;
	public:
		LightShader * getLightShaderProgram(const ShaderFlags &flags);
		SkeletalShader * getSkeletalShaderProgram(const ShaderFlags &flags);
		
		LightShader * getStaticShadowShaderProgram();
		SkeletalShader * getSkeletalShadowShaderProgram();
		
		void disableShaderProgram();
		ShaderProgram * getRunningShaderProgram() const;
		bool isShaderProgramActive() const;

	private:
		std::map<ShaderFlags, SkeletalShader *> skeletalShaderPrograms;
		std::map<ShaderFlags, LightShader *> staticShaderPrograms;
		std::map<GLuint, ShaderProgram *> allPrograms;
		SkeletalShader * skeletalShadowProgram;
		LightShader * staticShadowProgram;

		ShaderManager();
		~ShaderManager();
	};

	
}