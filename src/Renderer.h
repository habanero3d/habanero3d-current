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

#pragma once

#include "Singleton.h"
#include "IRenderTarget.h"
#include "RendererLightSetup.h"

#include <vector>
#include <string>

#include "vector3.h"
#include "Color.h"
#include "GeometryDrawTask.h"
#include "Camera.h"
#include "MemorySettingOrder.h"
#include "SkinVertex.h"
#include "RendererSkeletonSetup.h"

namespace Habanero
{
	class FrameDescription;
	class Window;
	class Texture;

	class Renderer : public Singleton<Renderer>
	{
		friend class RootObject;
	private:
		Renderer(const Renderer&);
		void operator =(const Renderer&); //shielding

		Renderer();
		~Renderer();
		
		RendererLightSetup LightSetup;
		RendererSkeletonSetup SkeletonSetup;
	public:

		void drawFrameToSurface(const FrameDescription * frame, const Camera * camera, IRenderTarget * target);
		const RendererLightSetup& getLightSetup() const;
		const RendererSkeletonSetup& getSkeletonSetup() const;

		void executeMemoryOrders(const std::vector<MemorySettingOrder>& orders);
		
	private:
		void setRenderTargetToWindow(Window * target);
		void setRenderTargetToTexture(Texture * target);
		void setRenderTarget(IRenderTarget * target);

		void setSkinnedMeshCache(GenericGeometry<SkinVertex> * geometry, bool cache);
		void setStaticMeshCache(GenericGeometry<StaticVertex> * geometry, bool cache);
		
		void setMeshCache(GenericGeometryBase * geometry, bool skinned, bool cache);
		void unsetMeshCache(GenericGeometryBase * geometry);
	public:
		void __setSkinnedMeshVertexAttributes();
		void __setStaticMeshVertexAttributes();
		
	public:
		bool __checkForErrors();
		void __removePendingErrors();
		
		static bool isShaderProgramActive();

	private:
		// do not use these two methods in a tight loop! They allocate memory for
		// string storage!
		std::string __getErrorDescription(uint errNo);
		std::vector<std::string> __getAllErrors();
		
		std::vector<uint> __errorList;

		
		/*!@brief:
		 This method draws geometry tasks to currently bound output.
		 @param frame description of the frame
		 @param only_visible if true, shadow casters listed in frame will be ignored
		 @param use_materials if true, texturing and lightning are enabled. Otherwise, shadow shaders will be used.
		 */
		void drawGeometryTasks(const FrameDescription * frame, bool only_visible, bool use_materials, bool wireframe);

		void drawStaticObject(StaticObjectDrawTask * task, bool use_materials, bool wireframe = false);
		void drawSkeletalObject(SkinnedObjectDrawTask * task, bool use_materials, bool wireframe = false);
		void drawLines(LinesDrawTask * task);
		void drawPoints(PointsDrawTask * task);
		
	public:
		bool __bindTexture(const uint width, const uint height, const void * data, uint &out_openGLID);
		bool __unbindTexture(const uint * openGLID);

	private:
		struct ConstarintsType
		{
			int maxVertexUniformBuffers;
			int maxFragmentUniformBuffers;
			int maxUniformBufferSize;
			int maxUniformBindings;
			int maxTextureImageUnits;
			int maxTextureCoords;
			
			std::string openGLversion;

			void poll();
			void dump() const;
		} Constraints;

		class LightSetup
		{
		public:
			LightSetup(const std::vector<LightDrawTask*>& tasks);

			void bind();
			void unbind();
			uint getUBO_id();
		private:


		};
	};
}
