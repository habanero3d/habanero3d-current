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


#include <string>

#include "StringUtil.h"
#include "Exception.h"

using std::string;

namespace Habanero
{
	Exception::Exception(const string &msg, const char *fileName, const char *functionName, uint lineNumber) :
		msg(msg),
		fileName(fileName),
		functionName(functionName),
		lineNumber(lineNumber)
	{
	}

	string Exception::describe() const
	{
		if (fileName != NULL)
			return format("%s at %s:%u(%s)", msg.c_str(), fileName, lineNumber, functionName);
		else
			return msg;
	}

	CRTError::CRTError(const string &msg, int error, const char *fileName, const char *function, uint lineNumber) :
		Exception(msg, fileName, function, lineNumber),
		error(error)
	{
	}

	CRTError::CRTError(const string &msg, const char *fileName, const char *function, uint lineNumber) :
		Exception(msg, fileName, function, lineNumber),
		error(errno)
	{
	}

	string CRTError::describe() const
	{
		return format("%s (error %d) at %s:%u(%s)", msg.c_str(), error, fileName, lineNumber, functionName);
	}
}