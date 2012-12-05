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

#include "FrameDescription.h"
#include "Spatial.h"

namespace Habanero
{

	FrameDescription::FrameDescription() : StackAllocator(4u * 1024u * 1024u, 64u)
	{
	}

	FrameDescription::~FrameDescription()
	{
		for (uint i = 0; i < visibleObjects.size(); ++i)
			delete visibleObjects[i];

		for (uint i = 0; i < shadowCasters.size(); ++i)
			delete shadowCasters[i];

		for (uint i = 0; i < lights.size(); ++i)
			delete lights[i];
	}

	void FrameDescription::dump() const
	{
		uint numSkelV = 0, numSkelS = 0, numStatV = 0, numStatS = 0;
		uint numLines = 0, numPoints = 0;
		uint numAmbient = 0, numDirectional = 0, numPoint = 0;

		for (uint i = 0; i < visibleObjects.size(); ++i)
			switch (visibleObjects[i]->type)
			{
				case GeometryDrawTask::GeometryType::Lines:
					numLines++;
					break;
				case GeometryDrawTask::GeometryType::Points:
					numPoints++;
					break;
				case GeometryDrawTask::GeometryType::SkinnedMesh:
					numSkelV++;
					break;
				case GeometryDrawTask::GeometryType::StaticMesh:
					numStatV++;
					break;
				default:
					break;
			}

		for (uint i = 0; i < shadowCasters.size(); ++i)
			switch (shadowCasters[i]->type)
			{
				case GeometryDrawTask::GeometryType::SkinnedMesh:
					numSkelS++;
					break;
				case GeometryDrawTask::GeometryType::StaticMesh:
					numStatS++;
					break;
				default:
					break;
			}

		for (uint i = 0; i < lights.size(); ++i)
			switch (lights[i]->type)
			{
				case LightDrawTask::LightType::Ambient:
					numAmbient++;
					break;
				case LightDrawTask::LightType::Directional:
					numDirectional++;
					break;
				case LightDrawTask::LightType::Point:
					numPoint++;
					break;
				default:
					break;
			}


		NEWLOG("numSkelV = %u, numSkelS = %u, numStatV = %u, numStatS = %u, numLines = %u, numPoints = %u, numAmbient = %u, numDirectional = %u, numPoint = %u",
			   numSkelV, numSkelS, numStatV, numStatS,
			   numLines, numPoints,
			   numAmbient, numDirectional, numPoint);
	}

}
