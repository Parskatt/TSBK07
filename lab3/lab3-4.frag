#version 150
in vec3 outNormal;
in vec2 texCoord;
in vec3 light;
in vec3 surf;
in vec4 surface_position;
out vec4 out_Color;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texUnit;
uniform float specularExponent;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform float t;


void main(void)
{

	vec4 a = texture(texUnit, texCoord);
	float diffuse_component=0;
	vec3 temp = vec3(0.0,0.0,0.0);
	for (int i = 0; i < 4;  i++)
	{
		vec3 LightDirection;
		float specularStrength=0;
	  if(isDirectional[i] == false){
				LightDirection = normalize(mat3(camMatrix)*vec3((vec4(lightSourcesDirPosArr[i], 1.0) - surface_position)));
			}
		else{
				LightDirection = mat3(camMatrix)*lightSourcesDirPosArr[i];
		}
		vec3 reflectedLightDirection = reflect(-LightDirection, normalize(outNormal));
		vec3 eyeDirection = normalize(-surf);
		if(dot(reflectedLightDirection, eyeDirection) > 0.0)
		{
			specularStrength = dot(reflectedLightDirection, eyeDirection);
			specularStrength = max(specularStrength, 0.01);
			specularStrength = pow(specularStrength, specularExponent);
			specularStrength = clamp(specularStrength, 0.0, 1);
		}
		diffuse_component = dot(normalize(outNormal), LightDirection);
		diffuse_component = clamp(diffuse_component, 0, 1);
		temp = temp + (diffuse_component*0.8+0.8*specularStrength)*lightSourcesColorArr[i];
	}

	out_Color = a*vec4(temp,1.0);
}
