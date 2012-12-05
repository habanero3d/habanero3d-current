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

#include "LightShader.h"
#include "ShaderFlags.h"
#include "matrix4.h"

namespace Habanero
{

	class SkeletalShader : public LightShader
	{
	private:
		void setupSkeleton();
		
	public:
		/*!@brief
		 Constructor for Skeletal Shader WITH LIGHTS
		 */
		SkeletalShader(const char *vertex, const char *fragment, const ShaderFlags &flags);
		/*!@brief
		 Constructor for Skeletal Shader WITHOUT LIGHTS used IN SHADOW MAPPING!
		 */
		SkeletalShader(const char *vertex, const char *fragment); 
		
		// make sure binding index matches a location setting in shader!
		//static const uint in_positionBindingIndex = 0u;
		//static const uint in_normalBindingIndex = 1u;
		//static const uint in_texCoordBindingIndex = 2u;
		static const uint in_boneIndicesBindingIndex = 3u;
		static const uint in_boneWeightsBindingIndex = 4u;
		
		// make sure these names matches shader code!
		static constexpr char in_positionBindingName[] = "in_position";
		static constexpr char in_normalBindingName[] = "in_normal";
		static constexpr char in_texCoordBindingName[] = "in_texCoord";
		static constexpr char in_boneIndicesBindingName[] = "in_boneInices";
		static constexpr char in_boneWeightsBindingName[] = "in_boneWeights";
		
		virtual void disable();

	};
}