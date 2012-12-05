#pragma once

/* author: michauzo */

#include "Spatial.h"
#include "Color.h"
#include "Sphere.h"

namespace Habanero
{
	class Light : public Spatial
	{
	protected:
		Sphere<float> range;
		Color color;
		Light(const Color &a_color, float a_radius);
	public:
		void setColor(Color color);
		void setRadius(float radius);
	
	protected:
		virtual void initBoundingVolume() = 0;
		
	};
}