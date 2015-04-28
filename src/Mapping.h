#pragma once

#include "default.h"
#include <string>
#include <map>

namespace Habanero
{
	namespace Resource
	{
		typedef std::map<uint, std::string> Mapping;
		
		inline void logMapping(const char * comment, const Mapping &m)
		{
			NEWLOG("Listing %s", comment);
			for(Mapping::const_iterator it = m.begin(); it != m.end(); ++it)
			{
				NEWLOG("[k: %d v: %s]", it->first, it->second.c_str());
			}
		}
	}
	
	typedef std::map<uint, std::string> ResourceMapping;
}
