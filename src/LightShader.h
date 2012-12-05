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

#include "ShaderProgram.h"
#include "ShaderFlags.h"
#include "vector3.h"
#include "Color.h"
#include "matrix4.h"

namespace Habanero
{
	class Material;

	class LightShader : public ShaderProgram
	{
	private:
		const ShaderFlags flags;
		GLint diffuseLocation; //acts as diffuse color or diffuse texture, depending on shader flags
		GLint normalMapLocation;
		GLint modelMatrixLocation;

		void setupLights();
		
	public:
		LightShader(const char *vertex, const char *fragment, const ShaderFlags &flags);
		LightShader(const char *vertex, const char *fragment); // empty constructor for SkeletalShadows TODO write it elegantly
		
	public:
		void setAllFromMaterial(const Material * material);
		
		void setNormal(const vector3f &normal);

		void setDiffuseTexture(GLuint textureId);
		void setDiffuseColor(const Color &color);

		void setNormalMap(GLuint textureId);
		
		void setModelMatrix(const matrix4f& in_matrix);
		
		virtual void disable();

	};
}