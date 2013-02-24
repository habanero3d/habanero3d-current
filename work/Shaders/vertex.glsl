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

#version 330

layout (std140) uniform ShadowMapMatrices
{
	mat4 directionalSM[8];
};

uniform mat4 in_ModelMatrix;

out vec3 VertexNormal;
out vec3 VertexPosition;

out vec4 directionalShadowMapCoord[8];

void main()
{
	gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * gl_Vertex);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	VertexNormal = normalize(gl_NormalMatrix * gl_Normal.xyz);
	VertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	for(int i = 0 ; i < 8; ++i)
	{
		directionalShadowMapCoord[i] = directionalSM[i] * in_ModelMatrix * gl_Vertex;
	}
}
