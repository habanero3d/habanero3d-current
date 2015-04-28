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

#include "game.h"
#include "Renderer.h"

const float animSpeed = 1.0f, ambient = 0.2f;

const unsigned int windowWidth = 1280, windowHeight = 800;
template <typename Real> vector3<Real> clampv(const vector3<Real> &vec, const vector3<Real> &min, const vector3<Real> &max);

void Game::run()
{
	while (window.processEvents())
	{
		float time = window.getTimeSinceLastUpdate() * animSpeed / 1000;
		logic(time);
		player->setParentToLocalR(matrix4f::buildScaling(0.0001f).rotated(vector3f(1, 0, 0), -M_PI / 2).rotated(vector3f(0, 1, 0), dir).translated(pos));
		scene->update(time);
		if (lastAnimation->weight == 0)
		{
			setAnimation(-1u);
			scene->update(1e-7f);
		}

		FrameDescription * frame = scene->getFrame(*camera);
		
		//NEWLOG("camera1: %u camera2: %u", camera, camera2);
		//frame->dump();
		std::vector<MemorySettingOrder> orders;
		ResourceManager::getInstance().getMemoryOrders(frame, orders);
		Renderer::getInstance().executeMemoryOrders(orders);
		
		Renderer::getInstance().drawFrameToSurface(frame, camera, &window);
		delete frame;
		
		window.update();
	}
	window.close();
}

void Game::logic(float time)
{
	if (walking)
	{
		pos = clampv(pos + vector3f(::cos(-dir + M_PI / 2), 0, ::sin(-dir + M_PI / 2)) * time * 0.1f, vector3f::zero, vector3f(1, 1, 1));
		pos.y = terrain->getHeightAt(pos.x, pos.z);
	}
}

void Game::setAnimation(uint i)
{
	if (i == -1u)
		i = 3;
	if (lastAnimation)
		lastAnimation->start(0);
	lastAnimation = &player->animationSet.getAnimationState(i);
	lastAnimation->start();
}

