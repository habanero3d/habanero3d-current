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


/*!
@file
@author qfel, akuda
@brief Niezależna od systemu część implementacji obiektu tekstury.
 */

#include "Texture.h"

#include <SFML/Graphics/Image.hpp>
#include <string.h>

#include "Renderer.h"

namespace Habanero
{

	Texture::Texture(const std::string& a_filename) :
	IRenderTarget(RenderTargetType::Texture),
	IResource(ResourceTypeEnum::Texture, a_filename),
	IManagedObject()
	{
		setLoadState(LoadStateType::Discovered);
		width = 0;
		height = 0;
		bits = 0;
		data = NULL;
	}

	bool Texture::load()
	{
		if (getLoadState() == LoadStateType::Loaded)
			return true;

		sf::Image image;
		bool loadingSuccessful;

		loadingSuccessful = image.loadFromFile(filename);

		if (!loadingSuccessful)
			return false;

		width = image.getSize().x;
		height = image.getSize().y;

		data = (void *) new uint[width * height];
		//tu nie da się dodać sprawdzenia poprawności, bo memcpy nie zwraca takiego info
		memcpy(data, image.getPixelsPtr(), 4 * width * height);

		if (Renderer::getInstance().__bindTexture(width, height, data, oglId))
		{
			setLoadState(LoadStateType::Loaded);
			return true;
		}
		else
		{
			delete [] (uint*) data;
			height = 0;
			width = 0;
			return false;
		}
	}

	bool Texture::unload()
	{
		if (getLoadState() == LoadStateType::Loaded)
		{
			if (Renderer::getInstance().__unbindTexture(&oglId))
				setLoadState(IResource::LoadStateType::Discovered);
			else
				return false;
		}

		if (data != NULL)
		{
			delete [] (uint *) data;
			data = NULL;
		}
		//TODO: dodać sprawdzenie czy powiodło się
		return true;
	}

	Texture::~Texture()
	{
		if (getLoadState() != LoadStateType::Discovered)
		{
			unload();
		}
	}

	uint Texture::getResourceSize() const
	{
		if(this->getLoadState() != IResource::LoadStateType::Loaded)
			return 0;
		else
			return height * width * bits;
	}
	
	uint Texture::getOpenGLID() const
	{
		return oglId;
	}

	uint Texture::getHeight() const
	{
		return height;
	}

	uint Texture::getWidth() const
	{
		return width;
	}

	uint Texture::getNumberOfBits() const
	{
		return bits;
	}

	uint Texture::getSizeInPixels() const
	{
		return height * width;
	}

	const void * Texture::getRawDataPtr() const
	{
		return data;
	}

}
