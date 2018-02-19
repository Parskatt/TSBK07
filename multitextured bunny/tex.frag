#version 150

out vec4 outColor;
in vec3 exNormal; // Phong
in vec2 texCoord;
in vec3 surf;
uniform sampler2D tex1, tex2;
uniform float time;

void main(void)
{
//	out_Color = vec4(1.0);
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;

	// Simple light
	shade = dot(normalize(exNormal), light);

// sin, static
//	outColor = shade * (texture(tex1, texCoord) * (0.5+sin(surf.z*15)/2) + texture(tex2, texCoord) * (0.5-sin(surf.z*15)/2) );
// sin, animated
	outColor = shade * (texture(tex1, texCoord) * (0.5+sin(surf.z*15 + time)/2) + texture(tex2, texCoord) * (0.5-sin(surf.z*15 + time)/2) );
	outColor = 1.0 * (texture(tex1, texCoord) * (0.5+sin(surf.z*15 + time)/2) + texture(tex2, texCoord) * (0.5-sin(surf.z*15 + time)/2) );
}
