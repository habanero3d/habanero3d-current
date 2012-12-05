#pragma once

namespace Habanero
{
	namespace ModelLoading
	{
		enum ModelType { Static, Animated, DLODStatic, DLODAnimated, CLODStatic, CLODAnimated, HeightMap, PointLight, AmbientLight };
	

		struct Model
		{
			const char *name;
			ModelType type;
			Model() {}
			Model(const char *s, ModelType type) : name(s), type(type)
			{}
		};

	}
}