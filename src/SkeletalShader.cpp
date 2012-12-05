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


#include "SkeletalShader.h"
#include "RendererSkeletonSetup.h"
#include "Renderer.h"

namespace Habanero
{

	SkeletalShader::SkeletalShader(const char *vertex, const char *fragment, const ShaderFlags &flags) : LightShader(vertex, fragment, flags)
	{
		setupSkeleton();
	}
	
	SkeletalShader::SkeletalShader(const char* vertex, const char* fragment) : LightShader(vertex, fragment)
	{
		setupSkeleton();
	}

	void SkeletalShader::setupSkeleton()
	{
		GLuint programHandler = getProgramHandler();

		GLuint matrixPaletteBlockIndex = glGetUniformBlockIndex(programHandler, RendererSkeletonSetup::skeletonUboName);
		NEWLOG("[SkeletalShader] binding skeleton buffer to program %u", programHandler);
		glUniformBlockBinding(programHandler, matrixPaletteBlockIndex, RendererSkeletonSetup::skeletonBindingIndex);
		NEWLOG("[SkeletalShader] skeleton buffer bound.");

		Renderer::getInstance().__setSkinnedMeshVertexAttributes();
		
	}
	
	constexpr char SkeletalShader::in_positionBindingName[];
	constexpr char SkeletalShader::in_normalBindingName[];
	constexpr char SkeletalShader::in_texCoordBindingName[];
	constexpr char SkeletalShader::in_boneIndicesBindingName[];
	constexpr char SkeletalShader::in_boneWeightsBindingName[];

//	const uint SkeletalShader::in_positionBindingIndex;
//	const uint SkeletalShader::in_normalBindingIndex;
//	const uint SkeletalShader::in_texCoordBindingIndex;
	const uint SkeletalShader::in_boneIndicesBindingIndex;
	const uint SkeletalShader::in_boneWeightsBindingIndex;
	
	void SkeletalShader::disable()
	{
		LightShader::disable();
	}
}
