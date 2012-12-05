#include "ModelFactory.h"
#include "ResourceManager.h"
#include "StaticObject.h"
#include "SkeletalAnimatedObject.h"
#include "HeightMapObject.h"

#define CreateObjectMacro(MeshType, ObjectType) \
	ref<MeshType> mesh = ResourceManager::getInstance().get ## MeshType (model.name); \
	mesh->load(); \
	ret = new ObjectType(mesh)

namespace Habanero
{
	namespace ModelLoading
	{

		Spatial *ModelFactory::GetModel(const Model &model)
		{
			Spatial *ret;
			switch (model.type)
			{
				case Static:
				{
					CreateObjectMacro(StaticMesh, StaticObject);
					break;
				}
				case Animated:
				{
					CreateObjectMacro(SkinnedMesh, SkeletalAnimatedObject);
					break;
				}

				case HeightMap:
				{
					CreateObjectMacro(HeightMapMesh, HeightMapObject);
					break;
				}
			}
			return ret;
		}
	
	}
}
