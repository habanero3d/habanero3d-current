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


/*******************************************************************************
Habanero Project

    File:       stringtools.cpp
    Author:     Andrzej Skalski

    All rights reserved.
*******************************************************************************/

#include <memory.h>
#include <string.h>

// wersja linuxowa wymaga:
#include <vector>

#include "StringUtil.h"

using std::string;

namespace Habanero
{
	string format(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		string result = vformat(fmt, args);
		va_end(args);
		return result;
	}


#ifdef WIN32
	string vformat(const char *fmt, va_list args)
	{
            
		char *buff = (char*)_alloca(_vscprintf(fmt, args) + 1);
		vsprintf(buff, fmt, args);
		return buff;
	}
#endif


	char* strdup(const char *s)
	{
		size_t size = strlen(s) + 1;
		char *ns = new char[size];
		memcpy(ns, s, size);
		return ns;
	}

	bool startsWith(const char *s, const char *preffix)
	{
		for (uint i = 0; preffix[i]; i++)
			if (s[i] != preffix[i])
				return false;
		return true;
	}

	bool startsWith(const std::string &s, const char *preffix)
	{
		return s.compare(0, strlen(preffix), preffix) == 0;
	}

	bool startsWith(const std::string &s, const std::string &preffix)
	{
		return s.compare(0, preffix.length(), preffix) == 0;
	}

	bool endsWith(const char *s, const char *suffix)
	{
		size_t len1 = strlen(s),
		       len2 = strlen(suffix);
		return len1 >= len2 && memcmp(s + len1 - len2, suffix, len2) == 0;
	}

	bool endsWith(const std::string &s, const char *suffix)
	{
		size_t len = strlen(suffix);
		return s.length() >= len && s.compare(s.length() - len, len, suffix) == 0;
	}

	bool endsWith(const std::string &s, const std::string &suffix)
	{
		return s.length() >= suffix.length() && s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0;
	}

	bool CStringComparator::operator()(const char *obj1, const char *obj2) const
	{
		return strcmp(obj1, obj2) < 0;
	}


#ifndef WIN32
    // ze strony http://stackoverflow.com/questions/69738/c-how-to-get-fprintf-results-as-a-stdstring-w-o-sprintf
    string vformat (const char *fmt, va_list ap)
    {
        // Allocate a buffer on the stack that's big enough for us almost
        // all the time.
        size_t size = 1024;
        char buf[size];

        // Try to vsnprintf into our buffer.
        va_list apcopy;
        va_copy (apcopy, ap);
        int needed = vsnprintf (&buf[0], size, fmt, ap);
        // NB. On Windows, vsnprintf returns -1 if the string didn't fit the
        // buffer.  On Linux & OSX, it returns the length it would have needed.

        if (needed <= size && needed >= 0) {
            // It fit fine the first time, we're done.
            return std::string (&buf[0]);
        } else {
            // vsnprintf reported that it wanted to write more characters
            // than we allotted.  So do a malloc of the right size and try again.
            // This doesn't happen very often if we chose our initial size
            // well.
            std::vector<char> buf;
            size = needed;
            buf.resize (size);
            needed = vsnprintf (&buf[0], size, fmt, apcopy);
            return std::string (&buf[0]);
        }
    }
#endif

}