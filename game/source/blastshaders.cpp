#include "blastshaders.hpp"

namespace Shaders
{
	std::string Framebuffer::VertexSource = R"(
uniform mat4 projection;
uniform vec2 camZoom;
uniform vec2 camPosition;
uniform mat2 camRotation;
uniform vec2 halfViewSize;

attribute vec4 vertex;
attribute vec2 texCoords;
attribute vec4 colors;
uniform vec2 position;
uniform vec2 origin;
uniform float rotation;
uniform vec2 scaling;
uniform vec2 parallax;

varying vec2 vTex;
varying vec4 vColor;

void main()
{
	float sinRot = sin(rotation);
	float cosRot = cos(rotation);
	mat2 rotMat = mat2(cosRot, -sinRot, sinRot, cosRot);

	vec2 worldSpaceCoords = (rotMat * (scaling * vertex.xy - origin)) + position;
	vec2 transformedPoint = camRotation * (camZoom * (worldSpaceCoords - camPosition * parallax)) + halfViewSize;
	gl_Position = projection* vec4(transformedPoint.xy, vertex.zw);
	vTex = texCoords;
	vColor = colors;
})";

#ifdef EMSCRIPTEN
	std::string Framebuffer::FragmentSource = R"(
precision mediump float;

uniform sampler2D texture;
uniform vec4 constraints;
uniform vec3 color;
uniform float opacity;
uniform vec2 textureScroll; //hmmmm

varying vec2 vTex;
varying vec4 vColor;

float boundBetween(float val, float lowerBound, float upperBound)
{
	if(lowerBound > upperBound)
	{
		float temp = lowerBound;
		lowerBound = upperBound;
		upperBound = temp;
	}

	val = val - lowerBound;
	float rangeSize = upperBound - lowerBound;
	if(rangeSize == 0.0)
	{
		return upperBound;
	}
	return val - (rangeSize * floor(val/rangeSize)) + lowerBound;
}

void main()
{

	vec2 constraintSize = abs(vec2(constraints[1] - constraints[0] , constraints[3] - constraints[2]));
	vec2 texCoords = constraintSize * vTex.st + vec2(constraints[0], constraints[2]) - textureScroll;
	texCoords = vec2(boundBetween(texCoords.s, constraints[0], constraints[1]), boundBetween(texCoords.t, constraints[2], constraints[3]));
	gl_FragColor = texture2D(texture, texCoords) * (vec4(color, opacity) * vec4(vColor.rgb, 1.0));
})";

#else
	std::string Framebuffer::FragmentSource = R"(
#version 130
uniform sampler2D texture;
uniform sampler2D sceneTexture;
uniform vec4 constraints;
uniform vec3 color;
uniform float opacity;
uniform vec2 textureScroll; //hmmmm
uniform vec2 blurDir;
uniform float gaussianValues[9];

varying vec2 vTex;
varying vec4 vColor;

float boundBetween(float val, float lowerBound, float upperBound)
{
	if(lowerBound > upperBound)
	{
		float temp = lowerBound;
		lowerBound = upperBound;
		upperBound = temp;
	}

	val = val - lowerBound;
	float rangeSize = upperBound - lowerBound;
	if(rangeSize == 0.0)
	{
		return upperBound;
	}
	return val - (rangeSize * floor(val/rangeSize)) + lowerBound;
}

vec4 sampleGaussian(vec2 tc)
{
	ivec2 resolution = textureSize(texture,0);
	vec2 bVal = vec2(blurDir.x/resolution.x,
					 blurDir.y/resolution.y) * 1.0;

	vec4 sum = vec4(0.0);

	sum += texture2D(texture, tc - (4.0 * bVal)) * gaussianValues[0];
	sum += texture2D(texture, tc - (3.0 * bVal)) * gaussianValues[1];
	sum += texture2D(texture, tc - (2.0 * bVal)) * gaussianValues[2];
	sum += texture2D(texture, tc - (1.0 * bVal)) * gaussianValues[3];
	sum += texture2D(texture, tc) * gaussianValues[4];
	sum += texture2D(texture, tc + (1.0 * bVal)) * gaussianValues[5];
	sum += texture2D(texture, tc + (2.0 * bVal)) * gaussianValues[6];
	sum += texture2D(texture, tc + (3.0 * bVal)) * gaussianValues[7];
	sum += texture2D(texture, tc + (4.0 * bVal)) * gaussianValues[8];

	return sum;
}

void main()
{
	vec2 constraintSize = abs(vec2(constraints[1] - constraints[0] , constraints[3] - constraints[2]));
	vec2 texCoords = constraintSize * vTex.st + vec2(constraints[0], constraints[2]) - textureScroll;
	texCoords = vec2(boundBetween(texCoords.s, constraints[0], constraints[1]), boundBetween(texCoords.t, constraints[2], constraints[3]));
	gl_FragColor = texture2D(sceneTexture, texCoords) + sampleGaussian(texCoords);
})";
#endif

std::string BrightnessExtract::FragmentSource = R"(

uniform sampler2D texture;
uniform vec4 constraints;
uniform vec3 color;
uniform float opacity;
uniform vec2 textureScroll; //hmmmm

varying vec2 vTex;
varying vec4 vColor;

float brightLimit = 0.9;

float boundBetween(float val, float lowerBound, float upperBound)
{
if(lowerBound > upperBound)
{
	float temp = lowerBound;
	lowerBound = upperBound;
	upperBound = temp;
}

val = val - lowerBound;
float rangeSize = upperBound - lowerBound;
if(rangeSize == 0.0)
{
	return upperBound;
}
return val - (rangeSize * floor(val/rangeSize)) + lowerBound;
}

void main()
{
vec2 constraintSize = abs(vec2(constraints[1] - constraints[0] , constraints[3] - constraints[2]));
vec2 texCoords = constraintSize * vTex.st + vec2(constraints[0], constraints[2]) - textureScroll;
texCoords = vec2(boundBetween(texCoords.s, constraints[0], constraints[1]), boundBetween(texCoords.t, constraints[2], constraints[3]));
float stp = 1.0 - step(length(texture2D(texture, texCoords).rgb), brightLimit);
gl_FragColor = texture2D(texture, texCoords) * stp * vec4(color, opacity) * vec4(vColor.rgb, 1.0);
})";
}

