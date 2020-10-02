#version 460 core

struct Light
{
vec3 pos;
float ambient;
float diffuse;
vec3 color;
};

struct Mat
{
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
float transparency;
};

in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;
in vec3 FragPos;

out vec4 fColor;

uniform sampler2D matDiffuseMap;
uniform vec3 cameraPos;
uniform Light light;
uniform Mat mat;

void main()
{
	//Ambient
	vec3 ambient = light.ambient * mat.ambient * texture(matDiffuseMap, TexCoords).rgb * light.color;

	//Diffuse

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.pos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * mat.diffuse * texture(matDiffuseMap, TexCoords).rgb * light.color;

	//Specular
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 specular = spec * mat.specular * light.color;

	//Result 

	vec3 result = ambient + diffuse + specular;
	fColor = vec4(result, mat.transparency);
}