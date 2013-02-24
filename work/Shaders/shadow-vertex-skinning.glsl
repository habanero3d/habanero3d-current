/*
Copyright 2009-2011 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
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

#version 330 core

layout (std140) uniform MatrixPalette
{
	mat4 BoneMatrices[64];
};

//layout (location = 0) attribute vec3 in_position;
//layout (location = 1) attribute vec3 in_normal;
//layout (location = 2) attribute vec2 in_texCoord;
layout (location = 3) attribute uvec4 in_boneIndices;
layout (location = 4) attribute vec4 in_boneWeights;

void main()
{
	vec4 position = vec4(0,0,0,0);
	vec4 normal = vec4(0,0,0,0);

	//vec4 in_normal4 = vec4(gl_Normal, 0.0);
	vec4 in_position4 = gl_Vertex;

	{
		position += in_boneWeights[0] * BoneMatrices[int(in_boneIndices[0])] * in_position4;
		position += in_boneWeights[1] * BoneMatrices[int(in_boneIndices[1])] * in_position4;
		position += in_boneWeights[2] * BoneMatrices[int(in_boneIndices[2])] * in_position4;
		position += in_boneWeights[3] * BoneMatrices[int(in_boneIndices[3])] * in_position4;
	}

	gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * position);
	
	// teksturowanie
	//gl_TexCoord[0].st = gl_MultiTexCoord0;//in_texCoord;
}

