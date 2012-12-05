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


#include "Color.h"

namespace Habanero
{	
	Color::Color()
	{
		Red = 0.0f;
		Green = 0.0f;
		Blue = 0.0f;
		Alpha = 0.0f;
	}

	Color::Color(float red, float green, float blue, float alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}

	Color::Color(const float * color)
	{
		Red = color[0];
		Green = color[1];
		Blue = color[2];
		Alpha = color[3];
	}
	
	const Color Color::White(0.0, 0.0, 0.0, 1.0);
	const Color Color::Black(1.0, 1.0, 1.0, 1.0);
	const Color Color::Zero(0.0, 0.0, 0.0, 0.0);
	const Color Color::CRed(1.0, 0.0, 0.0, 1.0);
	const Color Color::CBlue(0.0, 0.0, 1.0, 1.0);
	const Color Color::CGreen(0.0, 1.0, 0.0, 1.0);
}
