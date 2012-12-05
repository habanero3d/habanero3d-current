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


#include "Material.h"

namespace Habanero
{	
	Material::Material() : flags(0)
	{
		Ambient = Diffuse = Specular = Emissive = Color();
		Transparency = 0.f;
	}

	ShaderFlags Material::getFlags() const
	{
		return flags;
	}

	void Material::setAmbient(Color ambient)
	{
		flags.setColorAmbient(true);
		Ambient = ambient;
	}

	Color Material::getAmbientColor() const
	{
		HASSERT(flags.getColorAmbient());
		return Ambient;
	}

	void Material::setAmbient(const ref<Texture> &ambient)
	{
		flags.setTextureAmbient(true);
		ambientTexture = ambient;
	}

	ref<Texture> Material::getAmbientTexture() const
	{
		HASSERT(flags.getTextureAmbient());
		return ambientTexture;
	}

	void Material::setDiffuse(Color diffuse)
	{
		flags.setColorDiffuse(true);
		Diffuse = diffuse;
	}

	Color Material::getDiffuseColor() const
	{
		HASSERT(flags.getColorDiffuse());
		return Diffuse;
	}

	void Material::setDiffuse(const ref<Texture> &diffuse)
	{
		flags.setTextureDiffuse(true);
		diffuseTexture = diffuse;
	}

	ref<Texture> Material::getDiffuseTexture() const
	{
		HASSERT(flags.getTextureDiffuse());
		return diffuseTexture;
	}

	void Material::setSpecular(Color specular)
	{
		flags.setColorSpecular(true);
		Specular = specular;
	}

	Color Material::getSpecularColor() const
	{
		HASSERT(flags.getColorSpecular());
		return Specular;
	}

	void Material::setSpecular(const ref<Texture> &specular)
	{
		flags.setTextureSpecular(true);
		specularTexture = specular;
	}

	ref<Texture> Material::getSpecularTexture() const
	{
		HASSERT(flags.getTextureSpecular());
		return specularTexture;
	}

	void Material::setEmissive(Color emissive)
	{
		flags.setColorEmissive(true);
		Emissive = emissive;
	}

	Color Material::getEmissive() const
	{
		HASSERT(flags.getColorEmissive());
		return Emissive;
	}

	void Material::setTransparency(float transparency)
	{
		flags.setTransparency(true);
		Transparency = transparency;
	}

	float Material::getTransparency() const
	{
		HASSERT(flags.getTransparency());
		return Transparency;
	}

	void Material::setTransparencyMap(const ref<Texture> &transparencyMap)
	{
		flags.setTransparencyMap(true);
		this->transparencyMap = transparencyMap;
	}

	ref<Texture> Material::getTransparencyMap() const
	{
		HASSERT(flags.getTransparencyMap());
		return transparencyMap;
	}

	void Material::setNormalMap(const ref<Texture> &normalMap)
	{
		flags.setNormalMap(true);
		this->normalMap = normalMap;
	}

	ref<Texture> Material::getNormalMap() const
	{
		HASSERT(flags.getNormalMap());
		return normalMap;
	}

	void Material::setDisplacementMap(const ref<Texture> &displacementMap)
	{
		flags.setDisplacementMap(true);
		this->displacementMap = displacementMap;
	}

	ref<Texture> Material::getDisplacementMap() const
	{
		HASSERT(flags.getDisplacementMap());
		return displacementMap;
	}

	void Material::setMysteryMap(const ref<Texture> &mysteryMap)
	{
		flags.setMysteryMap(true);
		this->mysteryMap = mysteryMap;
	}

	ref<Texture> Material::getMysteryMap() const
	{
		HASSERT(flags.getMysteryMap());
		return mysteryMap;
	}
}