#version 400
//Defines
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 12

//In
in vec2 TexCoord;
in vec3 Normal;
in vec3 SurfPos;

//Out
out vec4 out_Color;

//Uniform
uniform vec3 viewPos;
uniform sampler2D tex1,tex2,tex3,map;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100.0);//Shininess
    // combine results
    return light.diffuse*diff;// also ugly but whatever(light.ambient + light.diffuse*diff + light.specular*spec);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 SurfPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - SurfPos);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);//shininess
    // attenuation
    float distance    = length(light.position - SurfPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +light.quadratic * (distance * distance));
    // combine results
    return attenuation*(light.ambient + light.diffuse*diff + light.specular*spec);
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - SurfPos);
    float viewdist = 1/(0.2*length(viewPos-SurfPos)+1);
    vec3 result = vec3(0,0,0);
    // phase 1: Directional lighting
    result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, SurfPos, viewDir);

		vec4 m = normalize(texture(map, TexCoord/255));

    out_Color = vec4(result, 1.0)*(texture(tex1, TexCoord) * m.r +
				texture(tex2, TexCoord) * m.g +
				texture(tex3, TexCoord) * m.b);
}
