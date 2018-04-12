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
#define NR_POINT_LIGHTS 4

//In
in vec2 TexCoord;
in vec3 Normal;
in vec3 SurfPos;

//Out
out vec4 out_Color;

//Uniform
uniform vec3 viewPos;
uniform sampler2D texUnit;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 temp = vec3(1.0,1.0,1.0);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100.0);//Shininess
    // combine results
    vec3 ambient  = light.ambient  * temp;//vec3(texture(texUnit, TexCoord));
    vec3 diffuse  = light.diffuse  * diff *temp;// vec3(texture(texUnit, TexCoord));
    vec3 specular = light.specular * spec * temp;//vec3(texture(texUnit, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 SurfPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - SurfPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100.0);//shininess
    // attenuation
    float distance    = length(light.position - SurfPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texUnit, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texUnit, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texUnit, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - SurfPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, SurfPos, viewDir);
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, SurfPos, viewDir);

    out_Color = vec4(result, 1.0);
}
