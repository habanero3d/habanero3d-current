
/* author: michauzo */

#include "LogManager.h"
#include "Light.h"
#include "vector3.h"
#include "Memory.h"

namespace Habanero
{

	Light::Light(const Color &a_color, float a_radius) :
	Spatial(new Spheref(vector3f::zero, a_radius)),
	color(a_color),
	range(vector3f::zero, a_radius)
	{
	}

	void Light::setColor(Color color)
	{
		this->color = color;
	}

	void Light::setRadius(float radius)
	{
		this->range = Spheref(vector3f::zero, radius);
		boundingVolume->zero();
		this->initBoundingVolume();
	}
}