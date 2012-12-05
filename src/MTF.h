#pragma once
#include "Material.h"
#include "MFCommon.h"
#include "ShaderFlags.h"
#include "Mapping.h"

namespace Habanero
{
	class ResourceManager;

	namespace MTF2
	{
		using namespace MF2;

		const uint signature = '2FTM';
        
		struct Material
		{
			uint flags;			
			// Definition
		};

		struct File
		{
			uint signature;
			Material material;
		};

		Habanero::Material *loadFromFile(const char *fileName, Resource::Mapping &textures);
	}
}