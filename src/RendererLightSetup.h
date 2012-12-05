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

/*
 this class represents temporary light setup, a bridge between FrameDescription
 and Renderer classes. This is a part of Renderer, hence the prefix, but to
 keep code clean, it was moved to a separate file.
 */

#pragma once

#include "OpenGL.h"
#include "LightShader.h"
#include "LightDrawTask.h"
#include "matrix4.h"
#include "DirectionalShadowMap.h"

namespace Habanero
{

	class RendererLightSetup
	{
		friend class Renderer;
	private:
		bool buffersGenerated;

		bool __checkForErrors();
	private:
		RendererLightSetup();
		~RendererLightSetup();

	public:
		/*!@brief
		 Creates Uniform Buffer Objects for caching lights settings.
		 Set's buffers' sizes and memory usage policy.
		 */
		void generateUBOs();
		void freeUBOs();

		/*!@brief
		 This method inserts lights information to buffers.
		 It assumes that a ShaderProgram is enabled.
		 */
		void setLights(const std::vector<LightDrawTask*>& a_lights, const matrix4f& viewMatrix);

		/*!@brief
		 This method binds shadow maps (textures) to current shader..
		 */
		void bindShadowMaps(const ShaderProgram * shader);
		
		bool areBuffersGenerated() const;

	private:
		GLuint uboAmbient, uboDirectional, uboPoint;
		GLuint uboDirectionalShadowMatrices;
		
		/*
		 updated each time "setLights" are set, ignored by the shader for
		 optimisation reasons.
		 */
		uint numAmbientLights, numDirectionalLights, numPointLights;

		struct AmbientLight
		{
			vector3f position;
			float __unusedAlignment0;
			Color color;
			float radius;
			float __unusedAlignment1;
			float __unusedAlignment2;
			float __unusedAlignment3;

			AmbientLight();

			void fromDrawTask(const AmbientLightDrawTask * task, const matrix4f & viewMatrix);

			static const AmbientLight zero;
		};

		struct PointLight
		{
			vector3f position;
			float __unusedAlignment0;
			Color color;
			float radius;
			float __unusedAlignment1;
			float __unusedAlignment2;
			float __unusedAlignment3;
			PointLight();

			void fromDrawTask(const PointLightDrawTask * task, const matrix4f & viewMatrix);

			static const PointLight zero;
		};

		struct DirectionalLight
		{
			vector3f position;
			float __unusedAlignment0;
			vector3f direction;
			float __unusedAlignment1;
			Color color;
			float radius;
			float __unusedAlignment2;
			float __unusedAlignment3;
			float __unusedAlignment4;

			DirectionalLight();

			void fromDrawTask(const DirectionalLightDrawTask * task, const matrix4f & viewMatrix);

			static const DirectionalLight zero;
		};

	public:
		static const uint maxLights = 8;
		PointLight pointLights[maxLights];
		AmbientLight ambientLights[maxLights];
		DirectionalLight directionalLights[maxLights];

		DirectionalShadowMap DirectionalShadowMaps[maxLights];
		matrix4f directionalShadowMatrices[maxLights];

		/*!@brief
		 This method sets up shadow maps for depth rendering. For now, it supports
		 only Directional lights.
		 */
		//void setUpShadowMaps();
		
		//void bindDirectionalShadowMap(uint light_no);
		//void unbindDirectionalShadowMap(uint light_no);

		uint getNumAmbientLights() const;
		uint getNumPointLights() const;
		uint getNumDirectionalLights() const;

	public:
		/*
		 These values should match uniform names as defined in fragment shader.
		 */
		static constexpr char uboAmbientName[] = "AmbientLights";
		static constexpr char uboDirectionalName[] = "DirectionalLights";
		static constexpr char uboPointName[] = "PointLights";
		static constexpr char uboDirectionalShadowMatricesName[] = "ShadowMapMatrices";

		static const GLuint ambientBindingIndex = 1u;
		static const GLuint directionalBindingIndex = 2u;
		static const GLuint pointBindingIndex = 3u;
		static const GLuint directionalShadowMatricesBindingIndex = 7u;
	};
}