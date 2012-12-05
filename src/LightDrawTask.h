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

#include "Enum.h"
#include "StackAllocator.h"

namespace Habanero
{

	class LightDrawTask : public IStackAllocatedUnit
	{
	public:
		ENUM(LightType,
			Ambient,
			Directional,
			Point);
	public:
		const LightType type;
		const vector3f position;
		const float radius;
		const Color color;

	protected:

		LightDrawTask(LightType a_type, const vector3f& a_position, float a_radius, const Color &a_color) :
		type(a_type),
		position(a_position),
		radius(a_radius),
		color(a_color)
		{
		}
	};

	class DirectionalLightDrawTask : public LightDrawTask
	{
	public:
		const vector3f direction;
		
		DirectionalLightDrawTask(const vector3f& a_position, float a_radius, const Color &a_color, const vector3f &a_direction) :
		LightDrawTask(LightType::Directional, a_position, a_radius, a_color),
		direction(a_direction)
		{
		}
	};
	
	class AmbientLightDrawTask : public LightDrawTask
	{
	public:
		AmbientLightDrawTask(const vector3f& a_position, float a_radius, const Color &a_color) :
		LightDrawTask(LightType::Ambient, a_position, a_radius, a_color)
		{
		}
	};
	
	class PointLightDrawTask : public LightDrawTask
	{
	public:
		PointLightDrawTask(const vector3f& a_position, float a_radius, const Color &a_color) :
		LightDrawTask(LightType::Point, a_position, a_radius, a_color)
		{
		}
	};
	
}