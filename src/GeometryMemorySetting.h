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

// author: akuda

#pragma once

#include "OpenGL.h"

namespace Habanero
{

	class GeometryMemorySetting
	{
	private:
		bool __isCached;
		GLuint __vertexBufferID;
		GLuint __indicesBufferID;
		GLuint __vertexArrayID; //VAO

	public:

		GeometryMemorySetting() :
		__isCached(false),
		__vertexBufferID(0),
		__indicesBufferID(0),
		__vertexArrayID(0)
		{
		}

		~GeometryMemorySetting()
		{
			//TODO this is only for debug
			HASSERT(__isCached == false);
			HASSERT(__vertexBufferID == 0);
			HASSERT(__indicesBufferID == 0);
			HASSERT(__vertexArrayID == 0);
		}

		bool isCached() const
		{
			return __isCached;
		}

		GLuint getVertexBufferID() const
		{
			return __vertexBufferID;
		}

		GLuint getIndicesBufferID() const
		{
			return __indicesBufferID;
		}

		GLuint getVertexArrayID() const
		{
			return __vertexArrayID;
		}

		void setCacheAt(GLuint new_vertexBufferID, GLuint new_indicesBufferID, GLuint new_vertexArrayID)
		{
			//TODO this might be overreative :)
			HASSERT(__isCached == false);
			HASSERT(new_indicesBufferID != 0);
			HASSERT(new_vertexArrayID != 0);
			HASSERT(new_vertexBufferID != 0);
			
			__isCached = true;
			__vertexBufferID = new_vertexBufferID;
			__indicesBufferID = new_indicesBufferID;
			__vertexArrayID = new_vertexArrayID;
			
			//NEWLOG("setting cache at vbo %d, veo %d, vao %d", __vertexBufferID, __indicesBufferID, __vertexArrayID);
		}

		void unsetCache()
		{
			__isCached = false;
			__vertexBufferID = 0;
			__indicesBufferID = 0;
			__vertexArrayID = 0;

		}
	};
}
