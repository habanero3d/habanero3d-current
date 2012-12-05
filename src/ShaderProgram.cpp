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

#include "default.h"

#include "ShaderProgram.h"
#include "File.h"
#include "Exception.h"
#include "StringUtil.h"
#include "Memory.h"
#include "Singleton.h"
#include "Renderer.h"
#include "ShaderManager.h"

#include <cstring>

using std::string;

namespace Habanero
{

	ShaderProgram::ShaderProgram(const char *vertex, const char *fragment, const ShaderFlags &flags)
	{

		NEWLOG("[ShaderProgram] creating shader for %s and %s.", vertex,fragment);
		
		// loading shaders' sources
		std::string fullVertexCode = getCodeFromFile(vertex);
		std::string fullFragmentCode = flags.getHeader() + getCodeFromFile(fragment);

		setUpFromCode(fullVertexCode, fullFragmentCode);
	}
	
	ShaderProgram::ShaderProgram(const char * vertex, const char * fragment)
	{
		NEWLOG("[ShaderProgram] creating shader for %s and %s, no materials (shadows?)", vertex,fragment);
		
		std::string fullVertexCode = getCodeFromFile(vertex);
		std::string fullFragmentCode = getCodeFromFile(fragment);
		
		setUpFromCode(fullVertexCode, fullFragmentCode);
	}

	void ShaderProgram::setUpFromCode(const std::string& fullVertexCode, const std::string& fullFragmentCode)
	{
		HASSERT(Renderer::getInstance().__checkForErrors());

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		{
			const char * fullVertexCodeArray[] = {fullVertexCode.c_str()};
			const char * fullFragmentCodeArray[] = {fullFragmentCode.c_str()};

			//	http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
			glShaderSource(vertexShaderID, 1, fullVertexCodeArray, NULL);
			glShaderSource(fragmentShaderID, 1, fullFragmentCodeArray, NULL);
		} // end loading shaders' sources

		{//compiling vertex shader
			GLint status;
			glCompileShader(vertexShaderID);
			std::string vertexShaderInfoLog = getShaderInfoLog(vertexShaderID);
			glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				NEWLOG("[ShaderProgram] Error while compiling Vertex Shader! %s\n full code:", vertexShaderInfoLog.c_str());
				dumpCode(fullVertexCode);
				RAISE(ShaderException, "Error while compiling Vertex Shader!", vertexShaderInfoLog);
			}
			else
				NEWLOG("Vertex Shader Info Log: %s", vertexShaderInfoLog.c_str());
		} // end compiling vertex shader

