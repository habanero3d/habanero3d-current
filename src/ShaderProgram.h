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


#pragma once
#include <string>

#include "OpenGL.h"
#include "Exception.h"
#include "ShaderFlags.h"

namespace Habanero
{
	class ShaderProgram
	{
	private:
		ShaderProgram(const ShaderProgram&);
		void operator=(const ShaderProgram&);

		GLuint fragmentShaderID, vertexShaderID, programID;
		void setUpFromCode(const std::string& fullVertexCode, const std::string& fullFragmentCode);
		
	public:	
		ShaderProgram(const char *vertex, const char *fragment, const ShaderFlags &flags);
		ShaderProgram(const char * vertex, const char * fragment);	
		virtual ~ShaderProgram();

		void enable();
		virtual void disable();
		
		inline GLuint getProgramHandler() const;
		
		inline static std::string getCodeFromFile(const std::string& path);
		inline static std::string getShaderInfoLog(const GLuint& shaderID);
		inline static std::string getProgramInfoLog(const GLuint& programID);
		
		inline static void dumpCode(const std::string& code);
		
		void dumpAttribInfo() const;
		void dumpUniformInfo() const;
		
		std::string getAttribInfo() const;
		std::string getUniformInfo() const;

	};

	class ShaderException : public Exception
	{
		std::string details;
	public:
		ShaderException(const std::string &msg,
				  const std::string &details,
				  const char *fileName,
				  const char *functionName,
				  uint lineNumber);
		~ShaderException();
		std::string describe() const;
	};
}

