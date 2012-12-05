#include "File.h"
#include <sstream>
#include <cstring>

namespace Habanero
{

	void Normalize(std::string &path)
	{
		const int bufferSize = 80;
		char buffer[bufferSize];
		std::vector<std::string> pathPartsStack;
		int pos ;
		do
		{
			pos = path.find("\\");
			if (pos!=-1)  path.replace(pos, 1, "/");
		}
		while (pos!=-1);
		std::stringstream stream(path, std::stringstream::in);
		while (!stream.eof())
		{
			stream.getline(buffer, bufferSize, '/');
			if (!strcmp(buffer, "."))
				continue;
			if (!strcmp(buffer, ".."))
			{
				pathPartsStack.pop_back();
				continue;
			}
			pathPartsStack.push_back(std::string(buffer));
		}
		std::vector<std::string>::iterator it = pathPartsStack.begin();
		path = *it;
		for (++it; it != pathPartsStack.end(); ++it)
		{
			path += '/';
			path += *it;
		}
	}
}