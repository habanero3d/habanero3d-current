/*
Copyright 2009-2011 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
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

#include "RootObject.h"

#include "SkeletalAnimatedObject.h"
#include "FastDelegate.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "StaticObject.h"
#include "PointLight.h"
#include "HeightMapObject.h"
#include "Window.h"
#include "Memory.h"
#include "Camera.h"
#include "AABB.h"
#include "HabMath.h"


using namespace Habanero;
using namespace fastdelegate;
class Game
{
public:
	Habanero::Window window;
	RootObject * root;
	Spatial * scene;
	SkeletalAnimatedObject *player;
	HeightMapObject *terrain;
	Camera *camera, *camera2;	
	
	vector3f pos;
	float dir;
	SkeletalAnimationState *lastAnimation;
	bool walking, cameraMode;

	void logic(float time);
	void setAnimation(uint i);
	
	Game();
	~Game();
	
	
	void run();
	void onKeyboardKey(KeyboardCodes::Key key, bool pressed);
	void onMouseButton(int x, int y, MouseCodes::Button button, bool down);
	void onMouseMove(int x, int y);
};