		{ //compiling fragment shader
			GLint status;
			glCompileShader(fragmentShaderID);
			std::string fragmentShaderInfoLog = getShaderInfoLog(fragmentShaderID);
			glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				NEWLOG("[ShaderProgram] Error while compiling Fragment Shader! %s\n full code:", fragmentShaderInfoLog.c_str());
				dumpCode(fullFragmentCode.c_str());
				RAISE(ShaderException, "Error while compiling Fragment Shader!", fragmentShaderInfoLog);
			}
			else
				NEWLOG("Fragment Shader Info Log: %s", fragmentShaderInfoLog.c_str());
		} //end compiling fragment shader


		{//creating program

			programID = glCreateProgram();

			glAttachShader(programID, vertexShaderID);
			glAttachShader(programID, fragmentShaderID);

			NEWLOG("[ShaderProgram] linking %u...", programID);

			glLinkProgram(programID);
			glValidateProgram(programID);

			std::string programInfoLog = getProgramInfoLog(programID);
			GLint status;

			glGetProgramiv(programID, GL_VALIDATE_STATUS, &status);
			if (status != GL_TRUE)
			{
				NEWLOG("[ShaderProgram] Error while linking / validating Shader Program! %s", programInfoLog.c_str());
				NEWLOG("vertex: ");
				dumpCode(fullVertexCode.c_str());
				NEWLOG("fragment:");
				dumpCode(fullFragmentCode.c_str());
				RAISE(ShaderException, "Error while linking / validating Shader Program!", programInfoLog);
			}
			else
				NEWLOG("[ShaderProgram] Shader Program Info Log: %s", programInfoLog.c_str());
		} //end creating program

		HASSERT(programID && vertexShaderID && fragmentShaderID);
		HASSERT(Renderer::getInstance().__checkForErrors());

	}

	ShaderProgram::~ShaderProgram(void)
	{
		uint vertexShaderID, programID, fragmentShaderID;
		if (vertexShaderID)
		{
			glDetachShader(programID, vertexShaderID);
			glDeleteShader(vertexShaderID);
		}
		if (fragmentShaderID)
		{
			glDetachShader(programID, fragmentShaderID);
			glDeleteShader(fragmentShaderID);
		}
		if (programID)
			glDeleteProgram(programID);
	}

	void ShaderProgram::enable()
	{
		glUseProgram(programID);
	}

	void ShaderProgram::disable()
	{
		glUseProgram(0);
	}

	GLuint ShaderProgram::getProgramHandler() const
	{
		return programID;
	}

	std::string ShaderProgram::getCodeFromFile(const std::string& path)
	{
		uint64 size;
		File::uptr fs = File::map(path.c_str(), File::Read, &size);
		char * code = new char[size];
		memcpy(code, fs.get(), size * sizeof (char));
		std::string result(code, size);
		delete code;
		return result;
	}

	std::string ShaderProgram::getShaderInfoLog(const GLuint& shaderID)
	{
		GLsizei infoLength, resultLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);
		char * info = new char[infoLength];
		glGetShaderInfoLog(shaderID, infoLength, &resultLength, info);
		std::string result(info, resultLength);
		delete info;
		return result;
	}

	std::string ShaderProgram::getProgramInfoLog(const GLuint& programID)
	{
		GLsizei infoLength, resultLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);
		char * info = new char[infoLength];
		glGetProgramInfoLog(programID, infoLength, &resultLength, info);
		std::string result(info, resultLength);
		delete info;
		return result;
	}

	void ShaderProgram::dumpCode(const std::string& code)
	{
		NEWLOG("%s", code.c_str());
	}

	std::string ShaderProgram::getAttribInfo() const
	{
		GLint attribNum;
		GLuint programID = this->getProgramHandler();
		glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &attribNum);

		const int maxBufferLength = 1024;
		char * nameBuffer = new char[maxBufferLength];
		char * resultBuffer = new char[maxBufferLength];
		int resultLength;
		int attrSize;
		GLenum attrType;

		std::string result("\n");

		HASSERT(Renderer::getInstance().__checkForErrors());

		for (uint attribNo = 0; attribNo < attribNum; ++attribNo)
		{
			/*
			 void glGetActiveAttrib(	GLuint  	program,
	GLuint  	index,
	GLsizei  	bufSize,
	GLsizei * 	length,
	GLint * 	size,
	GLenum * 	type,
	GLchar * 	name);
			 */
			glGetActiveAttrib(programID, attribNo, maxBufferLength, &resultLength, &attrSize, &attrType, nameBuffer);

			sprintf(resultBuffer, "attribute %u, \"%s\" , size %d, type %d, location %d\n", attribNo, nameBuffer, attrSize, attrType, glGetAttribLocation(programID, nameBuffer));
			result += std::string(resultBuffer);
		}
		HASSERT(Renderer::getInstance().__checkForErrors());
		for (uint attribNo = 0; attribNo < attribNum; ++attribNo)
		{
			GLuint bufferNo;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferNo);
			GLuint arrayEnabled;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &arrayEnabled);
			GLuint arraySize;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_SIZE, &arraySize);
			GLuint arrayStride;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &arrayStride);
			GLuint arrayType;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_TYPE, &arrayType);
			GLuint arrayInteger;
			glGetVertexAttribIuiv(attribNo, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &arrayInteger);

			sprintf(resultBuffer, "\ndetails of %u:\nBuffered in: %u\nAttribArray enabled: %u\narraySize: %u\narrayStride: %u\narrayType %u\narray is integer: %u\n\n",
					attribNo, bufferNo, arrayEnabled, arraySize, arrayStride, arrayType, arrayInteger);
			result += std::string(resultBuffer);
		}
		HASSERT(Renderer::getInstance().__checkForErrors());

		{
			GLint elementArray, arrayBuffer;
			glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArray);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
			sprintf(resultBuffer, "currently, the %dth ELEMENT_ARRAY_BUFFER (indices) array and %dth ARRAY_BUFFER (geometry) are bound.", elementArray, arrayBuffer);
			result += std::string(resultBuffer);
		}

		delete [] nameBuffer;
		delete [] resultBuffer;

		return result;
	}

	std::string ShaderProgram::getUniformInfo() const
	{
		GLint uniformNum;
		GLuint programID = this->getProgramHandler();
		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformNum);

		const int maxBufferLength = 1024;
		char * nameBuffer = new char[maxBufferLength];
		char * resultBuffer = new char[maxBufferLength];
		int resultLength;
		int attrSize;
		GLenum attrType;

		std::string result("\n");

		HASSERT(Renderer::getInstance().__checkForErrors());

		for (uint unifNo = 0; unifNo < uniformNum; ++unifNo)
		{
			/*
			 void glGetActiveAttrib(	GLuint  	program,
	GLuint  	index,
	GLsizei  	bufSize,
	GLsizei * 	length,
	GLint * 	size,
	GLenum * 	type,
	GLchar * 	name);
			 */
			glGetActiveUniformName(programID, unifNo, maxBufferLength, &resultLength, nameBuffer);

			sprintf(resultBuffer, "uniform %u, \"%s\" , location %d\n", unifNo, nameBuffer, glGetUniformLocation(programID, nameBuffer));
			result += std::string(resultBuffer);
		}
		HASSERT(Renderer::getInstance().__checkForErrors());
		return result;
	}
	
	void ShaderProgram::dumpAttribInfo() const
	{
		NEWLOG(getAttribInfo());
	}
	
	void ShaderProgram::dumpUniformInfo() const
	{
		NEWLOG(getUniformInfo());
	}

	ShaderException::ShaderException(const string &msg,
									const std::string &det,
									const char *fileName,
									const char *functionName,
									uint lineNumber) : Exception(msg, fileName, functionName, lineNumber), details(det)
	{
	}

	ShaderException::~ShaderException()
	{
	}

	std::string ShaderException::describe() const
	{
		if (fileName != NULL)
			return format("%s at %s:%u(%s)\nShader Info Log: %s", msg.c_str(), fileName, lineNumber, functionName, details.c_str());
		else
			return msg;
	}

}
