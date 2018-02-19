#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal;
out vec3 reflectedView;

uniform mat4 projMatrix;
uniform mat4 worldToViewMatrix;
uniform mat4 modelToWorldMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(worldToViewMatrix * modelToWorldMatrix); // simplified - should be inverse transpose
//	exNormal = normalMatrix1 * inNormal; if we need it (i.e. if we want to combine the skybox with surface light).

	gl_Position = projMatrix * worldToViewMatrix * modelToWorldMatrix * vec4(inPosition, 1.0);

// modelToWorldMatrix = model to world
// worldToViewMatrix = world to view

// IMPORTANT! You MUST know what coordinate system you work in!
// The skybox is defined in world coordinates. Therefore we need to output a direction in
// world coordinates.
// First we transform the position to view coordinates, where it will be the same as the view direction
// (since the PRP is origin there).
	vec3 posInViewCoord = vec3(worldToViewMatrix * modelToWorldMatrix * vec4(inPosition, 1.0));
	vec3 viewDirectionInViewCoord = normalize(posInViewCoord);
// The we transform the resulting direction (now without translations) back to world coordinates
// using the inverse of the world-to-view.
	vec3 viewDirectionInWorldCoord = inverse(mat3(worldToViewMatrix)) * viewDirectionInViewCoord;

// Also transform the normal vector to world coordinates (= skybox coordinates).
	vec3 wcNormal = mat3(modelToWorldMatrix) * inNormal;
// Using "reflect", we reflect the view direction to a reflected direction
	reflectedView = reflect(viewDirectionInWorldCoord, normalize(wcNormal)); // world coord = model of skybox
}
