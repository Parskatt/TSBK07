#version 400

//In
in  vec3 in_Position;

//Uniform
uniform mat4 modelToWorldMatrix;
uniform mat4 worldToViewMatrix;
uniform mat4 projectionMatrix;
uniform float time;
uniform float num_particles;
uniform float width;
uniform float height;
uniform float speed;
uniform sampler2D tex;

float mod_help;

void main()
{
	mat4 myMatrix;
	myMatrix = projectionMatrix*worldToViewMatrix*modelToWorldMatrix;

	vec4 newpos, offset;
	newpos = vec4(in_Position, 1.0);

	float x, u, v, temp;

	u = mod(gl_InstanceID, width) / width;
	temp = gl_InstanceID / width;
	v = floor(temp) / num_particles;

	offset = texture(tex, vec2(u, v))*20; //12

	newpos[0] += offset.r;
	newpos[1] += offset.g*15; // more offset in y
	newpos[2] += offset.b;

	newpos[1] -= speed*time;
	newpos[1] = height - mod(abs(newpos[1]), height);


	gl_Position = myMatrix * newpos;

}
