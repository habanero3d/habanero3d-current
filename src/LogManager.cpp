/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
This file is part of Habanero3d.

Habanero3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Habanero3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Habanero3d.  If not, see <http://www.gnu.org/licenses/>.


 */

/*
Author: Andrzej "akuda" Skalski
 */

#include "LogManager.h"

#include <cstdio>
#include <cstdarg>
#include <ctime>

namespace Habanero {
	
	LogManager::LogManager() : Singleton<LogManager>()
	{
		outfile = fopen("log.txt", "a+");	
	}
	
	LogManager::~LogManager()
	{
		fclose(outfile);
	}
	
	void LogManager::Log(const char * message, ...)
	{
		va_list ap;
#ifdef va_copy //linux
		va_list ap_copy;
		va_copy(ap_copy, ap);
#endif
		va_start(ap, message);
		printf("%s ", getPrefix().c_str());
		vprintf(message, ap);
		printf("\n");
#ifdef va_copy	
		va_end(ap);
		va_start(ap_copy, message);
		fprintf(outfile, "%s ", getPrefix().c_str());
		vfprintf(outfile, message, ap_copy);
		fprintf(outfile, "\n");
		va_end(ap_copy);
#else
		fprintf(outfile, "%s ", getPrefix().c_str());
		vfprintf(outfile, message, ap);
		fprintf(outfile, "\n");
		va_end(ap);
#endif
		// TODO: wywalić w ramach optymalizacji
		fflush(outfile);
		fflush(stdout);
	}
	
	void LogManager::Log(std::string message)
	{
		printf("%s %s\n", getPrefix().c_str(), message.c_str());
		fprintf(outfile, "%s %s\n", getPrefix().c_str(), message.c_str());
	}
	
	std::string LogManager::getPrefix()
	{
		static char buf[11];
		time_t t;
		time(&t);
		tm *lt = localtime(&t);
		sprintf(buf, "[%02d:%02d:%02d]", lt->tm_hour, lt->tm_min, lt->tm_sec);
		return std::string(buf);
	}
}