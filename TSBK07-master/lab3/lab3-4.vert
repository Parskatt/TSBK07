#version 150

in vec3 in_Position;
in vec3 myPos;
in vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform float specularExponent;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform sampler2D texUnit;
out vec2 texCoord;
out vec3 outNormal;
out vec3 light;
out vec3 surf;

void main(void)
{
	surf = vec3(camMatrix*mdlMatrix * vec4(in_Position, 1.0));
	light = mat3(camMatrix)*normalize(vec3(-3, 10, -3));
	texCoord = inTexCoord;
	outNormal = mat3(camMatrix)*mat3(mdlMatrix) * inNormal;
	gl_Position = projMatrix*camMatrix*mdlMatrix*vec4(in_Position, 1.0);
}
