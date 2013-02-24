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

layout (std140) uniform ShadowMapMatrices
{
	mat4 directionalSM[8];
};

layout (location = 3) attribute uvec4 in_boneIndices;
layout (location = 4) attribute vec4 in_boneWeights;

uniform mat4 in_ModelMatrix;

out vec3 VertexNormal;
out vec3 VertexPosition;

out vec4 directionalShadowMapCoord[8];

void main()
{
	vec4 position = vec4(0,0,0,0);
	vec4 normal = vec4(0,0,0,0);

	vec4 in_normal4 = vec4(gl_Normal, 0.0);
	vec4 in_position4 = gl_Vertex;

	//vec4 in_normal4 = vec4(in_normal, 0.0);
	//vec4 in_position4 = vec4(in_position, 0.0);

	/*
	if (in_boneWeights[0] == 0 && in_boneWeights[1] == 0 && in_boneWeights[2] == 0 && in_boneWeights[3] == 0)
	{
		position = in_position4;
		normal = in_normal4;
	}
	else*/
	{
		position += in_boneWeights[0] * BoneMatrices[int(in_boneIndices[0])] * in_position4;
		position += in_boneWeights[1] * BoneMatrices[int(in_boneIndices[1])] * in_position4;
		position += in_boneWeights[2] * BoneMatrices[int(in_boneIndices[2])] * in_position4;
		position += in_boneWeights[3] * BoneMatrices[int(in_boneIndices[3])] * in_position4;
		
		normal += in_boneWeights[0] * BoneMatrices[int(in_boneIndices[0])] * in_normal4;
		normal += in_boneWeights[1] * BoneMatrices[int(in_boneIndices[1])] * in_normal4;
		normal += in_boneWeights[2] * BoneMatrices[int(in_boneIndices[2])] * in_normal4;
		normal += in_boneWeights[3] * BoneMatrices[int(in_boneIndices[3])] * in_normal4;
	}
	
	//position = in_position4;
	//normal = in_normal4;
	

	gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * position);
	
	// teksturowanie
	gl_TexCoord[0].st = gl_MultiTexCoord0;//in_texCoord;
	
	// oświetlenie
	VertexNormal = normalize(gl_NormalMatrix * normal.xyz);
	VertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	for(int i = 0 ; i < 8; ++i)
	{
		directionalShadowMapCoord[i] = directionalSM[i] * in_ModelMatrix * position;
	}
}

