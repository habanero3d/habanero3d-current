#include "SCF.h"
#include "File.h"
#include "Exception.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelFactory.h"
#include "PointLight.h"
#include "AmbientLight.h"

#include <cstring>

/* @file
@author: michauzo
*/

namespace Habanero
{
	namespace SCF
	{
		Spatial *loadFromFile(const char *fileName)
		{
			uint64 size;
                        // kolejne Kuflowe obej�cie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid SCF signature");

			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");
			
			Spatial *scene = new Spatial(new Spheref(vector3f::zero, 0));
			char *filePtr = (char *) file.get();
			filePtr += sizeof(File);
			for (uint i = 0; i < file->childrenLen; ++i)
			{
				scene->attach(getSpatial(filePtr));
			}

			if (filePtr != (char *)file.get() + size)
			{
				RAISE(Exception, "File size not appropriate!");
			}

			return scene;
		}

		Spatial *getSpatial(char *&filePtr)
		{
			ModelLoading::ModelType type = *((ModelLoading::ModelType *) filePtr);
			filePtr += sizeof(type);
			char buffer[257];
			byte nameSize;
			LightData light;

			Spatial *ret;
			switch (type)
			{
			case ModelLoading::Static: 
			case ModelLoading::Animated:
			case ModelLoading::DLODStatic: 
			case ModelLoading::DLODAnimated:
			case ModelLoading::CLODStatic:
			case ModelLoading::CLODAnimated:
			case ModelLoading::HeightMap:
				nameSize = *((byte *) filePtr);
				filePtr += sizeof(nameSize);
				memcpy(buffer, filePtr, nameSize);
				buffer[nameSize] = 0;
				filePtr += nameSize;
				ret = ModelLoading::ModelFactory::GetModel(ModelLoading::Model(buffer, type));
				break;
			case ModelLoading::PointLight:
			case ModelLoading::AmbientLight:
				light = *((LightData *) filePtr);
				filePtr += sizeof(light);
				if (type == ModelLoading::PointLight)
					ret = new PointLight(light.color, light.range);
				else
					ret = new AmbientLight(light.color, light.range);
				break;
			default:
				RAISE(Exception, "Invalid object type");
			}

			SRT transform = *((SRT *) filePtr);
			filePtr += sizeof(SRT);
			ret->setParentToLocalR(matrix4f::buildRotation(transform.rotation) * matrix4f::buildScaling(transform.scale) *  matrix4f::buildTranslation(transform.translation));

			uint childrenLen = *((uint *) filePtr);
			filePtr += sizeof(childrenLen);
			for (uint i = 0; i < childrenLen; ++i)
			{
				ret->attach(getSpatial(filePtr));
			}

			return ret;
		}
	}
}