Game::Game() : window("New renderer demo", windowWidth, windowHeight, false),
lastAnimation(NULL),
walking(false),
pos(vector3f::zero),
dir(0),
cameraMode(false)
{
	//Engine setup:
	root = new RootObject(); //creation
	ResourceManager::getInstance().discoveryDirectory("Content"); //discovery
	//window.setActive(true);
	
	//interaction setup
	window.getKeyboard().setKeyCallback(MakeDelegate(this, &Game::onKeyboardKey));
	window.getMouse().setRelativeMode(true);
	window.getMouse().setButtonCallback(MakeDelegate(this, &Game::onMouseButton));
	window.getMouse().setMoveCallback(MakeDelegate(this, &Game::onMouseMove));

	//scene setup
	scene = new Spatial(new Spheref(vector3f::zero, 0));
	scene->setParentToLocal(matrix4f::identity); // this is critical

	ref<SkinnedMesh> dragon_mesh = ResourceManager::getInstance().getSkinnedMesh("Content/meshes/Dragon/dragon.smf");
	HASSERT(dragon_mesh.isValid());
	dragon_mesh->load();
	player = new SkeletalAnimatedObject(dragon_mesh);

	camera = new Camera(vector3f(0, 0, -1), vector3f(0, 1, 0), M_PI / 4, (float) windowWidth / windowHeight, 10, 20000);
	camera2 = new Camera(vector3f(0, 0, -1), vector3f(0, 1, 0), M_PI / 4, (float) windowWidth / windowHeight, .06, .1);

	ref<HeightMapMesh> hm_mesh = ResourceManager::getInstance().getHeightMapMesh("Content/meshes/HM1/HM.hmf");
	HASSERT(hm_mesh.isValid());
	hm_mesh->load();
	terrain = new HeightMapObject(hm_mesh);
	//terrain->setParentToLocalR(matrix4f::identity.scaled(2.0));
	
	//scene->__forceTransformUpdate();

	scene->attach(terrain);
	scene->attach(player);
	Spatial * dupa = new Spatial(new AABBf(-0.2, -0.2, -0.2, 0.2, 0.2, 0.2));
	dupa->setParentToLocalR(matrix4f::identity.translated(0, 0.1, 0));
	scene->attach(dupa);
	player->attach(camera);
	terrain->attach(camera2);

	ref<StaticMesh> church_mesh = ResourceManager::getInstance().getStaticMesh("Content/meshes/Church/Church.tmf");
	church_mesh->load();
	ref<SkinnedMesh> bigman_mesh = ResourceManager::getInstance().getSkinnedMesh("Content/meshes/BigMan/big_man.smf");
	bigman_mesh->load();
	
	SkeletalAnimatedObject * bigman = new SkeletalAnimatedObject(bigman_mesh);
	bigman->setParentToLocalR(matrix4f::identity.scaled(0.005).rotated(vector3f::nx, M_PI * 0.5).rotated(vector3f::nx, M_PI * 2.3).translated(0.2,.1,0.3));
	
	bigman->animationSet.getAnimationState((uint)0).loop = true;
	bigman->animationSet.getAnimationState((uint) 0).autoRewind = true;
	bigman->animationSet.getAnimationState((uint) 0).start();
	
	Spatial * church = new StaticObject(church_mesh);
	church->setParentToLocalR(matrix4f::identity.translated(30.0, 0.0, 30.0).scaled(0.01));
	scene->attach(church);
	scene->attach(bigman);
	
	camera2->setVisualizeBoudingVolume(false);
	camera2->setParentToLocalR(matrix4f::identity.translated(0.30, 0.1, 0.30));

	Spatial * light = new DirectionalLight(Color(0.3, 0.3, 0.0, 0.0), vector3f::nx, 4*  0.11);
	//Spatial * light = new PointLight(Color(1.0, 1.0, 1.0, 0.0), 5*  0.11);
	dupa->attach(light);
	dupa->setParentToLocalR(matrix4f::identity.translated(vector3f(0.1, 0.2, 0.3)));
	light->setParentToLocalR(matrix4f::identity.rotated(vector3f::nz, M_PI * -0.2));
	
	Spatial * light2 = new DirectionalLight(Color(0.5,0.5,0.5, 0.0), vector3f::nx, 4*  0.11);
	Spatial * dupa2 = new Spatial(new AABBf(-0.2, -0.2, -0.2, 0.2, 0.2, 0.2));
	dupa2->setParentToLocalR(matrix4f::identity.rotated(vector3f::nz, M_PI * 1.5).translated(0.2, 0.4, 0.2));
	dupa2->attach(light2);
	scene->attach(dupa2);
	
	camera->setParentToLocalR(matrix4f::identity.translated(0, 500, 2000).rotated(vector3f::ny, M_PI * .5).rotated(vector3f::nx, M_PI * 0.5).rotated(vector3f::nz, M_PI *0.5));

	//camera->setParentToLocalR(matrix4f::identity.translated(0, 500, 2000).rotated(vector3f::ny, M_PI * .5).rotated(vector3f::nx, M_PI * 0.5).rotated(vector3f::nz, M_PI *0.5));

	//scene->attach(new PointLight(Color(0.85, 0.85, 0.85) ,0.6));
	
	player->animationSet.getAnimationState(6).loop = true;
	player->animationSet.getAnimationState(3).loop = true;
	player->animationSet.getAnimationState((uint) 0).autoRewind = true;
	player->animationSet.getAnimationState(2).autoRewind = true;
	player->animationSet.getAnimationState(5).autoRewind = true;

	setAnimation(-1u);
}

Game::~Game()
{
	delete scene;
	delete root;
}

void Game::onKeyboardKey(KeyboardCodes::Key key, bool pressed)
{
	switch (key)
	{
		case KeyboardCodes::Key::Escape: window.postClose();
			break;
		case KeyboardCodes::Key::W:
			if (pressed)
				setAnimation(6);
			else
				setAnimation(-1u);
			//setAnimation(pressed ? "Walk" : nullptr);
			walking = pressed;
			break;
		case KeyboardCodes::Key::Space:
			if (pressed)
			{
				setAnimation(5);
				walking = false;
			}
			break;
		case KeyboardCodes::Key::LShift:
			cameraMode = pressed;
			break;
		case KeyboardCodes::Key::RShift:
			camera2->getFrustum().addConvexPoint(vector3f(-1., 0., 0.));
			break;
	}
}

void Game::onMouseButton(int x, int y, MouseCodes::Button button, bool down)
{
	if (down) switch (button)
		{
			case MouseCodes::Button::Left:
				setAnimation(0);
				walking = false;
				break;
			case MouseCodes::Button::Right:
				setAnimation(2);
				walking = false;
				break;
		}
}

void Game::onMouseMove(int x, int y)
{
	if (cameraMode)
		camera->setParentToLocalR(camera->getParentToLocalR().rotated(vector3f(1, 0, 0), y * 0.01).rotated(vector3f(0, 0, 1), x * 0.00125));
	else
		dir = fmod(dir - x * 0.01, 2.0 * M_PI);
}

template <typename Real>
vector3<Real> clampv(const vector3<Real> &vec, const vector3<Real> &min, const vector3<Real> &max)
{
	return vector3<Real > (clamp(vec.x, min.x, max.x), clamp(vec.y, min.y, max.y), clamp(vec.z, min.z, max.z));
}
