#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 color;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	gl_Position = proj * view * model * vec4(position,1.0f);

	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(position,1.0f));
	TexCoords = texCoords;
	Color = color;
}