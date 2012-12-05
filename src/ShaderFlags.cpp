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



#include "ShaderFlags.h"

namespace Habanero
{
	ShaderFlags::ShaderFlags(uint mask)
	{
		(*this) = *((ShaderFlags*)&mask);
	}

	ShaderFlags::~ShaderFlags()
	{
	}

	std::string ShaderFlags::getHeader() const
	{
		std::string result = "#version 330\n";
		
		if(colorAmbient)
			result += "#define COLOR_AMBIENT\n";
		else
			result += "#define TEXTURE_AMBIENT\n";

		if(colorDiffuse)
			result += "#define COLOR_DIFFUSE\n";
		else
			result += "#define TEXTURE_DIFFUSE\n";

		if(colorSpecular)
			result += "#define COLOR_SPECULAR\n";
		else
			result += "#define TEXTURE_SPECULAR\n";

		if(colorEmissive)
			result += "#define COLOR_EMISSIVE\n";

		if(transparency)
			result += "#define TRANSPARENCY\n";
		else
			result += "#define TRANSPARENCY_MAP\n";

		if(normalMap)
			result += "#define NORMAL_MAP\n";

		if(displacementMap)
			result += "#define DISPLACEMENT_MAP\n";

		if(mysteryMap)
			result += "#define MYSTERY_MAP\n";

		return result;
	}

	bool ShaderFlags::operator<(const ShaderFlags &l) const
	{
		return (*((uint*)this) & 0xFF) < (*((uint*)&l) & 0xFF);
	}
	
	bool ShaderFlags::operator==(const ShaderFlags &l) const
	{
		return (*((uint*)this) & 0xFF) == (*((uint*)&l) & 0xFF);
	}
	

	void ShaderFlags::setColorAmbient(bool colorAmbient)
	{
		this->colorAmbient = colorAmbient;
	}

	bool ShaderFlags::getColorAmbient() const
	{
		return colorAmbient;
	}

	void ShaderFlags::setColorDiffuse(bool colorDiffuse)
	{
		this->colorDiffuse = colorDiffuse;
	}

	bool ShaderFlags::getColorDiffuse() const
	{
		return colorDiffuse;
	}

	void ShaderFlags::setColorSpecular(bool colorSpecular)
	{
		this->colorSpecular = colorSpecular;
	}

	bool ShaderFlags::getColorSpecular() const
	{
		return colorSpecular;
	}

	void ShaderFlags::setColorEmissive(bool colorEmissive)
	{
		this->colorEmissive = colorEmissive;
	}

	bool ShaderFlags::getColorEmissive() const
	{
		return colorEmissive;
	}

	void ShaderFlags::setTransparency(bool transparency)
	{
		this->transparency = transparency;
	}

	bool ShaderFlags::getTransparency() const
	{
		return transparency;
	}

	void ShaderFlags::setTextureAmbient(bool textureAmbient)
	{
		colorAmbient = !textureAmbient;
	}

	bool ShaderFlags::getTextureAmbient() const
	{
		return !colorAmbient;
	}

	void ShaderFlags::setTextureDiffuse(bool textureDiffuse)
	{
		colorDiffuse = !textureDiffuse;
	}

	bool ShaderFlags::getTextureDiffuse() const
	{
		return !colorDiffuse;
	}

	void ShaderFlags::setTextureSpecular(bool textureSpecular)
	{
		colorSpecular = !textureSpecular;
	}

	bool ShaderFlags::getTextureSpecular() const
	{
		return !colorSpecular;
	}

	void ShaderFlags::setNormalMap(bool normalMap)
	{
		this->normalMap = normalMap;
	}

	bool ShaderFlags::getNormalMap() const
	{
		return normalMap;
	}

	void ShaderFlags::setDisplacementMap(bool displacementMap)
	{
		this->displacementMap = displacementMap;
	}

	bool ShaderFlags::getDisplacementMap() const
	{
		return displacementMap;
	}

	void ShaderFlags::setTransparencyMap(bool transparencyMap)
	{
		transparency = !transparencyMap;
	}

	bool ShaderFlags::getTransparencyMap() const
	{
		return !transparency;
	}

	void ShaderFlags::setMysteryMap(bool mysteryMap)
	{
		this->mysteryMap = mysteryMap;
	}

	bool ShaderFlags::getMysteryMap() const
	{
		return mysteryMap;
	}
}

