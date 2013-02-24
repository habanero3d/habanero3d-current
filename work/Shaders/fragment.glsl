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

uniform sampler2D shadowMaps[8];

in vec4 directionalShadowMapCoord[8];

#ifdef COLOR_DIFFUSE
uniform vec4 diffuse;
#else
uniform sampler2D diffuse;
#endif

#ifdef NORMAL_MAP
uniform sampler2D normalMap;
#endif

struct AmbientLightStruct
{
vec3 position;
vec4 color;
float radius;
};

struct DirectionalLightStruct
{
vec3 position;
vec3 direction;
vec4 color;
float radius;
};

struct PointLightStruct
{
vec3 position;
vec4 color;
float radius;
};

layout (std140) uniform AmbientLights
{
AmbientLightStruct ambientLights[8];
};

layout (std140) uniform DirectionalLights
{
DirectionalLightStruct directionalLights[8];
};

layout (std140) uniform PointLights
{
PointLightStruct pointLights[8];
};

in vec3 VertexNormal;
in vec3 VertexPosition;

void main()
{
#ifdef NORMAL_MAP
    vec3 normal = normalize(texture(normalMap, gl_TexCoord[0].st).xyz);
#else
    vec3 normal = normalize(VertexNormal);
#endif
	
	vec4 ambientColor = vec4(0,0,0,0);

	for(int i = 0; i < 8; i++)
	{
		float lightDistance = distance(ambientLights[i].position, VertexPosition);
		if (lightDistance <= ambientLights[i].radius)
		{
			ambientColor += ambientLights[i].color;
		}
	}
	ambientColor = clamp(ambientColor, vec4(0,0,0,0), vec4(1,1,1,1));

	vec4 pointColor = vec4(0,0,0,0);
	for(int i = 0; i < 8; i++)
	{
		vec3 lightVec = pointLights[i].position - VertexPosition;
		float lightDistance = distance(pointLights[i].position, VertexPosition);
		float radius = pointLights[i].radius;
		if (lightDistance <= radius)
		{
			float scale = (radius - lightDistance) / radius;
			scale = 1 - scale*scale;
			float intensity = dot(normalize(lightVec), normal) * scale;
			pointColor += intensity * pointLights[i].color;
		}
	}
	pointColor = clamp(pointColor, vec4(0,0,0,0), vec4(1,1,1,1));
	
	vec4 directionalColor = vec4(0,0,0,0);

#define apply_directional(light_no) \
	{	\
		float lightDistance = distance(directionalLights[light_no].position, VertexPosition);	\
		if (lightDistance <= directionalLights[light_no].radius)	\
		{	\
			float intensity = max(-dot(directionalLights[light_no].direction, normal), 0);	\
			float shadeFactor = textureProj(shadowMaps[light_no], directionalShadowMapCoord[light_no]);	\
			shadeFactor = pow(shadeFactor,3); \
			directionalColor += intensity * shadeFactor * directionalLights[light_no].color;	\
		}	\
	}	

	apply_directional(0);
	apply_directional(1);
/*	
	apply_directional(2);
	apply_directional(3);
	apply_directional(4);
	apply_directional(5);
	apply_directional(6);
	apply_directional(7);
	*/
	

	
	directionalColor = clamp(directionalColor, vec4(0,0,0,0), vec4(1,1,1,1));

#ifdef COLOR_DIFFUSE
    vec4 diffuseColor = diffuse;
#else
    vec4 diffuseColor = texture(diffuse, gl_TexCoord[0].st);
#endif

	vec4 finalColor = pointColor + ambientColor + directionalColor;

	gl_FragColor = diffuseColor * clamp(finalColor, vec4(0,0,0,0), vec4(1,1,1,1));
       
}
