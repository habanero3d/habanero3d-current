#include "HMF.h"
#include "File.h"
#include "Exception.h"
#include "ResourceManager.h"
#include "MTF.h"
#include "HeightMapMesh.h"
#include "assert.h"
#include "AABB.h"

/* @file
@author: michauzo
*/

namespace Habanero
{
	namespace HMF2
	{
		Habanero::HeightMapMesh::InitInfo loadFromFile(const char *fileName, Resource::Mapping &materials, Resource::Mapping &textures)
		{
			uint64 size;
                        // kolejne Kuflowe obej�cie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid HMF signature");
			
			if (size < sizeof(File))
				RAISE(Exception, "Incomplete file");

			HeightMap hmInfo = file->mesh;
			uint width = hmInfo.width;
			uint height = hmInfo.height;
			float maxY = hmInfo.maxHeight;
			float repX = hmInfo.texRepX;
			float repY = hmInfo.texRepY;
			ref<Texture> heightMap = ResourceManager::getInstance().getTexture(textures[hmInfo.textureId]);
			heightMap->load();

			HASSERT(width >= 2 && height >= 2);

			GenericGeometry<StaticVertex>::InitInfo initInfo;
			std::vector<StaticVertex> &vertices = initInfo.vertices;
			vertices.reserve(width * height);

			float sxy = 1.0f / (width - 1);
			maxY /= 255.0f;
			repX /= width - 1;
			repY /= height - 1;
			for (uint z = 0; z < height; z++)
			{
				byte *line = (byte*)heightMap->getRawDataPtr() + z * (heightMap->getHeight() - 1) / (height - 1) * heightMap->getWidth() * 4;
				for (uint x = 0; x < width; x++)
				{
					StaticVertex v;
					v.position.x = x * sxy;
					v.position.y = line[x * (heightMap->getWidth() - 1) / (width - 1) * 4] * maxY;
					v.position.z = z * sxy;
					v.texCoord.x = x * repX;
					v.texCoord.y = z * repY;
					v.normal = vector3f::zero;
					vertices.push_back(v);
				}
			}

			auto normal = [&](uint x, uint y) -> vector3f&
			{
				return vertices[y * width + x].normal;
			};
			auto position = [&](uint x, uint y) -> vector3f&
			{
				return vertices[y * width + x].position;
			};
			uint c;
			auto triangle = [&](uint x, uint y, uint x1, uint y1, uint x2, uint y2)
			{
				c++;
				normal(x, y) += cross(position(x2, y2) - position(x, y), position(x1, y1) - position(x, y));
			};
			for (uint y = 0; y < height; y++)
				for (uint x = 0; x < width; x++)
				{
					c = 0;
					if (x + 1 < width && y + 1 < height)
						triangle(x, y, x + 1, y, x, y + 1);
					if (x > 0 && y + 1 < height)
						triangle(x, y, x, y + 1, x - 1, y + 1);
					if (x > 0 && y > 0)
						triangle(x, y, x - 1, y, x, y - 1);
					if (x + 1 < width && y > 0)
						triangle(x, y, x, y - 1, x + 1, y - 1);
					normal(x, y) /= (float)c;
				}
			SubMesh::InitInfo subMeshInitInfo;
			subMeshInitInfo.material = shared(MTF2::loadFromFile(materials[hmInfo.materialId].c_str(), textures));
			for (uint y = 0; y + 1 < height; y++)
				for (uint x = 0; x + 1 < width; x++)
				{
					subMeshInitInfo.indices.push_back(y * width + x);				
					subMeshInitInfo.indices.push_back((y + 1) * width + x);
					subMeshInitInfo.indices.push_back(y * width + x + 1);

					subMeshInitInfo.indices.push_back(y * width + x + 1);
					subMeshInitInfo.indices.push_back((y + 1) * width + x);
					subMeshInitInfo.indices.push_back((y + 1) * width + x + 1);
				}
			initInfo.subMeshes.push_back(std::move(subMeshInitInfo));
			Habanero::HeightMapMesh::InitInfo hmInitInfo;
			hmInitInfo.geometry = new GenericGeometry<StaticVertex>(initInfo);
			hmInitInfo.height= height;
			hmInitInfo.width = width;
			hmInitInfo.maxY = hmInfo.maxHeight;
			hmInitInfo.terrainTex = textures[hmInfo.textureId];
			//hmInitInfo.boundingVolume = 

			return hmInitInfo;
		}
	}
}