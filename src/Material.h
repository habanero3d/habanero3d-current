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
#include "Color.h"
#include "Texture.h"
#include "ShaderFlags.h"
#include "ref.h"

namespace Habanero
{	
	class Material
	{
	private:
		Material(const Material&);
		void operator =(const Material&);

		ShaderFlags flags;

		Color Ambient,
		      Diffuse,
		      Specular,
		      Emissive;
		float Transparency;

		ref<Texture>	ambientTexture,
								diffuseTexture,
								specularTexture,
								normalMap,
								displacementMap,
								transparencyMap,
								mysteryMap;

	public:
		Material();

		ShaderFlags getFlags() const;

		void setAmbient(Color ambient);
		Color getAmbientColor() const;

		void setAmbient(const ref<Texture> &ambient);
		ref<Texture> getAmbientTexture() const;

		void setDiffuse(Color diffuse);
		Color getDiffuseColor() const;

		void setDiffuse(const ref<Texture> &diffuse);
		ref<Texture> getDiffuseTexture() const;

		void setSpecular(Color specular);
		Color getSpecularColor() const;

		void setSpecular(const ref<Texture> &specular);
		ref<Texture> getSpecularTexture() const;

		void setEmissive(Color emissive);
		Color getEmissive() const;

		void setTransparency(float transparency);
		float getTransparency() const;

		void setTransparencyMap(const ref<Texture> &transparencyMap);
		ref<Texture> getTransparencyMap() const;

		void setNormalMap(const ref<Texture> &normalMap);
		ref<Texture> getNormalMap() const;

		void setDisplacementMap(const ref<Texture> &displacementMap);
		ref<Texture> getDisplacementMap() const;

		void setMysteryMap(const ref<Texture> &mysteryMap);
		ref<Texture> getMysteryMap() const;
	};
}
