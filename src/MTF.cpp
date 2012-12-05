#include "MTF.h"
#include "File.h"
#include "Exception.h"
#include "ResourceManager.h"

namespace Habanero
{
	namespace MTF2
	{
		Habanero::Material * loadFromFile(const char *fileName, Resource::Mapping &textures)
		{
			uint64 size;
                        // kolejne Kuflowe obej�cie buga gcc
			std::unique_ptr<File, MappingDeleter> file((File*)Habanero::File::map(fileName, Habanero::File::Read, &size).release());
			if (file->signature != signature)
				RAISE(Exception, "Invalid MTF signature");
			
			ShaderFlags flags = file->material.flags;
			 
			Habanero::Material *material = new Habanero::Material();

			char * filePos = (char*)(&file->material.flags + 1);
			if(flags.getColorAmbient())
			{
				material->setAmbient(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)].c_str());
				material->setAmbient(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorDiffuse())
			{
				material->setDiffuse(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)].c_str());
				material->setDiffuse(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorSpecular())
			{
				material->setSpecular(*((Color*)filePos));
				filePos += sizeof(Color);
			}
			else
			{
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)].c_str());
				material->setSpecular(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if(flags.getColorEmissive())
			{
				material->setEmissive(*((Color*)filePos));
				filePos += sizeof(Color);
			}

			if(flags.getTransparency())
			{
				material->setTransparency(*((float*)filePos));
				filePos += sizeof(float);
			}
			else
			{
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)]);
				material->setTransparencyMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getNormalMap())
            {
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)]);
				material->setNormalMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getDisplacementMap())
            {
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)]);
				material->setDisplacementMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			if (flags.getMysteryMap())
            {
				ref<Texture> tex = ResourceManager::getInstance().getTexture(textures[*((uint*)filePos)]);
				material->setMysteryMap(tex);
				tex->load();
				filePos += sizeof(uint);
			}

			uint tmpDebug = filePos - (char*)&file->signature;
			if(filePos - (char*)&file->signature != size)
				RAISE(Exception, "Invalid file size");

			return material;
		}
	}
}