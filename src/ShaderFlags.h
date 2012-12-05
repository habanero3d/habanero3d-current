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

#include <string>

namespace Habanero
{
	class ShaderFlags
	{
	private:
		// trochę nadmiaru, skoro np colorAmbient i textureAmbient się wykluczają, to mogą mieć jeden bit - zmieniam
		bool colorAmbient : 1;	// jeśli colorAmbient, to nie textureAmbient
		bool colorDiffuse : 1;	// jw, ale diffuse
		bool colorSpecular : 1; // jw, ale specular
		bool colorEmissive : 1;
		bool transparency : 1;	// jw, ale transparency i transparencyMap
		bool normalMap : 1;
		bool displacementMap : 1;
		bool mysteryMap : 1; // "tajemnicza_map"? :-P

	public:
		ShaderFlags(uint mask);
		~ShaderFlags();

		bool operator<(const ShaderFlags &l) const;
		bool operator==(const ShaderFlags &l) const;

		std::string getHeader() const;

		void setColorAmbient(bool colorAmbient);
		bool getColorAmbient() const;

		void setColorDiffuse(bool colorDiffuse);
		bool getColorDiffuse() const;

		void setColorSpecular(bool colorSpecular);
		bool getColorSpecular() const;

		void setColorEmissive(bool colorEmissive);
		bool getColorEmissive() const;

		void setTransparency(bool transparency);
		bool getTransparency() const;

		void setTextureAmbient(bool textureAmbient);
		bool getTextureAmbient() const;

		void setTextureDiffuse(bool textureDiffuse);
		bool getTextureDiffuse() const;

		void setTextureSpecular(bool textureSpecular);
		bool getTextureSpecular() const;

		void setNormalMap(bool normalMap);
		bool getNormalMap() const;

		void setDisplacementMap(bool displacementMap);
		bool getDisplacementMap() const;

		void setTransparencyMap(bool transparencyMap);
		bool getTransparencyMap() const;

		void setMysteryMap(bool mysteryMap);
		bool getMysteryMap() const;
	};
}
