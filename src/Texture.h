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


/*! @file
@author qfel, akuda
 */
#pragma once
#include "IResource.h"
#include "Mapping.h"
#include "IManagedObject.h"
#include "IRenderTarget.h"

namespace Habanero
{
	class ResourceManager;

	class Texture : public IResource, public IManagedObject, public IRenderTarget
	{
	private:
		uint bits, width, height;
		void * data;
		uint oglId;
	public:
		Texture(const std::string& a_filename);
		Texture(uint a_bits, uint a_width, uint a_height);
		
		~Texture();

		const std::string& getFilename() const;

		bool load();
		bool unload();

		uint getOpenGLID() const;

		uint getHeight() const;
		uint getWidth() const;
		uint getNumberOfBits() const;
		uint getSizeInPixels() const;

		const void * getRawDataPtr() const;
		
		virtual uint getResourceSize() const;

	};
}
