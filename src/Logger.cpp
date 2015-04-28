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


#include <stdarg.h>
#include <time.h>
#include <string>

#include "assert.h"
#include "StringUtil.h"
#include "IStream.h"
#include "Logger.h"

using std::string;

namespace Habanero
{

	IStream *globalLogger = NULL;

	static string formatLogEntry(const string &s)
	{
		time_t t;
		time(&t);
		tm *lt = localtime(&t);
		return format("[%d:%02d:%02d] %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, s.c_str());
	}

	void log_internal(IStream *dest, const char *fmt, ...)
	{
		HASSERT(dest != NULL);
		HASSERT(fmt != NULL);
		va_list args;
		va_start(args, fmt);
		string s = formatLogEntry(vformat(fmt, args));
		va_end(args);
		dest->write(s.data(), s.length());
		dest->flush();
		if (globalLogger != NULL && dest != globalLogger)
		{
			globalLogger->write(s.data(), s.length());
			globalLogger->flush();
		}
	}

}